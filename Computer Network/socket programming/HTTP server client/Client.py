import sys
import time
from socket import *

serverIp = sys.argv[1]
serverPort = int(sys.argv[2])
filename = sys.argv[3]

requesthead1 = 'GET /'
requesthead2 = ' HTT245P/1.1\r\nHost: %s:%d\r\nConnection: keep-alive\r\nUpgrade-Insecure-Requests: 1\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8, \
               application/signed-exchange;v=b3\r\nPurpose: prefetch\r\nAccept-Encoding: gzip,deflate,br\r\nAccept-Language: zh-CN,zh;q=0.9,en;q=0.8'%(serverIp,serverPort)
#比如说在上边即便我写了个HTT245P/1.1 ，也会经过send TCP发送出去，然后由服务器端的程序保证协议的正确性
#所以要想编写出遵循某一协议的端代码都能用这些服务，发送的数据格式一定要是遵循约定的协议的，这样才能让遵循协议的解析程序使用
requesthead = requesthead1+filename+requesthead2
print(requesthead2)

clientsocket = socket(AF_INET,SOCK_STREAM)
clientsocket.connect((serverIp,serverPort))

clientsocket.send(requesthead.encode()) #send和recv就是发送和接收数据，具体发送什么样的数据不关心
#不管是什么协议，是需要程序去解析的 而不是说只有某个协议才接收，是由程序员写的程序负责的

for i in range(10):
    print(i)
    # mod = clientsocket.recv(1)
    mod = clientsocket.recv(1024)
    #recv是阻塞的，每当收到server的send时，就接收并步出；如果是server的send集中在一起发送时，可以合并成一个然后步出
    #可能有一定的接收间隔阈量，超出接收间隔就步出，在下一个recv进行接收
    #若server的TCP连接提前关闭，则所有recv都不阻塞，直到运行完客户端程序
    #若客户端的TCP连接提前关闭，则在server端报错远程主机强迫关闭了一个现有的连接
    print(mod.decode())
clientsocket.close()