#192.168.0.101
#非常简陋的Proxy server
#但是由于浏览器有时候发出一些出乎意料的报文，所以处理的程序不太完美
#对于整体缓存器的处理逻辑有了一个大概的了解
#掌握熟练文件操作、HTTP协议后写出来的程序会更好更鲁棒
#体会到了异常处理程序的重要性
#实际的Proxy server还需要保证返回的网页和源处的网页一致，需要响应条件get
#等等等等等
from socket import *
import os

import sys
#
# if len(sys.argv)<=1:
#     print('usage:"python proxyserver.py server_ip"\n[server_ip:It is the IP address of proxy server')
#     sys.exit(2)

#创建一个server socket，将其绑定到端口上并建立侦听
serverSocket = socket(AF_INET,SOCK_STREAM)
serverPort = 8888
serverSocket.bind(('',8888)) #绑定到本机的IP地址和serverPort的端口号上
serverSocket.listen(1)
print('The server is ready to receive')

while 1:
    #开始从客户端收取数据
    print('ready to serve...')

    message = ''
    while message=='':
        ConnectionSocket,addr = serverSocket.accept()
        message = ConnectionSocket.recv(4096).decode()
    # print("received a connection from:",addr)
    # print("message",message)
    #从接受的数据中提取filename
    # print(message.split()[1]) #以空格为分割符，将message分割成一个可索引的数组
    filename = message.split()[1].partition("/")[2] #partition返回一个三元的元组，第一个为分隔符左边的字串，第二个为分隔符本身，第三个为分隔符右边的字串
    # print("filename",filename)
    fileExist = "false"
    try:
        #看文件是否在缓存内
        f = open(filename,"r") #这里如果不存在的话应该会throw exception
        outputdata = f.readlines()
        fileExist = "true"
        #代理服务器找到了缓存中的文件并返回一个响应
        # ConnectionSocket.send("HTTP/1.0 200 OK\r\n".encode())
        # ConnectionSocket.send("Content-Type:text/html\r\n".encode())
        for i in range(len(outputdata)):
            ConnectionSocket.sendall(outputdata[i].encode())

        print("read from cache________________________________________________________________________________________")
    except IOError:
        if fileExist == "false":
            #在代理服务器上创建一个TCPsocket
            print('Creating socket on proxyserver')
            c = socket(AF_INET,SOCK_STREAM)
            hostn = message.split()[1].partition("/")[2].partition("/")[0] #得到请求的主机
            dest =  message.split()[1].partition("/")[2].partition("/")[2] #得到请求的主机下的filename
            try:
                c.connect((hostn,80))
                messagenew = message.split()[0]+ ' /'+dest+ ' ' + message.split()[2]+'\r\n'
                messagenew+=message.split('\n')[1].split()[0]+' '+hostn+'\r\n'

                othermessage = message.split('\r\n')[2:]
                for i in othermessage:
                    messagenew+=i+'\r\n'
                c.sendall(messagenew.encode())
                buff = c.recv(4096)
                ConnectionSocket.sendall(buff)


                dirpath = 'D:\Study\硕士\我的工作\网课及其他学习笔记\计算机网络\socket实验\Proxy server/'+filename[:filename.rfind('/')]
                if not os.path.exists(dirpath):
                    os.makedirs(dirpath)
                os.chdir(dirpath)
                if filename.rfind('/')+1 != len(filename): #最后一个/后边非空
                    tmpfile = open(filename[filename.rfind('/')+1:],'w')
                    tmpfile.writelines(buff.decode().replace('\r\n','\n'))
                    tmpfile.close()
                else:
                    tmpfile = open('empty','w') #最后一个/后边为空
                    tmpfile.writelines(buff.decode().replace('\r\n','\n'))
                    tmpfile.close()
            except Exception:
                # print('无法处理的message',message)
                print('无法处理的message')
            # c.close()
        else:
            print("按理说不应该出现这样的情况，检查程序是否写错")

#github 上那个自顶向下的3kstar的代码根本不能用