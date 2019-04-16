import java.io.IOException;
import java.io.InputStreamReader;
import java.io.LineNumberReader;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Random;

import javax.jms.*;

import org.apache.activemq.ActiveMQConnectionFactory;

public class Client {
	
	/****	CONSTANTS	****/
	
	// name of the property specifying client's name
	public static final String CLIENT_NAME_PROPERTY = "clientName";

	// name of the topic for publishing offers
	public static final String OFFER_TOPIC = "Offers";

	public static final String CLIENT_SALE_QUEUE_SUFFIX = "SaleQueue";

	public static final String CLIENT_MSG_TYPE_KEY = "MsgType";

	public static final int CLIENT_MSG_TYPE_BUY = 28;

	public static final int CLIENT_REPORT_TYPE_NOT_ENOUGH_TRANSFERRED = 3;

	public static final String SENDER_KEY = "sender";
	/****	PRIVATE VARIABLES	****/
	
	// client's unique name
	private String clientName;

	// client's account number
	private int accountNumber;
	
	// offered goods, mapped by name
	private Map<String, Goods> offeredGoods;
	
	// available goods, mapped by seller's name 
	private Map<String, List<Goods>> availableGoods;
	
	// reserved goods, mapped by name of the goods
	private Map<String, Goods> reservedGoods;
	
	// buyer's names, mapped by their account numbers
	private Map<Integer, String> reserverAccounts;
	
	// buyer's reply destinations, mapped by their names
	private Map<String, Destination> reserverDestinations;
	
	// connection to the broker
	private Connection conn;
	
	// session for user-initiated synchronous messages
	private Session clientSession;

	// session for listening and reacting to asynchronous messages
	private Session eventSession;
	
	// sender for the clientSession
	private MessageProducer clientSender;
	
	// sender for the eventSession
	private MessageProducer eventSender;

	// receiver of synchronous replies
	private MessageConsumer replyReceiver;
	
	// topic to send and receiver offers
	private Topic offerTopic;
	
	// queue for sending messages to bank
	private Queue toBankQueue;

	// queue for sending synchronous ack messages to bank
	private Queue ackBankQueue;
	
	// queue for receiving synchronous replies
	private Queue replyQueue;

	// random number generator
	private Random rnd;
	
	// reader of lines from stdin
	private LineNumberReader in = new LineNumberReader(new InputStreamReader(System.in));
	
	/****	PRIVATE METHODS	****/
	
	/*
	 * Constructor, stores clientName, connection and initializes maps
	 */
	private Client(String clientName, Connection conn) {
		this.clientName = clientName;
		this.conn = conn;
		
		// initialize maps
		offeredGoods = new HashMap<String, Goods>();
		availableGoods = new HashMap<String, List<Goods>>();
		
		reservedGoods = new HashMap<String, Goods>();
		reserverAccounts = new HashMap<Integer, String>();
		reserverDestinations = new HashMap<String, Destination>();
		
		// generate some goods
		rnd = new Random();
		for (int i = 0; i < 10; i++) {
			addGoods();
		}
	}
	
	/*
	 * Generate a goods item
	 */
	private void addGoods() {
		String name = "";
		
		for (int i = 0; i < 4; i++) {
			char c = (char) ('A' + rnd.nextInt('Z' - 'A'));
			name += c;
		}
		
		offeredGoods.put(name, new Goods(name, rnd.nextInt(10000)));
	}
	
