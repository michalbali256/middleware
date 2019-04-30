
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
        self.local_.register_command("named_msg", self._accept_named_msg)
        self.local_.register_command("join", self._accept_join)
        self.local_.register_command("leave", self._accept_leave)
        self.local_.register_command("channel_msg", self._accept_channel_msg)
        self.local_.register_command("migrate", self._accept_migrate)
        self.local_.register_command("name", self._accept_name)
        self.local_.register_command("delete_name", self._accept_delete_name)
        self.local_.set_notify_handler(self._predecessor_change)

        self.local_.start()  # starting the local chord node
        self.msg_handler_ = None  # handler for outputting chat messages

        self.mntnd_chnls = dict()
        self.mntnd_names = dict()
        self.joined_channels = set()

        self.name_ = name

        name_hash = hash_string(name)
        print('name IDID' + str(name_hash))
        
        print('registering our name')
        node = self.local_.find_successor(name_hash)
        node.user_command("name", json.dumps({
            'name': name,
            'id': self.local_.id()
        }))

        self.local_.stabilize

    def set_msg_handler(self, handler):
        """Sets handler of the incoming messages. The handler must be
        a function accepting a message as its first parameter."""
        self.msg_handler_ = handler
    

    def _accept_name(self, msg):
        """Handler of a name command."""
        try:
            msg = json.loads(msg)
            self.mntnd_names[msg['name']] = msg['id']

            print('we map name ' + msg['name'] + ' to id ' + str(msg['id']))
        except (KeyError, ValueError) as error:
            print("bad message accepted", error)
        return ''

    def _accept_delete_name(self, msg):
        """Handler of a name command."""
        try:
            msg = json.loads(msg)

            name = msg['name']
            if name not in self.mntnd_names:
                print("accepted delete command for unknown name " + name)
                return ''
            del self.mntnd_names[name]

            print('deleted name ' + name + ' from maintained names')

        except (KeyError, ValueError) as error:
            print("bad message accepted", error)
        return ''

    def _accept_named_msg(self, msg):
        """Handler of a msg command."""
        try:
            msg = json.loads(msg)
            name = msg['name']

            if name == self.name_:
                if self.msg_handler_ is not None:
                    self.msg_handler_(msg['msg'])
                return ''

            if not name in self.mntnd_names:
                print('received a message for unknown name ' + name)
                return ''

            recp_id = self.mntnd_names[name]
            print('received a message for name ' + name + ', sending it to ' + str(recp_id))


            node = self.local_.find_successor(recp_id)
            node.user_command("msg", json.dumps({
                'key': str(recp_id),
                'value': msg['msg']
            }))
            
        except (KeyError, ValueError) as error:
            print("bad message accepted", error)
        return ''

    def send_named_msg(self, recp, msg):
        """Handler of a named msg command."""
        try:
            name_hash = hash_string(recp)
            print('sending message to ' + recp + ' with hash ' + str(name_hash))

            name_maintainer = self.local_.find_successor(name_hash)
            name_maintainer.user_command('named_msg', json.dumps({
                'name': recp,
                'msg': 'message from ' + self.name_ + ": " + msg
            }))

            print('message sent')
        except socket.error:
            print("send message failed")

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

    def _accept_migrate(self, msg):
        """Handler of a migrate command."""
        try:
            msg = json.loads(msg)
            print("migrate accepted")


            migrate_channels = msg['channels']

            for ch, ids in migrate_channels.items():
                for id in ids:
                    self._add_node_to_channel(ch, id)
                print('migrated channel ' + ch + ' with ' + str(len(ids)) + ' users.')
            
            migrate_names = msg['names']   
            
            for name, id in migrate_channels.items():
                mntnd_names[name] = id
            
            print("migrate done")

        except (KeyError, ValueError) as error:
            print("bad migrate accepted", error)
        return ''

    def _add_node_to_channel(self, ch_name, id):
        
        if not ( ch_name in self.mntnd_chnls):
            print('Creating new channel: ' + ch_name)
            self.mntnd_chnls[ch_name] = set()

        self.mntnd_chnls[ch_name].add(id)
        print('Node with id: ' + id + ' joined channel: ' + ch_name)


    def _accept_join(self, msg):
        """Handler of a join command."""
        try:
            msg = json.loads(msg)
            self._add_node_to_channel(msg['channel_name'], msg['joining_id'])
            
        except (KeyError, ValueError) as error:
            print("bad join request accepted", error)
        return ''

    def _accept_leave(self, msg):
        """Handler of a leave command."""
        try:
            msg = json.loads(msg)
            ch_name = msg['channel_name'] 
            if not ( ch_name in self.mntnd_chnls):
                print('This node is not maintainer of channel ' + ch_name + ", but leave command was received.")
                return ''

            id = msg['leaving_id'];
            if not ( id in self.mntnd_chnls[ch_name]):
                print('User with id ' + id + ' is not member of channel ' + ch_name + ".")
                return ''
            self.mntnd_chnls[ch_name].remove(id);
            print('Node with id: ' + msg['leaving_id'] + ' left channel: ' + ch_name)
        except (KeyError, ValueError) as error:
            print("bad message accepted", error)
        return ''

    def _accept_channel_msg(self, msg):
        """Handler of a channel_msg command."""
        try:
            msg = json.loads(msg)
            ch_name = msg['channel_name'] 
            if not ( ch_name in self.mntnd_chnls):
                print('This node is not maintainer of channel ' + ch_name + ", but send message command was received.")
                return ''

            for id in self.mntnd_chnls[ch_name]:
                try:
                    node = self.local_.find_successor(int(id))
                    node.user_command("msg", json.dumps({
                        'key': id,
                        'value': msg['msg']
                    }))
                except socket.error:
                    print("failed send of channel message to user " + id)
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
            
    def join_channel(self, ch_name):
        """ joins channel with name ch_name """
        try:
            ch_hash = hash_string(ch_name);
            print("trying to join channel " + str(ch_name) + " with hash: " + str(ch_hash))
            node = self.local_.find_successor(ch_hash)
            node.user_command("join", json.dumps({
                'channel_name': ch_name,
                'joining_id': str(self.local_.id())
            }))
            self.joined_channels.add(ch_name)
            print("channel joined")
        except socket.error:
            print("channel join failed")

    def leave_channel(self, ch_name):
        """ leaves channel with name ch_name """
        try:
            ch_hash = hash_string(ch_name);
            print("trying to leave channel " + str(ch_name) + " with hash: " + str(ch_hash))
            node = self.local_.find_successor(ch_hash)
            node.user_command("leave", json.dumps({
                'channel_name': ch_name,
                'leaving_id': str(self.local_.id())
            }))
            self.joined_channels.remove(ch_name)
            print("channel left")
        except socket.error:
            print("channel leave failed")

    def send_to_channel(self, ch_name, msg):
        try:
            ch_hash = hash_string(ch_name);
            print("sending message to channel " + str(ch_name) + " with hash: " + str(ch_hash))
            node = self.local_.find_successor(ch_hash)
            node.user_command("channel_msg", json.dumps({
                'channel_name': ch_name,
                'msg': 'message from ' + self.name_ + ' to channel ' + ch_name + ': ' + msg
            }))
            print("message to channel sent")
        except socket.error:
            print("channel send failed")

    def _predecessor_change(self, new_predecessor):
        """Handler of the change of the predecessor event of the local chord
        node.
        :param new_predecessor: new predecessor of our node."""

        if new_predecessor == self.local_:
            return;

        migrate_chan = dict()
        for ch, ids in list(self.mntnd_chnls.items()):
            ch_id = hash_string(ch)
            if not self.local_.is_ours(ch_id):
                migrate_chan[ch] = list(ids)
                del self.mntnd_chnls[ch]
                print('migrating channel ' + ch + ' to a new predecessor.')
        
        migrate_names = dict()
        for name, id in list(self.mntnd_names.items()):
            name_id = hash_string(name)
            if not self.local_.is_ours(name_id):
                migrate_names[name] = id
                del self.mntnd_names[name]
                print('migrating name ' + name + ' to a new predecessor.')
        
        if len(migrate_chan) > 0 or len(migrate_names) > 0:
            migrate = {'channels':migrate_chan, 'names':migrate_names}
            new_predecessor.user_command('migrate', json.dumps(migrate))

    def shutdown(self):
        """Shuts down the chat node."""
        print("shutting down ...")

        name_hash = hash_string(self.name_)
        del_name_node = self.local_.find_successor(name_hash)
        del_name_node.user_command("delete_name", json.dumps({
            'name': self.name_
        }))

        for ch in list(self.joined_channels):
            self.leave_channel(ch)
        
        
        migrate_chan = dict()
        for ch, ids in list(self.mntnd_chnls.items()):
            if len(ids) > 0:
                migrate_chan[ch] = list(ids)
                print('migrating channel ' + ch + ' to a new predecessor.')
        
        if len(migrate_chan) > 0 or len(self.mntnd_names) > 0:
            migrate = {'channels':migrate_chan, 'names': self.mntnd_names}
            succ = self.local_.successor()
            succ.user_command('migrate', json.dumps(migrate))

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
            chat.send_named_msg(user_input[1], user_input[2])
        elif len(user_input) == 2 and user_input[0] == "join":
            chat.join_channel(user_input[1])
        elif len(user_input) == 2 and user_input[0] == "leave":
            chat.leave_channel(user_input[1])
        elif len(user_input) == 3 and user_input[0] == "send":
            chat.send_to_channel(user_input[1], user_input[2])
        else:
            print("unknown command")


if __name__ == "__main__":
    import sys

    main_program()
