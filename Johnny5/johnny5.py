import socket

from johnny5socket import socket_stuff

s = socket_stuff()
s.connect("140.228.0.0", 80)
s.send("Hello World")
