
from settings import SIZE
import socket
import time
import sys

def retry_on_socket_error(retry_limit):
	def decorator(func):
		def inner(self, *args, **kwargs):
			retry_count = 0
			last_error = None
			while retry_count < retry_limit:
				try:
					ret = func(self, *args, **kwargs)
					return ret
				except socket.error as e:
					last_error = e
					self.log("SOCKER ERROR caught in " + func.__name__ + " going to retry: " + str(e));
					# exp retry time
					time.sleep(2 ** retry_count)
					retry_count += 1
			if retry_count == retry_limit:
				raise last_error
		return inner
	return decorator

class Node(object):
    def __init__(self, address):
        self.address_ = address

    def address(self):
        return self.address_

    def id(self, offset=0):
        return (self.address_.__hash__() + offset) % SIZE

    def __eq__(self, other):
        return isinstance(other, Node) and self.address_ == other.address_