	/*
	 * Set up all JMS entities, get bank account, publish first goods offer 
	 */
	private void connect() throws JMSException {
        // create two sessions - one for synchronous and one for asynchronous processing
        clientSession = conn.createSession(false, Session.AUTO_ACKNOWLEDGE);
        eventSession = conn.createSession(false, Session.AUTO_ACKNOWLEDGE);
        
        // create (unbound) senders for the sessions
        clientSender = clientSession.createProducer(null);
        eventSender = eventSession.createProducer(null);
        
        // create queue for sending messages to bank
        toBankQueue = clientSession.createQueue(Bank.BANK_QUEUE);

		ackBankQueue = clientSession.createQueue(Bank.BANK_ACK_QUEUE);

        // create a temporary queue for receiving messages from bank
        Queue fromBankQueue = eventSession.createTemporaryQueue();

		// temporary receiver for the first reply from bank
		// note that although the receiver is created within a different session
		// than the queue, it is OK since the queue is used only within the
		// client session for the moment
        MessageConsumer tmpBankReceiver = clientSession.createConsumer(fromBankQueue);        
        
        // start processing messages
        conn.start();
        
        // request a bank account number
        Message msg = eventSession.createTextMessage(Bank.NEW_ACCOUNT_MSG);
        msg.setStringProperty(CLIENT_NAME_PROPERTY, clientName);
        // set ReplyTo that Bank will use to send me reply and later transfer reports
        msg.setJMSReplyTo(fromBankQueue);
        clientSender.send(toBankQueue, msg);
        
        // get reply from bank and store the account number
        TextMessage reply = (TextMessage) tmpBankReceiver.receive();
        accountNumber = Integer.parseInt(reply.getText());
        System.out.println("Account number: " + accountNumber);
        
        // close the temporary receiver
        tmpBankReceiver.close();
        
        // temporarily stop processing messages to finish initialization
        conn.stop();
        
        /* Processing bank reports */
        
        // create consumer of bank reports (from the fromBankQueue) on the event session
        MessageConsumer bankReceiver = eventSession.createConsumer(fromBankQueue);
        
        // set asynchronous listener for reports, using anonymous MessageListener
        // which just calls our designated method in its onMessage method
        bankReceiver.setMessageListener(new MessageListener() {
			@Override
			public void onMessage(Message msg) {
				try {
					processBankReport(msg);
				} catch (JMSException e) {
					e.printStackTrace();
				}
			}
        });
                
        // TODO finish the initialization
        
        /* Step 1: Processing offers */
		
		offerTopic = eventSession.createTopic(OFFER_TOPIC);
		MessageConsumer offerConsumer = eventSession.createConsumer(offerTopic);
		offerConsumer.setMessageListener(new MessageListener() {
            @Override
            public void onMessage(Message msg) {
                try {
                	//either offers are published to the topic
                	if(msg instanceof ObjectMessage)
                    	processOffer((ObjectMessage) msg);
                	//or a new client asks for offers
                	else if (msg instanceof TextMessage)
                		publishGoodsList(eventSender);
                } catch (JMSException e) {
                    e.printStackTrace();
                }
            }
        });
		
        // create a topic both for publishing and receiving offers
        // hint: Sessions have a createTopic() method
        
        // create a consumer of offers from the topic using the event session
        
        // set asynchronous listener for offers (see above how it can be done)
        // which should call processOffer()
        
        /* Step 2: Processing sale requests */

        Queue q = clientSession.createQueue(clientName + CLIENT_SALE_QUEUE_SUFFIX);

        MessageConsumer salesConsumer = eventSession.createConsumer(q);
        salesConsumer.setMessageListener(new MessageListener() {
			@Override
			public void onMessage(Message msg) {
				try {
					processSale(msg);
				} catch (JMSException e) {
					e.printStackTrace();
				}
			}
		});

        // create a queue for receiving sale requests (hint: Session has createQueue() method)
        // note that Session's createTemporaryQueue() is not usable here, the queue must have a name
        // that others will be able to determine from clientName (such as clientName + "SaleQueue")
                
        // create consumer of sale requests on the event session
                
        // set asynchronous listener for sale requests (see above how it can be done)
        // which should call processSale()
        
        // end TODO
        
        // create temporary queue for synchronous replies
        replyQueue = clientSession.createTemporaryQueue();
        
        // create synchronous receiver of the replies
        replyReceiver = clientSession.createConsumer(replyQueue);

        // restart message processing
        conn.start();

        TextMessage hello = eventSession.createTextMessage("new client");
        hello.setStringProperty(SENDER_KEY, clientName);
        eventSender.send(offerTopic, hello);

        // send list of offered goods
        publishGoodsList(clientSender);


    }

