import json
import socket
import threading

from address import Address
from settings import SIZE
from network import *
from node import *


# decorator to thread-safe Remote's socket
def requires_connection(func):
	""" initiates and cleans up connections with remote server """
	def inner(self, *args, **kwargs):
		try:
			self.log("locking...")
			self.mutex_.acquire()
			self.log("locked")

			self.open_connection()
			ret = func(self, *args, **kwargs)
			self.close_connection()
			self.mutex_.release()
			self.log("lock released")

			return ret
		except socket.error as e:
			self.log("SOCKET ERROR" + str(e))
			self.mutex_.release();
			self.log("lock released")
			# do not close the socket if it is not open yet
			if self.socket_ is not None:
				self.close_connection()
			raise e
	return inner


# class representing a remote peer
class Remote(Node):
	def __init__(self, remote_address):
		Node.__init__(self, remote_address)
		self.mutex_ = threading.Lock()

	def log(self, info):
		# f = open("./chord.log", "a+")
		# f.write(str(self.id()) + " : " +  info + "\n")
		# f.close()
		# print threading.currentThread().name + ": remote " + str(hash(self.address_)) + " : " +  info
		pass

	def open_connection(self):
		self.socket_ = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.socket_.connect((self.address_.ip, self.address_.port))

	def close_connection(self):
		self.socket_.close()
		self.socket_ = None

	def __str__(self):
		return "Remote %s" % self.address_

	def send(self, msg):
		self.log('sending: ' + msg)
		send_to_socket(self.socket_, msg)
		self.socket_.shutdown(socket.SHUT_WR)
		self.last_msg_send_ = msg

	def recv(self):
		# we use to have more complicated logic here
		# and we might have again, so I'm not getting rid of this yet
		response = read_from_socket(self.socket_)
		self.log('received: ' + response)
		return response

	def ping(self):
		s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		try:
			s.connect((self.address_.ip, self.address_.port))
			send_to_socket(s, '')
			s.close()
			self.log('ping successful')
			return True
		except socket.error as err:
			s.close()
			self.log("ping error: " + str(err))
			return False

	@requires_connection
	def command(self, msg):
		self.send(msg)
		response = self.recv()
		return response

	''' command parameter must not contain any whitespaces
	 msg must not contain no line feed or carriage return characters '''
	@retry_on_socket_error(3)
	@requires_connection
	def user_command(self, command, msg):
		self.send(command + ' ' + msg)
		response = self.recv()
		return response

	@requires_connection
	def get_successors(self):
		self.send('get_successors')

		response = self.recv()
		# if our next guy doesn't have successors, return empty list
		if response == "":
			return []
		response = json.loads(response)
		return list(map(lambda address: Remote(Address(address[0], address[1])), response))

	@requires_connection
	def successor(self):
		self.send('get_successor')

		response = json.loads(self.recv())
		return Remote(Address(response[0], response[1]))

	@requires_connection
	def predecessor(self):
		self.send('get_predecessor')

		response = self.recv()
		response = json.loads(response)
		if response == "":
			return None
		return Remote(Address(response[0], response[1]))

	@requires_connection
	def find_successor(self, id):
		self.send('find_successor %s' % id)

		response = json.loads(self.recv())
		return Remote(Address(response[0], response[1]))

	@requires_connection
	def closest_preceding_finger(self, id):
		self.send('closest_preceding_finger %s' % id)

		response = json.loads(self.recv())
		return Remote(Address(response[0], response[1]))

	@requires_connection
	def notify(self, node):
		self.send('notify %s %s' % (node.address_.ip, node.address_.port))
