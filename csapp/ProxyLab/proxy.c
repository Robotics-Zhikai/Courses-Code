#include <stdio.h>
#include <stdlib.h>
#include <string.h> //memset
#include <netdb.h> //addrinfo
#include <unistd.h> //close 
#include <errno.h> //errno
#include <fcntl.h> // open
#include <pthread.h> //与线程有关的函数

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

#define MAXLINE 8192

//在函数前边加上static能保证本页的函数名字不与其他页的函数名字相冲突

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";

static int openlisten_fd(char * port)
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

static int openclient_fd(char * host,char * port)
{
    struct addrinfo hints;
    struct addrinfo * result,*p;
    
    memset(&hints,0,sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags |= AI_ADDRCONFIG;
    getaddrinfo(host,port,&hints,&result); //DNS查询IP地址

    int clientfd;
    for(p=result;p;p=p->ai_next)
    {
        if ((clientfd = socket(p->ai_family,p->ai_socktype,p->ai_protocol))<0)
            continue;
        if (connect(clientfd,p->ai_addr,p->ai_addrlen)<0)
        {
            close(clientfd);
            continue;
        }
        break;
    }

    freeaddrinfo(result);
    if (!p) //说明链表中的所有IP都不行
        return -1;
    return clientfd;
}


#define RIO_BUFSIZE 8192
typedef struct 
{
    int rio_fd; //将缓冲区与文件符连接起来
    int rio_cnt; //未读的数据还有多少
    char * rio_bufptr; //当前第一个未读数据的指针
    char rio_buf[RIO_BUFSIZE]; //内部缓冲区
}rIo_t;

static void rio_readinitb(rIo_t * rp,int fd)
{
    rp->rio_fd = fd;
    rp->rio_cnt = 0;
    rp->rio_bufptr = rp->rio_buf;
}

static int rio_read(rIo_t* rp,char * buf,size_t n)
//buf等于null时可以用在不断将缓冲区充满的用途上
{
    while(rp->rio_cnt<=0)
    {
        //当浏览器发送的目标IP地址正确，但是端口错误时，read一开始会阻塞一会儿，然后取消阻塞，返回0
        rp->rio_cnt = read(rp->rio_fd,rp->rio_buf,sizeof(rp->rio_buf));
        //read有可能读取出错，返回-1，因此while(rp->rio_cnt<=0)循环
        //当其为空时，fill the buf

        // printf("----------------------------------------------------------\n");
        // printf("%s\n",rp->rio_buf); //这里输出的值是没有任何问题的，完全按照预期 
        // printf("----------------------------------------------------------\n");

        if (rp->rio_cnt==0)
        {
            return 0; //说明遇到EOF了 fd里没东西 等作为客户端的时候探究一下read write的机制
        }
        else if (rp->rio_cnt<0)
        { //如果把这里的大括号去了就会出现read到的数据和server write的数据不一致的情况
          //从反汇编来看，可能是不加大括号的话会把read到的数据给弄乱
          //从上面printf出来的情况来看，printf出的是没什么问题的，主要是后边的代码把数据给弄乱了
            if (errno!=EINTR)
                //return -1; //被信号打断了 这个还不知道该怎么处理
                //printf("errno!=EINTR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
                return -1;
            else if (errno==EINTR)
                printf("errno==EINTR\n");
        }
        else 
            rp->rio_bufptr = rp->rio_buf;
    }
    if (buf!=NULL)
    {
        if (n > rp->rio_cnt)
        n = rp->rio_cnt;
        memcpy(buf,rp->rio_bufptr,n);
        rp->rio_bufptr+=n;
        rp->rio_cnt-=n;
        return n;
    }
    else
        return rp->rio_cnt; 
    
}

// /* $begin rio_read */
// static ssize_t rio_read(rIo_t *rp, char *usrbuf, size_t n)
// {
//     int cnt;

//     while (rp->rio_cnt <= 0) {  /* Refill if buf is empty */
// 	rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, 
// 			   sizeof(rp->rio_buf));
// 	if (rp->rio_cnt < 0) {
// 	    if (errno != EINTR) /* Interrupted by sig handler return */
// 		return -1;
// 	}
// 	else if (rp->rio_cnt == 0)  /* EOF */
// 	    return 0;
// 	else 
// 	    rp->rio_bufptr = rp->rio_buf; /* Reset buffer ptr */
//     }

//     if (usrbuf!=NULL)
//     {
//         /* Copy min(n, rp->rio_cnt) bytes from internal buf to user buf */
//         cnt = n;          
//         if (rp->rio_cnt < n)   
//         cnt = rp->rio_cnt;
//         memcpy(usrbuf, rp->rio_bufptr, cnt);
//         rp->rio_bufptr += cnt;
//         rp->rio_cnt -= cnt;
//         return cnt;
//     }
//     else
//         return rp->rio_cnt; 
    
// }
// /* $end rio_read */

static ssize_t rio_writen(int fd,const char * buf,const size_t n) //需要保证一定 write n字节
{
    size_t leftn = n;
    char * p = buf;
    int wc;
    while(leftn>0)
    {
        wc = write(fd,p,leftn); //否则的话这个如果被中断就直接退出了
        //https://stackoverflow.com/questions/11844284/why-isnt-write2-returning-eintr
        //write在进行写磁盘时，会处于不可被中断的情形；但是当在进行写socket时，中断是会发生的，需要处理EINTR
        //被中断的write返回-1，fd的当前位置不变
        if (wc<=0)
        {
            if (errno==EINTR) //被中断
                wc = 0;
            else 
                return -1;
        }
        leftn-=wc;
        p+=wc;
    }
    return n;
}
static void Rio_writen(int fd,const char * buf,const size_t n)
{
    if (rio_writen(fd,buf,n)!=n)
        printf("rio_writen函数写错啦！！！！！！！！！！！！\n");
}

static void rio_writeFlushALL(int fd,rIo_t * rp) //把所有缓冲中未处理的数据处理一遍，把rio_cnt置0
{
    if(rp->rio_cnt==0)
    {
        rp->rio_bufptr = rp->rio_buf;
        return;
    }
    Rio_writen(fd,rp->rio_bufptr,rp->rio_cnt);
    rp->rio_bufptr = rp->rio_buf;
    rp->rio_cnt = 0;
}

static ssize_t rio_readlineb(rIo_t * rp,char * buf,size_t maxlen)//读取一行 //为了防止溢出，所以有第三个参数
{
    char * pcur = buf;
    int rc;
    for(int i = 0;i<maxlen;i++)
    {
        char c;
        rc = rio_read(rp,&c,1);
        //printf("rc:%d\n",rc);
        if (rc>0)
        {
            *(pcur++) = c;//'\n'也读进去了
            if (c=='\n')
               break;
        }
        else if (rc==0)
        {
            if (i==0) //说明遇到EOF了,读完了
                return 0;
            else
                break;
        }
        else
            return -1; //读取出错
        
    }
    *pcur = '\0';
    return pcur-buf;
}



static void clienterror(int fd,char * cause,char *errnum,char* shortmsg,char * longmsg)
{
    char printfbuf[MAXLINE];

    //输出HTTP响应报文头
    sprintf(printfbuf,"HTTP/1.0 %s %s\r\n",errnum,shortmsg);
    rio_writen(fd,printfbuf,strlen(printfbuf));
    sprintf(printfbuf,"Content-type: text/html\r\n \r\n");
    rio_writen(fd,printfbuf,strlen(printfbuf));

    //输出HTTP响应报文
    sprintf(printfbuf,"<html><title>Proxy Error</title>");
    rio_writen(fd,printfbuf,strlen(printfbuf));
    sprintf(printfbuf,"<body bgcolor=""ffffff"">\r\n");
    rio_writen(fd,printfbuf,strlen(printfbuf));
    sprintf(printfbuf,"%s: %s\r\n",errnum,shortmsg);
    rio_writen(fd,printfbuf,strlen(printfbuf));
    sprintf(printfbuf,"<p>%s: %s\r\n",longmsg,cause);
    rio_writen(fd,printfbuf,strlen(printfbuf));
    sprintf(printfbuf,"<hr><em>The proxy server\r\n");
    rio_writen(fd,printfbuf,strlen(printfbuf));
    
}

static void parseURI(char * URI,char * host,char * port,char * filename)
{
    char * pbegin = URI;
    char * pend = pbegin;

    *host = '\0';
    *port = '\0';
    *filename = '\0';
    if (strstr(URI,"http://")==URI) //说明URI的开头是http://
    {
        pbegin+=strlen("http://");
        
        if ((pend=strchr(pbegin,':'))==NULL)
        {
            pend = strchr(pbegin,'/');
            memcpy(host,pbegin,pend-pbegin);
            *(host+(pend-pbegin)) = '\0';

            memcpy(port,"80",2); //默认的端口号是80
            *(port+2) = '\0';
        }
        else
        {
            memcpy(host,pbegin,pend-pbegin);
            *(host+(pend-pbegin)) = '\0';

            pbegin = pend+1;
            pend = strchr(pbegin,'/');
            memcpy(port,pbegin,pend-pbegin);
            *(port+(pend-pbegin)) = '\0';
        }
        pbegin =pend;
    }
    else if (strchr(URI,'/')==NULL) //连/都没找到，说明URI是无效的
        return;
    //否则的话就是浏览器中的URL直连web proxy
    strcpy(filename,pbegin);
}

typedef struct 
{
    char * headContent[MAXLINE];
}Httpheader;

void freeHttpheader(Httpheader * hp) //释放内存 防止内存泄露
{
    char ** p = hp->headContent;
    while(*p!=NULL)
    {
        free(*p);
        p++;
    }
}

static void sendHTTPheaderTofd(int fd,Httpheader * hp)
{
    char ** p = hp->headContent;
    char buf[MAXLINE];
    while(*p!=NULL)
    {
        if ((strstr(*p,"GET"))==*p)
        {
            char * locHost = strstr(*p,"localhost");
            //如果是localhost的话，就把localhost去掉，单纯保留后边的filename
            //因为如果是浏览器直接指名道姓连ip的话，浏览器发出的URI没有http 只有filename
            if (locHost!=NULL)
            {
                char* beg = strstr(locHost,"/");
                char* end = strstr(locHost,"\n")+1;
                memcpy(buf,beg,end-beg);
                *(buf+(end-beg)) = '\0';
                sprintf(*p,"GET %s",buf);
            }
                
        }

        if((strstr(*p,"Connection"))==*p)
        {
            sprintf(buf,"Connection: close\r\n");
            strcpy(*p,buf);
        }

        if((strstr(*p,"Proxy-Connection"))==*p)
        {
            sprintf(buf,"Proxy-Connection: close\r\n");
            strcpy(*p,buf);
        }

        rio_writen(fd,*p,strlen(*p));
        p++;
    }    
    // sprintf(buf,"GET %s HTTP/1.0\r\n",URI);
    // rio_writen(clientfd,buf,strlen(buf));
    // sprintf(buf,"User-Agent: %s\r\n",user_agent_hdr);
    // rio_writen(clientfd,buf,strlen(buf));
    // sprintf(buf,"Connection: close\r\n");
    // rio_writen(clientfd,buf,strlen(buf));
    // sprintf(buf,"Proxy-Connection: close\r\n");
    // rio_writen(clientfd,buf,strlen(buf));
}


static int parseHTTPheader(rIo_t * rp,Httpheader * hp) //解析HTTP报文头 不包括实体体
{
    char buf[MAXLINE];
    char ** p = hp->headContent;
    ssize_t rc;
    while((rc=rio_readlineb(rp,buf,MAXLINE))>0)
    {
        *p = malloc(strlen(buf)+1);
        memcpy(*p,buf,strlen(buf)+1);
        p++;
        //printf("%s\n",buf);
        if(strcmp(buf,"\r\n")==0)
            break;
    }
    *p = NULL;
    if (rc<0)
    {
        return -1; //说明读取出错
    }
    else if (rc==0)
        return 0; //说明没有读到任何东西
    return rc;
}
static int ParseHTTPheader(rIo_t * rp,Httpheader * hp)
{
    int rc;
    if ((rc=parseHTTPheader(rp,hp))<=0)
    {
        printf("parseHTTPheader(&riobuffer,&hp)==-1:rc=%d\n",rc);
    }
    return rc;
}

static int getHTTPbodyContentSize(Httpheader * hp)
{
    char ** p = hp->headContent;
    unsigned long size = 0;
    char buf[MAXLINE];
    while(*p!=NULL)
    {
        if((strstr(*p,"Content-Length"))==*p) //这个字段指的是entity body 的字节大小
        {
            char *pbegin,*pend;
            pbegin = strchr(*p,' ')+1;
            pend = strstr(*p,"\r\n");
            memcpy(buf,pbegin,pend-pbegin);
            *(buf+(pend-pbegin)) = '\0';
            size = atoi(buf);
            break;
        }
        p++;
    }
    if (*p==NULL)//说明HTTP报文中没有Content-Length字段
        return -1;
    return size;

}


static void doit(int fd)
{
    printf("fd:%d\n",fd);
    char buf[MAXLINE];
    memset(buf,0,MAXLINE);
    rIo_t riobuffer;
    
    // read(fd,buf,MAXLINE);
    // printf("%s\n",buf);
    Httpheader hp;
    rio_readinitb(&riobuffer,fd);
    //ParseHTTPheader(&riobuffer,&hp);

    if (ParseHTTPheader(&riobuffer,&hp)==0) //当是post时，就会返回0 不知道为啥
    {
        char ** p = hp.headContent;
        while (*p!=NULL)
        {
            printf("%s\n",*p);
            p++;
        }

        freeHttpheader(&hp);
        return;
    }
    
        
    strcpy(buf,hp.headContent[0]);

    char method[MAXLINE],URI[MAXLINE],version[MAXLINE];
    sscanf(buf,"%s %s %s",method,URI,version);

    if (strcasecmp(method,"GET")!=0) //第一个字符串不是GET
    {
        clienterror(fd,method,"501","Not Implemented","Proxy server Not implement this method");
        return;
    }
    
    char host[MAXLINE],port[MAXLINE],filename[MAXLINE];
    parseURI(URI,host,port,filename);
    printf("URI:%s host:%s port:%s filename:%s\n",URI,host,port,filename);
        

    int clientfd = openclient_fd(host,port); //代理服务器作为客户端，查IP地址并建立连接请求，旨在和已有的服务器建立连接
    if (clientfd<=0)
    {
        clienterror(fd,URI,"601","Not Connect","Web server Not Connect to host:port");
        return;
    }
    
    sendHTTPheaderTofd(clientfd,&hp);//proxy把从浏览器接收到的Get请求报文转发给server

    //接下来就开始处理从server返回的数据了,需要把返回的数据发送给浏览器
    rIo_t riobufferClient;
    rio_readinitb(&riobufferClient,clientfd);
    Httpheader hpClient;

    //sleep(1); 
    //proxy作为客户端报文头给远程server后，远程server给proxy发送http响应，响应具体是由响应头和响应数据主体组成的
    //如果不sleep(1)，那么如果proxy发送的报文头立刻就送到了远程server，然后远程server立刻开始写数据，
    //proxy在远程server写数据的过程中也开始读对应的套接字，
    //这样看似会造成冲突，导致proxy读到的数据和远程server发送的数据不一样，会有数据的丢失
    //但实际上是没有冲突的，造成上边的看似bug的原因主要是与rio_read里边if的一个大括号有关系
    ParseHTTPheader(&riobufferClient,&hpClient); 
    //给fd写数据是不断拓展fd的数据流，读fd是不断从头追赶数据流的末尾
    
    int ContentLength = getHTTPbodyContentSize(&hpClient);
    printf("ContentLength:%d\n",ContentLength);
    sendHTTPheaderTofd(fd,&hpClient); //发送报文头给浏览器
    if (ContentLength==-1)//当没有ContentLength字段时，可能是image/jpeg类型的数据，也可能是Not modified，被浏览器缓存的数据
    {
        char ** p = hpClient.headContent;
        while (*p!=NULL)
        {
            printf("%s\n",*p);
            p++;
        }
            
    }

    int receivedData = 0;
    // 跟http协议有关，有时候没有ContentLength字段，但是会有实体体，也需要进行发送
    while(rio_read(&riobufferClient,NULL,-1))//不断填满缓冲区然后发送，只有当遇到EOF时才退出while
    //如果直接read write的话，由于maxline的限制，不能把所有的字节都传递到位，会有丢失，导致浏览页面内容的丢失
    {   
        receivedData+=riobufferClient.rio_cnt;
        // printf("last:%d\n",riobufferClient.rio_cnt);
        rio_writeFlushALL(fd,&riobufferClient);
        // printf("after:%d\n",riobufferClient.rio_cnt);

        // if (rior==ContentLength)
        //     break;
    }
    printf("receivedData:%dB\n",receivedData);
        
    // char * contenttmp = (char *)malloc(ContentLength);

    
   // open("./cache/",)
    // read(clientfd,buf,sizeof(buf));
    // write(fd,buf,sizeof(buf));
    //printf("%s\r\n",buf);

    freeHttpheader(&hp);
    freeHttpheader(&hpClient);
    close(clientfd);
    
}

void * thread(void * argp)
{
    int connfd = *((int*)argp);
    pthread_detach(pthread_self());//分离当前线程，使得当前线程完成后能够被自动回收
    free(argp);
    //if ((i++)%5==0) //浏览器点击f12的网络选项能看到发送了什么HTTP请求以及对应的请求是否收到
    doit(connfd); //这里可能会出现的一个bug是可能在connfd连续发送多个请求报文，而目前实现的只处理一个请求报文然后返回，这样会丢失信息
    //浏览器只要输对了目标IP信息，就会给服务器发送过来，connfd就会接收到，也就是取消accept的阻塞
    //只不过对于监听端口能够接收到正确的信息
    //而浏览器如果没有输对端口，那么connfd收到的是空信息

    close(connfd);
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
    int * connfdp = NULL;
    pthread_t tid;
    while (1)
    {
        //以浏览器相对于proxy的角度来说，一问一答就做一个循环
        //实际上在打开hit.edu.cn的过程中，要从同一主机发很多链接，经过很多次一问一答

        connfdp = (int *)malloc(sizeof(int));
        *connfdp = accept(listenfd,&socketclient,&clientlen);//这是一个首先建立链接的过程
        //等待来自客户端的链接请求到达侦听描述符listenfd，然后在socketclient中写入客户端的套接字地址，并返回一个已连接描述符
        //当浏览器处发送的端口不是监听端口的话，也会取消accept的阻塞
        
        getnameinfo(&socketclient,sizeof(struct sockaddr),host,MAXLINE,service,MAXLINE,NI_NUMERICHOST);
        printf("get connection from (%s:%s)\n",host,service);
        printf("%d,%d\n",count++,*connfdp);
        //每次得到的service都不一样
        pthread_create(&tid,NULL,thread,connfdp);


        // doit(*connfdp); //注释掉的这两行是序列化的服务器，如果用序列化的服务器的化很容易就broken pipe
        // close(*connfdp); 
    }
    
    printf("%s", user_agent_hdr);
    return 0;
}



//关于并发服务器：可以采用生产者消费者模型来进行编写。主线程不停地接收来自客户端的链接请求（作为单一生产者），然后多个消费者进程
//在适当时机处理对应的连接