	/*
	 * Publish a list of offered goods
	 * Parameter is an (unbound) sender that fits into current session
	 * Sometimes we publish the list on user's request, sometimes we react to an event
	 */
	private void publishGoodsList(MessageProducer sender) throws JMSException {
		// TODO

		ObjectMessage m = clientSession.createObjectMessage(new ArrayList<Goods>(offeredGoods.values()));
		m.setStringProperty(SENDER_KEY, clientName);

		clientSender.send(offerTopic, m);

			// create a message (of appropriate type) holding the list of offered goods
			// which can be created like this: new ArrayList<Goods>(offeredGoods.values())

			// don't forget to include the clientName in the message so other clients know
			// who is sending the offer - see how connect() does it when sending message to bank

			// send the message using the sender passed as parameter

	}
	
	/*
	 * Send empty offer and disconnect from the broker 
	 */
	private void disconnect() throws JMSException {
		// delete all offered goods
		offeredGoods.clear();
		
		// send the empty list to indicate client quit
		publishGoodsList(clientSender);
		
		// close the connection to broker
		conn.close();
	}
	
	/*
	 * Print known goods that are offered by other clients
	 */
	private void list() {
		System.out.println("Available goods (name: price):");
		// iterate over sellers
		for (String sellerName : availableGoods.keySet()) {
			System.out.println("From " + sellerName);
			// iterate over goods offered by a seller
			for (Goods g : availableGoods.get(sellerName)) {
				System.out.println("  " + g);
			}
		}
	}
	
	/*
	 * Main interactive user loop
	 */
	private void loop() throws IOException, JMSException {
		// first connect to broker and setup everything
		connect();
		
		loop:
		while (true) {
			System.out.println("\nAvailable commands (type and press enter):");
			System.out.println(" l - list available goods");
			System.out.println(" p - publish list of offered goods");
			System.out.println(" b - buy goods");
			System.out.println(" e - buy goods for specified money");
			System.out.println(" a - current account balance");
			System.out.println(" q - quit");

			// read first character
			String line = in.readLine();
			int c = line.charAt(0);
			// throw away rest of the buffered line
			//in.re
			//while (in.ready()) in.read();
			switch (c) {
				case 'q':
					disconnect();
					break loop;
				case 'b':
					buy(false);
					break;
				case 'e':
					buy(true);
					break;
				case 'l':
					list();
					break;
				case 'a':
					accountBalance();
					break;
				case 'p':
					publishGoodsList(clientSender);
					System.out.println("List of offers published");
					break;
				case '\n':
				default:
					break;
			}
		}
	}

	private void accountBalance() throws IOException, JMSException
	{
		MapMessage bankMsg = clientSession.createMapMessage();
		bankMsg.setInt(Bank.ACCOUNT_KEY, accountNumber);
		bankMsg.setInt(Bank.ORDER_TYPE_KEY, Bank.ORDER_TYPE_GET_BALANCE);
		bankMsg.setJMSReplyTo(replyQueue);
		clientSender.send(toBankQueue, bankMsg);

		Message m = replyReceiver.receive();
		if (!(m instanceof MapMessage)){
			System.out.println("Received unknown message.");
		}

		System.out.println("My account balance is: " + Integer.toString (((MapMessage) m).getInt(Bank.BALANCE_KEY)));
	}
	
