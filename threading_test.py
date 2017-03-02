import os
import subprocess
import signal
import socket

server_process = subprocess.Popen("./webserver example_config", stdout=subprocess.PIPE, shell=True)

req = "GET /echo HTTP/1.0\r\n\r\n"

s1 = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
s2 = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
s1.connect(("localhost",2020))
s2.connect(("localhost",2020))
s1.send(req) 
s2.send(req) 
s1.send("ABCD\r\n\r\n") 
s2.send("ABCD\r\n\r\n") 
buf1 = s1.recv(1024) 
buf2 = s2.recv(1024) 

print("===========Response1==============")
print(buf1)
print("===========Response2==============")
print(buf2)

os.killpg(os.getpgid(server_process.pid), signal.SIGTERM)