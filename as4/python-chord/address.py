from settings import SIZE
from hash import hash_string

# Helper function to determine if a key falls within a range
def inrange(c, a, b):
	# is c in [a,b)?, if a == b then it assumes a full circle
	# on the DHT, so it returns True.
	a = a % SIZE
	b = b % SIZE
	c = c % SIZE
	if a < b:
		return a <= c and c < b
	return (a < c and c < SIZE) or (0 <= c and c < b)


class Address(object):
	def __init__(self, ip, port):
		self.ip = ip
		self.port = int(port)
		# print "%s:%s" % (ip, port)

	def __hash__(self):
		return hash_string("%s:%s" % (self.ip, self.port)) % SIZE

	def __cmp__(self, other):
		return other.__hash__() < self.__hash__()

	def __lt__(self, other):
		return self.__hash__() < other.__hash__()

	def __eq__(self, other):
		return other.__hash__() == self.__hash__()

	def __str__(self):
		return "[\"%s\", %s]" % (self.ip, self.port)
