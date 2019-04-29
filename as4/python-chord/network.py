
import threading
import time

def curr_time():
	return time.strftime("%H:%M:%S", time.localtime())

def sock_log_recv(s, msg):
	print(curr_time() + " sock_log recv from " + str(s.getpeername()) + " on " + str(s.getsockname()) + " " + threading.currentThread().name + " len " + str(len(msg)) + ": " + msg)

def sock_log_send(s, msg):
	print(curr_time() + " sock_log send to " + str(s.getpeername()) + " from " + str(s.getsockname()) + " " + threading.currentThread().name + " len " + str(len(msg)) + ": " + msg)

# reads from socket until "\r\n"
def read_from_socket(s):
	result = ""
	while 1:
		data = s.recv(256).decode('utf-8')
		# sock_log_recv(s, data)
		if len(data) == 0:
			# sock_log_recv(s, "SOCKET ERROR")
			return result

		if data[-1:] == "\n":
			result += data[:-1]
			break
		result += data
	return result

# sends all on socket, adding "\r\n"
def send_to_socket(s, msg):
	# sock_log_send(s, msg)
	s.sendall((str(msg) + "\n").encode('utf=8'))
