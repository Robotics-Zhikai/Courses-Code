#include <stdio.h>
#include <stdlib.h>
#include <string.h> //memset
#include <netdb.h> //addrinfo
#include <unistd.h> //close 
#include <errno.h> //errno

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

#define MAXLINE 8192

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";

int openlisten_fd(char * port)
{
    struct addrinfo hints;
    struct addrinfo * result,*p;
    int rc,listenfd,optival=1;
    
    memset(&hints,0,sizeof(struct addrinfo));
    hints.ai_family = AF_INET; //将列表限制为IPv4地址
    hints.ai_socktype = SOCK_STREAM; 
    //getaddrinfo函数默认最多返回三个addrinfo结构，设置为SOCK_STREAM将列表限制为对每个地址最多一个addrinfo地址
    hints.ai_flags |= AI_PASSIVE;//这个标志告诉函数返回的套接字地址可能被服务器用作监听套接字
    hints.ai_flags |= AI_ADDRCONFIG;//仅当getaddrinfo函数返回的info链表至少配置有一个ipv4地址时，才返回ipv4地址信息
    hints.ai_flags |= AI_NUMERICSERV; //强制service参数使用十进制端口号的字符串形式，而不能是服务名
    if ((rc = getaddrinfo(NULL,port,&hints,&result))!=0)
    //host参数的值是NULL，表示告诉内核这个服务器会接收发送到 该主机的所有IP地址 的请求
    {
        fprintf(stderr,"getaddinfo faild(port %s):%s\n",port,gai_strerror(rc));
        return -2;
    }

    char host[MAXLINE];
    char service[MAXLINE];

    for(p=result;p;p=p->ai_next) //这应该就是在主机的所有可行的ip地址中找一个，然后作为监听套接字
    {
        if ((listenfd = socket(p->ai_family,p->ai_socktype,p->ai_protocol))<0)
            continue; //socket faild 暂时还不知道是什么原因failed了

        //getnameinfo(p->ai_addr,sizeof(struct sockaddr),host,MAXLINE,service,MAXLINE,NI_NUMERICHOST);
        //printf("host:%s,service:%s\n",host,service);

        setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,(const void *)&optival,sizeof(int));
        //./proxy 10800后，如果CtrlC后再重启服务器的话，就会默认造成服务器在30秒内拒绝客户端链接请求，
        //会造成所有的p都不能socket成功，对调试造成麻烦
        //加上这个语句后，就能够避免这样的情况

        if (bind(listenfd,p->ai_addr,sizeof(struct sockaddr_in))==0)
            break; //说明bind成功了
        if (close(listenfd)<0)
        {
            fprintf(stderr,"open_listenfd closed faild:%s\n",strerror(errno));
            return -1;
        }
    }

    //getaddrinfo的result参数中会调用malloc申请堆内存，需要调用freeaddrinfo释放
    freeaddrinfo(result);
    if (!p)
        return -1; //说明所有的getaddrinfo的套接字都不行
    
    if (listen(listenfd,1024)<0)//listen将默认socket得到的主动套接字转化为被动套接字
    {
        close(listenfd);
        return -1;
    }
    return listenfd;
}

void doit(int fd)
{
    char buf[MAXLINE];
    read(fd,buf,MAXLINE);
    printf("%s\n",buf);
}

int main(int argc,char ** argv)
{
    
    if (argc!=2)
    {
        fprintf(stderr,"usage:%s <port>\r\n",argv[0]);
        exit(1); //退出当前进程
    }
    char * port = argv[1];
    int listenfd = openlisten_fd(port);
    char host[MAXLINE];
    char service[MAXLINE];


    struct sockaddr socketclient;
    int count = 0;
    int clientlen = sizeof(struct sockaddr);
    while (1)
    {
        int connfd = accept(listenfd,&socketclient,&clientlen);
        //等待来自客户端的链接请求到达侦听描述符listenfd，然后在socketclient中写入客户端的套接字地址，并返回一个已连接描述符
        
        getnameinfo(&socketclient,sizeof(struct sockaddr),host,MAXLINE,service,MAXLINE,NI_NUMERICHOST);
        printf("get connection from (%s:%s)\n",host,service);
        printf("%d,%d\n",count++,connfd);

        doit(connfd);
    
        close(connfd);
    }
    
    printf("%s", user_agent_hdr);
    return 0;
}