	/*
	 * Perform buying of goods
	 */
	private void buy(boolean specifyAmount) throws IOException, JMSException {
		// get information from the user
		System.out.println("Enter seller name:");
		String sellerName = in.readLine();
		System.out.println("Enter goods name:");
		String goodsName = in.readLine();

		int amount = 0;
		if (specifyAmount) {
			System.out.println("Enter amount to pay:");
			amount = Integer.parseInt(in.readLine());
		}
		// check if the seller exists
		List<Goods> sellerGoods = availableGoods.get(sellerName);
		if (sellerGoods == null) {
			System.out.println("Seller does not exist: " + sellerName);
			return;
		}
		
		// TODO
		
		// First consider what message types clients will use for communicating a sale
		// we will need to transfer multiple values (of String and int) in each message
		// MapMessage? ObjectMessage? TextMessage with extra properties?
		
		/* Step 1: send a message to the seller requesting the goods */
		Queue toSellerQueue = clientSession.createQueue(sellerName + CLIENT_SALE_QUEUE_SUFFIX);

		MapMessage msg = clientSession.createMapMessage();
		msg.setInt(CLIENT_MSG_TYPE_KEY, CLIENT_MSG_TYPE_BUY);
		msg.setString("buyerName", clientName);
		msg.setString(GOODS_KEY, goodsName);
		msg.setInt("buyerAccountNumber", accountNumber);
		msg.setJMSReplyTo(replyQueue);
		clientSender.send(toSellerQueue, msg);
		// create local reference to the seller's queue
		// similar to Step 2 in connect() but using sellerName instead of clientName
		
		// create message requesting sale of the goods
		// includes: clientName, goodsName, accountNumber
		// also include reply destination that the other client will use to send reply (replyQueue)
		// how? see how connect() uses SetJMSReplyTo() 
					
		// send the message (with clientSender)
		
		/* Step 2: get seller's response and process it */

		Message rep = replyReceiver.receive();

		if(!(rep instanceof MapMessage)) {
			System.out.println("Received unrecognized reply" + rep);
			return;
		}

		MapMessage repMap = (MapMessage) rep;

		if(!repMap.getBoolean(CLIENT_SALE_ACCEPTED_KEY))
		{
			System.out.println("The the goods are no longer available.");
			return;
		}

		// receive the reply (synchronously, using replyReceiver)
		
		// parse the reply (depends on your selected message format)
		// distinguish between "sell denied" and "sell accepted" message
		// in case of "denied", report to user and return from this method
		// in case of "accepted"
		// - obtain seller's account number and price to pay

		int price;
		if(specifyAmount)
			price = amount;
		else
			price = repMap.getInt(PRICE_KEY);

		int sellerAccount = repMap.getInt(SELLER_ACCOUNT_NUMBER_KEY);

		/* Step 3: send message to bank requesting money transfer */
		
		// create message ordering the bank to send money to seller
		MapMessage bankMsg = clientSession.createMapMessage();
		bankMsg.setStringProperty(CLIENT_NAME_PROPERTY, clientName);
		bankMsg.setInt(Bank.ORDER_TYPE_KEY, Bank.ORDER_TYPE_SEND);
		bankMsg.setInt(Bank.ORDER_RECEIVER_ACC_KEY, sellerAccount);
		bankMsg.setInt(Bank.AMOUNT_KEY, price);

		System.out.println("Sending $" + price + " to account " + sellerAccount);
		
		// send message to bank
		clientSender.send(toBankQueue, bankMsg);

		/* Step 4: wait for seller's sale confirmation */

		Message conf = replyReceiver.receive();
		if(!(conf instanceof MapMessage)) {
			System.out.println("Received unrecognized confirmation" + rep);
			return;
		}
		int reportType = ((MapMessage) conf).getInt(Bank.REPORT_TYPE_KEY);
		if(reportType == Bank.REPORT_TYPE_NOT_ENOUGH_MONEY) {
			System.out.println("The transaction failed. Not enough money.");
		} else if (reportType == Bank.REPORT_TYPE_RECEIVED) {
			if (((MapMessage) conf).getString(GOODS_KEY).equals(goodsName)) {
				System.out.println("The transaction was successful");
			} else {
				System.out.println("The transaction failed. Received wrong confirmation message: " + conf);
			}
		} else if (reportType == CLIENT_REPORT_TYPE_NOT_ENOUGH_TRANSFERRED)
		{
			System.out.println("Not enough money transferred for the item. The transaction failed.");
		}

		// receive the confirmation, similar to Step 2

		// parse message and verify it's confirmation message
		
		// report successful sale to the user
	}
	
