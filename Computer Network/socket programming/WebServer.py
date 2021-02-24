#客户端应该访问
#http://192.168.0.100:6789/hellozk.html
#但是经常出现recv一个空值 不知道为啥
#192.168是C类端口，适用于小型的局域网，外界是连不进来的 因此只有连到一个路由器上的终端才能访问192.168 而外网比如手机流量是连不进来的
from socket import  *
serverSocket = socket(AF_INET,SOCK_STREAM)

#prepare a server socket

serverPort = 6789
serverSocket.bind(('',serverPort))
serverSocket.listen(1) #最大连接数为1
print('the server is ready to receive')

while 1:
    #Establish the connection
    print('ready to serve ... ')
    connectionsocket,addr = serverSocket.accept()  #在这里就阻塞住了 直到accepted
    try:
        message = connectionsocket.recv(1024).decode() #这个不定期就会收到一个空值 不知道为啥
        # print('message:',message)
        print(message.split())
        filename = message.split()[1]
        # print('filename:',filename)
        # print('filename[1:]:',filename[1:])
        f = open(filename[1:])
        outputdata = f.read()
        #send one http header line into socket
        header = 'HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Type: text/html\r\nContent-Length: %d\r\n\r\n'%(len(outputdata))
        connectionsocket.send(header.encode())
        #send the content of the requested file to the client
        for i in range(0,len(outputdata)):
            connectionsocket.send(outputdata[i].encode())
        connectionsocket.close()
    except IOError:
        #发送文件没有找到
        header = 'HTTP/1.1 404 notFound'
        connectionsocket.send(header.encode())
        connectionsocket.close()
serverSocket.close()