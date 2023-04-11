from socket import *

client = socket(AF_UNIX, SOCK_STREAM)
client.connect('/tmp/socket')
client.send(b"Testing 1 2 3")
client.settimeout(1)
print(client.recv(10))
client.close()