	/*
	 * Process a message with goods offer
	 */
	private void processOffer(ObjectMessage msg) throws JMSException {
		// TODO
		String sender = msg.getStringProperty(SENDER_KEY);
		if(clientName.equals(sender))
			return;

		List<Goods> gods = (List<Goods>) msg.getObject();

		if (gods.isEmpty()) {
			availableGoods.remove(sender);
		} else {
			availableGoods.put(sender, gods);
		}

		// parse the message, obtaining sender's name and list of offered goods
		
		// should ignore messages sent from myself
		// if (clientName.equals(sender)) ...
		
		// store the list into availableGoods (replacing any previous offer)
		// empty list means disconnecting client, remove it from availableGoods completely
//
//
//
//
//
	}

	public static final String CLIENT_SALE_ACCEPTED_KEY = "accepted";
	public static final String SELLER_ACCOUNT_NUMBER_KEY = "sellerAccountNumber";
	public static final String PRICE_KEY = "price";
	public static final String GOODS_KEY = "goodsName";
	public static final int CLIENT_MSG_TYPE_BUY_REPLY = 31;

	/*
	 * Process message requesting a sale
	 */
	private void processSale(Message msg) throws JMSException {
		// TODO
		
		/* Step 1: parse the message */

		if (!(msg instanceof MapMessage)) {
			System.out.println("Received unknown message:\n: " + msg);
			return;
		}

		MapMessage sale = (MapMessage) msg;
		int type = sale.getInt(CLIENT_MSG_TYPE_KEY);

		if(type != CLIENT_MSG_TYPE_BUY) {
			System.out.println("Received unknown MapMessage:\n: " + msg);
			return;
		}

				String buyerName = sale.getString("buyerName");
		String goodsName = sale.getString(GOODS_KEY);
		int buyerAccountNumber = sale.getInt("buyerAccountNumber");
		Destination toBuyer = sale.getJMSReplyTo();

		// distinguish that it's the sale request message

		// obtain buyer's name (buyerName), goods name (goodsName) , buyer's account number (buyerAccount)
		
		// also obtain reply destination (buyerDest)
		// how? see for example Bank.processTextMessage()

		/* Step 2: decide what to do and modify data structures accordingly */

		MapMessage reply = clientSession.createMapMessage();
		reply.setInt(CLIENT_MSG_TYPE_KEY, CLIENT_MSG_TYPE_BUY_REPLY);
		Goods goods = offeredGoods.get(goodsName);
		if (goods == null)
		{
			reply.setBoolean(CLIENT_SALE_ACCEPTED_KEY,false);
		}
		else
		{
			offeredGoods.remove(goodsName);
			reservedGoods.put(buyerName, goods);
			reserverAccounts.put(buyerAccountNumber, buyerName);
			reserverDestinations.put(buyerName, toBuyer);

			reply.setBoolean(CLIENT_SALE_ACCEPTED_KEY,true);
			reply.setInt(SELLER_ACCOUNT_NUMBER_KEY, accountNumber);
			reply.setInt(PRICE_KEY, goods.price);
		}

		eventSender.send(toBuyer, reply);
		// check if we still offer this goods
//		Goods goods = offeredGoods.get(goodsName);
//		if (goods != null) ...

		// if yes, we should remove it from offeredGoods and publish new list
		// also it's useful to create a list of "reserved goods" together with buyer's information
		// such as name, account number, reply destination
//		offeredGoods.remove(goodsName);
//		reservedGoods.put(buyerName, goods);
//		reserverAccounts.put(buyerAccount, buyerName);
//		reserverDestinations.put(buyerName, buyerDest);
		
		/* Step 3: send reply message */
		
		// prepare reply message (accept or deny)
		// accept message includes: my account number (accountNumber), price (goods.price)
		
		// send reply
	}
	
