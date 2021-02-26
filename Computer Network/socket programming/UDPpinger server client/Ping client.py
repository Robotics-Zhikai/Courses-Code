from socket import *
import time

clientSocket = socket(AF_INET,SOCK_DGRAM)
serverIP = '192.168.0.104'
serverPort = 12000
clientSocket.settimeout(1.0)
# clientSocket.sendto('阿嘎嘎'.encode(),(serverIP,serverPort))
# clientSocket.close()
# time.sleep(100)
for i in range(10):
    message = 'Ping %d %s'%(i+1,str(time.time()))
    clientSocket.sendto(message.encode(),(serverIP,serverPort))
    sendtime = time.time()
    print("发送的message：",message)
    try:
        recvmessage,serveraddress = clientSocket.recvfrom(1024)
        rtt = time.time()-sendtime
        print("接收到的message",recvmessage)
        print("rtt：",rtt)
    except Exception as e:
        print("丢包了")
    # except clientSocket.timeout as e : #由于clientSocket.timeout不是继承于特定的python类，会报错
    #     print(e)

#还可以写个心跳程序，用来告诉服务器client还活着。
#具体实现方式是client每隔客户端处的固定时间给server发送一个带时间戳的心跳包，然后由server进行解析
#server收到的是字符串的形式，需要将字符串按照程序员给定的格式进行解析，得到相邻心跳包的时间间隔
#期间server应当能处理异常，如心跳包丢失、心跳包编号顺序乱序及其他