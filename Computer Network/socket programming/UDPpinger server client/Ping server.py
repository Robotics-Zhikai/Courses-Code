import random
from socket import *
import time
serverSocket = socket(AF_INET,SOCK_DGRAM)
serverSocket.bind(('',12000)) #绑定当前IP地址及端口号


while 1:
    rand = random.randint(0,10)
    message,address = serverSocket.recvfrom(1024)
    message = message.upper()

    if rand<4: #如果rand小于4则认为发生了丢包不响应客户端 这是对丢包的一种模拟
        print("丢包模拟出现丢包,丢包信息为：",message.decode())
        continue
    print("接收到的message：",message.decode())
    time.sleep(0.53) #模拟延迟时间
    serverSocket.sendto(message,address) #当这个send了，但是对方对应的socket已经关闭了，那么就会报错远程主机强迫关闭了一个现有的连接
#这个没有close serversocket