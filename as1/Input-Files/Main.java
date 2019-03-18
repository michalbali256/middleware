import java.util.Random;
import java.rmi.Naming;
import java.rmi.RemoteException;

public class Main {
	// How many nodes and how many edges to create.
	private static final int GRAPH_NODES = 1000;
	private static final int GRAPH_EDGES = 2000;

	// How many searches to perform
	private static final int SEARCHES = 50;

	private static Node[] nodes;

	private static Random random = new Random();
	private static Searcher searcher = new SearcherImpl();

	/**
	 * Creates nodes of a graph.
	 * 
	 * @param howMany number of nodes
	 */
	
	public static void createNodesLocal(int howMany) {
		nodes = new Node[howMany];

		for (int i = 0; i < howMany; i++) {
			nodes[i] = new NodeImpl();
		}
	}

	public static void createNodesRemote(int howMany) throws RemoteException
	{
		nodes = new Node[howMany];

		for (int i = 0; i < howMany; i++) {
			nodes[i] = new NodeRemoteImpl();
		}
	}

	public static void createNodesFromServer(NodeRemoteProvider provider, int howMany) throws RemoteException
	{
		nodes = new Node[howMany];

		for (int i = 0; i < howMany; i++) {
			nodes[i] = provider.makeNode();
		}
	}

	/**
	 * Creates a fully connected graph.
	 */
	public static void connectAllNodes() throws RemoteException {
		for (int idxFrom = 0; idxFrom < nodes.length; idxFrom++) {
			for (int idxTo = idxFrom + 1; idxTo < nodes.length; idxTo++) {
				nodes[idxFrom].addNeighbor(nodes[idxTo]);
				nodes[idxTo].addNeighbor(nodes[idxFrom]);
			}
		}
	}

	/**
	 * Creates a randomly connected graph.
	 * 
	 * @param howMany number of edges
	 */
	public static void connectSomeNodes(int howMany) throws RemoteException {
		for (int i = 0; i < howMany; i++) {
			final int idxFrom = random.nextInt(nodes.length);
			final int idxTo = random.nextInt(nodes.length);

			nodes[idxFrom].addNeighbor(nodes[idxTo]);
		}
	}

	/**
	 * Runs a quick measurement on the graph.
	 * 
	 * @param howMany number of measurements
	 */
	public static void searchBenchmark(int howMany) throws RemoteException {
		// Display measurement header.
		long sum = 0;
		long sumT = 0;
		//System.out.printf("%7s %8s %13s %13s%n", "Attempt", "Distance", "Time", "TTime");
		for (int i = 0; i < howMany; i++) {
			// Select two random nodes.
			final int idxFrom = random.nextInt(nodes.length);
			final int idxTo = random.nextInt(nodes.length);

			// Calculate distance, measure operation time
			final long startTimeNs = System.nanoTime();
			final int distance = searcher.getDistance(nodes[idxFrom], nodes[idxTo]);
			final long durationNs = System.nanoTime() - startTimeNs;
			sum += durationNs;

			// Calculate transitive distance, measure operation time
			final long startTimeTransitiveNs = System.nanoTime();
			final int distanceTransitive = searcher.getDistanceTransitive(2, nodes[idxFrom], nodes[idxTo]);
			final long durationTransitiveNs = System.nanoTime() - startTimeTransitiveNs;
			sumT += durationTransitiveNs;
			if (distance != distanceTransitive) {
				System.out.printf("Standard and transitive algorithms inconsistent (%d != %d)%n", distance,
						distanceTransitive);
			} else {
				// Print the measurement result.
				//System.out.printf("%7d %8d %13d %13d%n", i, distance, durationNs / 1000, durationTransitiveNs / 1000);
			}
		}
		System.out.printf("%13d %13d\n", sum, sumT);

	}

	public static void main(String[] args) {
		
		long seed = random.nextLong();
		
		try
		{
			// Use the registry on this host to find the server.
			// The host name must be changed if the server uses
			// another computer than the client!
			
			
			System.out.println ("GOT HERE");
			// Create a randomly connected graph and do a quick measurement.
			// Consider replacing connectSomeNodes with connectAllNodes to verify that all distances are equal to one.
			searcher = (Searcher) Naming.lookup("//u-pl30/Searcher");
			System.out.println ("GOT HERE");
			//remote searcher, local nodes serialized
			System.out.println("------------ Remote searcher, local nodes -------------");
			random.setSeed(seed);
			createNodesLocal(GRAPH_NODES);
			connectSomeNodes(GRAPH_EDGES);
			searchBenchmark(SEARCHES);
			
			System.out.println("------------ Remote searcher, remote nodes -------------");

			NodeRemoteProvider provider = (NodeRemoteProvider) 
Naming.lookup ("//u-pl30/NodeRemoteProvider");
			random.setSeed(seed);
			createNodesFromServer(provider, GRAPH_NODES);
			connectSomeNodes(GRAPH_EDGES);
			searchBenchmark(SEARCHES);


			System.out.println("------------ Local searcher, remote nodes -------------");

			searcher = new SearcherImpl();
			random.setSeed(seed);
			createNodesFromServer(provider, GRAPH_NODES);
			connectSomeNodes(GRAPH_EDGES);
			searchBenchmark(SEARCHES);

			System.out.println("------------ Local searcher, local nodes -------------");
			//local searcher, local nodes serialized
			
		
			random.setSeed(seed);
			createNodesLocal(GRAPH_NODES);
			connectSomeNodes(GRAPH_EDGES);
			searchBenchmark(SEARCHES);


		}
		catch (Exception e)
		{ 
			System.out.println ("Client Exception: " + e.getMessage ());
			e.printStackTrace ();
		}
		
		
		
		
		
	
	}
}
