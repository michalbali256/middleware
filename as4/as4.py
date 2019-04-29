
"""
	chat assignment implementation for the Middleware course
	authors:
		Jiri Kralovec (2014)
"""

# With this, it is not necessary to adjust python path in order to work with the chord module
import sys
sys.path.insert(0, 'chord')

# This adds backward compatibility for Python2
try: input = raw_input
except NameError: pass


from chord import Local
# from remote import Remote
from address import Address
from settings import SIZE
# from chord import retry_on_socket_error
import json
import socket
# import threading
import hashlib


def hash_string(string):
	"""Produces a chord hash (id) for a string"""

	# Hash string to integer (stable across executions, processes, nodes)
	sha256 = hashlib.sha256()
	sha256.update(string.encode('utf-8'))
	digest = sha256.hexdigest()
	ret = int(digest, 16) % SIZE

	# Hash used to be stable, but now it is salted, do not use it here !
	#ret = hash(string) % SIZE

	return ret


class Chat(object):
	"""Data structure that represents a member of a chat network."""

	def __init__(self, local_address, remote_address, name):
		"""
		:param local_address: address of the local node.
		:param remote_address: address of the remote node to bootstrap from.
		:param name: name belonging to the local node (name of the local user)
		"""

		self.local_ = Local(local_address, remote_address)  # local chord node

		self.local_.register_command("msg", self._accept_msg)
		self.local_.set_notify_handler(self._predecessor_change)

		self.local_.start()  # starting the local chord node
		self.msg_handler_ = None  # handler for outputting chat messages

		self.name_ = name

	def set_msg_handler(self, handler):
		"""Sets handler of the incoming messages. The handler must be
		a function accepting a message as its first parameter."""
		self.msg_handler_ = handler

	def _accept_msg(self, msg):
		"""Handler of a msg command."""
		try:
			msg = json.loads(msg)
			if int(msg['key']) == self.local_.id() \
					and self.msg_handler_ is not None:
				self.msg_handler_(msg['value'])
			else:
				print('received message not for us but for: ', msg['key'])
		except (KeyError, ValueError) as error:
			print("bad message accepted", error)
		return ''

	def send_msg(self, node_id, msg):
		"""Attempts to send a message msg to a node with id node_id."""
		try:
			node_id = node_id % SIZE
			str_hash = str(node_id)
			print("trying to send message to: " + str(node_id) \
				  + " with hash: " + str_hash)
			node = self.local_.find_successor(node_id)
			node.user_command("msg", json.dumps({
				'key': str_hash,
				'value': 'message from ' + self.name_ + ": " + msg
			}))
			print("message sent")
		except socket.error:
			print("send failed")

	def _predecessor_change(self, new_predecessor):
		"""Handler of the change of the predecessor event of the local chord
		node.
		:param new_predecessor: new predecessor of our node."""
		pass



	def shutdown(self):
		"""Shuts down the chat node."""
		print("shutting down ...")
		self.local_.shutdown()


def print_program_usage():
	"""Prints program usage to a standard output"""
	print("Usage:")
	print("python chat.py [-j <bootstrap_address>] <our_address> <name>")
	print("where <name> is a name under which we want to be known in the chat")
	print("and <our_address> is an address of our local machine " \
		  "where we will be listening (it also determines an id of out node)")
	print("if only <our_address> and <name> are given a new chord " \
		  "network is started with only our node")
	print("if the optional [-j <bootstrap_address>] is given our node " \
		  "will join an existing chord network through")
	print("node on address <bootstrap_address>")
	print()
	print("<*_address> may be a port number, then the IP address " \
		  "will be determined by gethostbyname(gethostname())")
	print("which will be an IPv4 address of some network interface we have")
	print("or it may be in format address:port where address is an " \
		  "IPv4 address or domain name,")
	print("for example: 77.75.72.3:5000 or localhost:6000")


def print_usage():
	"""Prints commands that the program accepts on the standard input."""
	print("Usage:")
	print("msg <name> <message> - sends a message to peer identified by <name>")
	print("send <channel> <message> - sends a message to channel name <channel>")
	print("join <channel> - joins a <channel> - " \
		  "we receive message from the channel afterwards")
	print("leave <channel> - leaves a joined channel")
	print("exit - exits the program")


def is_hundred_and_twenty_seven(ip_address):
	"""Returns whether given ip address (string) is the 127.0.0.1 address."""
	return [int(x) for x in ip_address.split('.')] == [127, 0, 0, 1]


def parse_int(string):
	"""Attempts to convert given parameter to an int, returns the
	integer value or None when the conversion fails."""
	try:
		return int(string)
	except ValueError:
		return None


def parse_address(string):
	"""Parses network address from the 'ip:port' or 'port' format."""
	tmp = string.split(":", 1)
	try:
		hostname = socket.gethostname()
		port = None
		if len(tmp) == 1:  # only port was given address is localhost
			port = parse_int(string)
		elif len(tmp) == 2:  # address:port was given
			hostname = socket.gethostbyname(tmp[0])
			port = parse_int(tmp[1])
		if port is not None:
			return Address(hostname, port)
		raise ValueError('wrong address: ' + string)
	except socket.gaierror as e:
		raise ValueError("wrong address: " + string)


def parse_program_params():
	"""Attempts to load console parameters."""
	local_address = None  # out address
	remote_address = None  # address of the bootstrap node
	name = None  # our chat name

	try:
		if len(sys.argv) == 3:
			local_address = parse_address(sys.argv[1])
			name = sys.argv[2]
		elif len(sys.argv) == 5 and sys.argv[1] == '-j':
			remote_address = parse_address(sys.argv[2])
			local_address = parse_address(sys.argv[3])
			name = sys.argv[4]
		else:
			print_program_usage()
			exit()
	except ValueError as error:
		print(error)
		print()
		print_program_usage()
		exit()

	if remote_address is not None and \
			(is_hundred_and_twenty_seven(remote_address.ip) and
				 not is_hundred_and_twenty_seven(local_address.ip) or
					 not is_hundred_and_twenty_seven(remote_address.ip) and
					 is_hundred_and_twenty_seven(local_address.ip)):
		print("error: either both ip addresses have to be" \
			  " 127.0.0.1 or none of them")
		print_program_usage()
		exit()

	return (local_address, remote_address, name)


def main_program():
	"""Main function of the chat program."""
	print("Welcome in the Chat!")

	local_address, remote_address, name = parse_program_params()

	def msg_printer(msg):
		"""Handler of an incoming message, prints the message to the
		standard output."""
		print(msg)

	chat = Chat(local_address, remote_address, name)
	print("our address is: %s:%s" % (local_address.ip, local_address.port))
	print("our id is: " + str(hash(local_address)))
	print("our name is: " + name + " name id: " + str(hash_string(name)))
	if remote_address is not None:
		remote_hash = str(hash(remote_address))
		print("bootstrap address is: %s:%s id: %s" % \
			  (remote_address.ip, remote_address.port, remote_hash))
	print_usage()
	chat.set_msg_handler(msg_printer)

	end = False
	while not end:
		user_input = input().split(" ", 2)
		if len(user_input) == 1 and user_input[0] == "exit":
			end = True
			chat.shutdown()
		elif len(user_input) == 3 and user_input[0] == "msg":
			chat.send_msg(int(user_input[1]), user_input[2])
		elif len(user_input) == 2 and user_input[0] == "join":
			pass
		elif len(user_input) == 2 and user_input[0] == "leave":
			pass
		elif len(user_input) == 3 and user_input[0] == "send":
			pass
		else:
			print("unknown command")


if __name__ == "__main__":
	import sys

	main_program()
