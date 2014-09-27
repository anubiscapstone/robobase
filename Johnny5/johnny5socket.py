import socket

class socket_stuff:

    MSGLEN = 10

    def __init__(self, sock = None):
        if sock is None:
            self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        else:
            self.sock = sock

    def connect(self, host, port):
        self.sock.connect((host, port))

    def send(self, msg):
        sent_num = 0
        while sent_num < MSGLEN:
            sent = self.sock.send(msg[sent_num:])
            sent_num += sent

    def receive(self):
        chunks = []
        bytes_recd=0
        while bytes_recd < MSGLEN:
            chunk = self.sock.recv(min(MSGLEN - bytes_recd, 2048))
            chunks.append(chunk)
            bytes_recd += len(chunk)
        return ''.join(chunks)
