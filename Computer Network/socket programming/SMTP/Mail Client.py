from socket import *
import base64

subject = "邮件主题"
contenttype = "text/plain"

msg = "\r\n I love computer networks"
endmsg = "\r\n.\r\n"

mailserver = 'smtp.163.com' #向网易借用的客户端服务器地址
mailport = 25

fromaddress = '***************@163.com' #将要发送邮件的邮箱地址
toaddress = '***************@qq.com' #目的邮箱地址

#http://tool.chinaz.com/Tools/base64.aspx 网站base64
username = '***************jb20=' #fromaddress的带@163.com的base64编码
password = '***************UQ==' #网易邮箱的授权码的base64编码 不是密码 需要在网易邮箱的客户端内操作

clientSocket = socket(AF_INET,SOCK_STREAM) #TCP连接
clientSocket.connect((mailserver,mailport))

recv = clientSocket.recv(1024).decode()
print(recv)

if recv[:3] !='220': #可见SMTP主要处理这些数字
    print('220 不是从服务器发送并收到的')

helocommand = 'HELO Zhikai\r\n'
clientSocket.send(helocommand.encode())
recv1 = clientSocket.recv(1024).decode()
print(recv1)
if recv1[:3]!='250':
    print('250不是从服务器发送并收到的')

clientSocket.sendall('AUTH LOGIN\r\n'.encode()) #sendall表示全部发送完毕 否则如果没有发送完的话就throw exception
recv = clientSocket.recv(1024).decode()
print(recv)
if (recv[:3]!='334'):
    print('334不是从服务器发送并接收到的')
clientSocket.sendall((username+'\r\n').encode())
recv = clientSocket.recv(1024).decode()
print("username",recv)
if (recv[:3]!='334'):
    print('334不是从服务器发送并接收到的')
clientSocket.sendall((password+'\r\n').encode())
recv = clientSocket.recv(1024).decode()
print(recv)
if (recv[:3]!='235'):
    print('235不是从服务器发送并接收到的')
#经过这个登录，才能借助SMT.163服务器作为客户端服务器，发起下边的与目标邮件的地址


clientSocket.sendall(('MAIL FROM: <'+fromaddress+'>\r\n').encode())#这个要是与上边登录的不符，则会报错
recv = clientSocket.recv(1024).decode()
print(recv)
if (recv[:3]!='250'):
    print('250不是从服务器发送并接收到的')

clientSocket.sendall(('RCPT TO: <'+toaddress+'>\r\n').encode())
recv = clientSocket.recv(1024).decode()
print(recv)
if (recv[:3]!='250'):
    print('250不是从服务器发送并接收到的')

clientSocket.sendall('DATA\r\n'.encode())
recv = clientSocket.recv(1024).decode()
print(recv)
if (recv[:3]!='354'):
    print('354不是从服务器发送并接收到的')

headmessage ='from:'+fromaddress+'\r\n'
headmessage+='to:'+toaddress+'\r\n'
headmessage+='subject:'+subject+'\r\n'
headmessage += 'Content-Type:' + contenttype + '\t\n' #这个指明发送的内容是什么样的 是HTML 还是imagejpeg还是其他 也可能是混合类型
headmessage += '\r\n' + msg
#实际上到这一步的时候，已经与目标服务器建立了联系，但是目标服务器规定了必须有这样的报文头，才能不被识别为垃圾邮件
#必须有headmessage 否则发送不到对应的邮箱 返回一个554的错误代码
#554发送的邮件内容包含了未被许可的信息，或被系统识别为垃圾邮件。请检查是否有用户发送病毒或者垃圾邮件；
clientSocket.sendall(headmessage.encode())
clientSocket.sendall(endmsg.encode())
recv = clientSocket.recv(1024).decode()
print(recv)
if (recv[:3]!='250'):
    print('250不是从服务器发送并接收到的')

clientSocket.sendall('QUIT\r\n'.encode())
recv = clientSocket.recv(1024).decode()
print(recv)
if (recv[:3]!='221'):
    print('221不是从服务器发送并接收到的')

clientSocket.close()

#就是一个通信协议的问题 没什么难理解的