	/*
	 * Process message with (transfer) report from the bank
	 */
	private void processBankReport(Message msg) throws JMSException {
		/* Step 1: parse the message */
		
		// Bank reports are sent as MapMessage
		if (msg instanceof MapMessage) {
			MapMessage mapMsg = (MapMessage) msg;
			// get report number
			int cmd = mapMsg.getInt(Bank.REPORT_TYPE_KEY);
			if (cmd == Bank.REPORT_TYPE_RECEIVED || cmd == Bank.REPORT_TYPE_NOT_ENOUGH_MONEY) {
				// get account number of sender and the amount of money sent
				int buyerAccount = mapMsg.getInt(Bank.REPORT_SENDER_ACC_KEY);
				// match the sender account with sender
				String buyerName = reserverAccounts.get(buyerAccount);

				//send a message to buyer in any case
				MapMessage conf = clientSession.createMapMessage();
				conf.setInt(Bank.REPORT_TYPE_KEY, cmd);

				if (cmd == Bank.REPORT_TYPE_RECEIVED) {
					int amount = mapMsg.getInt(Bank.AMOUNT_KEY);

					// match the reserved goods
					Goods g = reservedGoods.get(buyerName);

					//message to bank is necessary only if bank is ready to move the money
					MapMessage bankAck = clientSession.createMapMessage();

					System.out.println("Received $" + amount + " from " + buyerName);
					// did he pay enough?
					if (amount >= g.price) {
						// get the buyer's destination
						conf.setString(GOODS_KEY, g.name);

						//bank can make the transfer
						bankAck.setBoolean(Bank.ACK_KEY, true);


					} else {
						conf.setInt(Bank.REPORT_TYPE_KEY, CLIENT_REPORT_TYPE_NOT_ENOUGH_TRANSFERRED);
						//bank does not have permission to move the money
						bankAck.setBoolean(Bank.ACK_KEY, false);
						System.out.println("It is not enough money to buy " + g.name + ". Reverting transaction.");
					}

					eventSender.send(ackBankQueue, bankAck);
				}

				Destination buyerDest = reserverDestinations.get(buyerName);

				// remove the reserved goods and buyer-related information
				reserverDestinations.remove(buyerName);
				reserverAccounts.remove(buyerAccount);
				reservedGoods.remove(buyerName);


				eventSender.send(buyerDest, conf);

				publishGoodsList(eventSender);

			} else {
				System.out.println("Received unknown MapMessage:\n: " + msg);
			}
		} else {
			System.out.println("Received unknown message:\n: " + msg);
		}
	}
	
	/**** PUBLIC METHODS ****/
	
	/*
	 * Main method, creates client instance and runs its loop
	 */
	public static void main(String[] args) {

		if (args.length != 1) {
			System.err.println("Usage: ./client <clientName>");
			return;
		}
		
		Connection connection = null;
		
		Client client = null;
		
		try {
            // create connection to the broker.
            ActiveMQConnectionFactory connectionFactory = new ActiveMQConnectionFactory("tcp://localhost:61616");
            connection = connectionFactory.createConnection();
            
            // create instance of the client
            client = new Client(args[0], connection);
            
            // perform client loop
            client.loop();
		}
		catch (Exception e) {
			e.printStackTrace();
		} finally {
	        try {
	        	// always close the connection
	            connection.close();
	        } catch (Throwable ignore) {
	        	// ignore errors during close
	        }
		}
	}
}
