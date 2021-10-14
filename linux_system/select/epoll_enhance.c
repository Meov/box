#include <stdio.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_EVENTS 1024 /*监听上限*/
#define BUFLEN  4096    /*缓存区大小*/
#define SERV_PORT 6666  /*端口号*/

void recvdata(int fd,int events,void *arg);
void senddata(int fd,int events,void *arg);

/*描述就绪文件描述符的相关信息*/
struct myevent_s
{
    int fd;             //要监听的文件描述符
    int events;         //对应的监听事件，EPOLLIN和EPLLOUT
    void *arg;          //指向自己结构体指针
    void (*call_back)(int fd,int events,void *arg); //回调函数
    int status;         //是否在监听:1->在红黑树上(监听), 0->不在(不监听)
    char buf[BUFLEN];   
    int len;
    long last_active;   //记录每次加入红黑树 g_efd 的时间值
};

int g_efd;      //全局变量，作为红黑树根
struct myevent_s g_events[MAX_EVENTS+1];    //自定义结构体类型数组. +1-->listen fd


/*
 * 封装一个自定义事件，包括fd，这个fd的回调函数，还有一个额外的参数项
 * 注意：在封装这个事件的时候，为这个事件指明了回调函数，一般来说，一个fd只对一个特定的事件
 * 感兴趣，当这个事件发生的时候，就调用这个回调函数
 */
void eventset(struct myevent_s *ev, int fd, void (*call_back)(int fd,int events,void *arg), void *arg)
{
    ev->fd = fd;
    ev->call_back = call_back;
    ev->events = 0;
    ev->arg = arg;
    ev->status = 0;
    if(ev->len <= 0)
    {
        memset(ev->buf, 0, sizeof(ev->buf));
        ev->len = 0;
    }
    ev->last_active = time(NULL); //调用eventset函数的时间
    return;
}

/* 向 epoll监听的红黑树 添加一个文件描述符 */
void eventadd(int efd, int events, struct myevent_s *ev)
{
    struct epoll_event epv={0, {0}};
    int op = 0;
    epv.data.ptr = ev; // ptr指向一个结构体（之前的epoll模型红黑树上挂载的是文件描述符cfd和lfd，现在是ptr指针）
    epv.events = ev->events = events; //EPOLLIN 或 EPOLLOUT
    if(ev->status == 0)       //status 说明文件描述符是否在红黑树上 0不在，1 在
    {
        op = EPOLL_CTL_ADD; //将其加入红黑树 g_efd, 并将status置1
        ev->status = 1;
    }
    if(epoll_ctl(efd, op, ev->fd, &epv) < 0) // 添加一个节点
        printf("event add failed [fd=%d],events[%d]\n", ev->fd, events);
    else
        printf("event add OK [fd=%d],events[%0X]\n", ev->fd, events);
    return;
}

/* 从epoll 监听的 红黑树中删除一个文件描述符*/
void eventdel(int efd,struct myevent_s *ev)
{
    struct epoll_event epv = {0, {0}};
    if(ev->status != 1) //如果fd没有添加到监听树上，就不用删除，直接返回
        return;
    epv.data.ptr = NULL;
    ev->status = 0;
    epoll_ctl(efd, EPOLL_CTL_DEL, ev->fd, &epv);
    return;
}

/*  当有文件描述符就绪, epoll返回, 调用该函数与客户端建立链接 */
void acceptconn(int lfd,int events,void *arg)
{
    struct sockaddr_in cin;
    socklen_t len = sizeof(cin);
    int cfd, i;
    if((cfd = accept(lfd, (struct sockaddr *)&cin, &len)) == -1)
    {
        if(errno != EAGAIN && errno != EINTR)
        {
            sleep(1);
        }
        printf("%s:accept,%s\n",__func__, strerror(errno));
        return;
    }
    do
    {
        for(i = 0; i < MAX_EVENTS; i++) //从全局数组g_events中找一个空闲元素，类似于select中找值为-1的元素
        {
            if(g_events[i].status ==0)
                break;
        }
        if(i == MAX_EVENTS) // 超出连接数上限
        {
            printf("%s: max connect limit[%d]\n", __func__, MAX_EVENTS);
            break;
        }
        int flag = 0;
        if((flag = fcntl(cfd, F_SETFL, O_NONBLOCK)) < 0) //将cfd也设置为非阻塞
        {
            printf("%s: fcntl nonblocking failed, %s\n", __func__, strerror(errno));
            break;
        }
        eventset(&g_events[i], cfd, recvdata, &g_events[i]); //找到合适的节点之后，将其添加到监听树中，并监听读事件
        eventadd(g_efd, EPOLLIN, &g_events[i]);
    }while(0);

    printf("new connect[%s:%d],[time:%ld],pos[%d]",inet_ntoa(cin.sin_addr), ntohs(cin.sin_port), g_events[i].last_active, i);
    return;
}

/*读取客户端发过来的数据的函数*/
void recvdata(int fd, int events, void *arg)
{
    struct myevent_s *ev = (struct myevent_s *)arg;
    int len;

    len = recv(fd, ev->buf, sizeof(ev->buf), 0);    //读取客户端发过来的数据

    eventdel(g_efd, ev);                            //将该节点从红黑树上摘除

    if (len > 0) 
    {
        ev->len = len;
        ev->buf[len] = '\0';                        //手动添加字符串结束标记
        printf("C[%d]:%s\n", fd, ev->buf);                  

        eventset(ev, fd, senddata, ev);             //设置该fd对应的回调函数为senddata    
        eventadd(g_efd, EPOLLOUT, ev);              //将fd加入红黑树g_efd中,监听其写事件    

    } 
    else if (len == 0) 
    {
        close(ev->fd);
        /* ev-g_events 地址相减得到偏移元素位置 */
        printf("[fd=%d] pos[%ld], closed\n", fd, ev-g_events);
    } 
    else 
    {
        close(ev->fd);
        printf("recv[fd=%d] error[%d]:%s\n", fd, errno, strerror(errno));
    }   
    return;
}

/*发送给客户端数据*/
void senddata(int fd, int events, void *arg)
{
    struct myevent_s *ev = (struct myevent_s *)arg;
    int len;

    len = send(fd, ev->buf, ev->len, 0);    //直接将数据回射给客户端

    eventdel(g_efd, ev);                    //从红黑树g_efd中移除

    if (len > 0) 
    {
        printf("send[fd=%d], [%d]%s\n", fd, len, ev->buf);
        eventset(ev, fd, recvdata, ev);     //将该fd的回调函数改为recvdata
        eventadd(g_efd, EPOLLIN, ev);       //重新添加到红黑树上，设为监听读事件
    }
    else 
    {
        close(ev->fd);                      //关闭链接
        printf("send[fd=%d] error %s\n", fd, strerror(errno));
    }
    return ;
}

/*创建 socket, 初始化lfd */

void initlistensocket(int efd, short port)
{
    struct sockaddr_in sin;

    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(lfd, F_SETFL, O_NONBLOCK);                //将socket设为非阻塞

    memset(&sin, 0, sizeof(sin));               //bzero(&sin, sizeof(sin))
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(port);

    bind(lfd, (struct sockaddr *)&sin, sizeof(sin));

    listen(lfd, 20);

    /* void eventset(struct myevent_s *ev, int fd, void (*call_back)(int, int, void *), void *arg);  */
    eventset(&g_events[MAX_EVENTS], lfd, acceptconn, &g_events[MAX_EVENTS]);    

    /* void eventadd(int efd, int events, struct myevent_s *ev) */
    eventadd(efd, EPOLLIN, &g_events[MAX_EVENTS]);  //将lfd添加到监听树上，监听读事件

    return;
}

int main()
{
    int port=SERV_PORT;

    g_efd = epoll_create(MAX_EVENTS + 1); //创建红黑树,返回给全局 g_efd
    if(g_efd <= 0)
            printf("create efd in %s err %s\n", __func__, strerror(errno));
    
    initlistensocket(g_efd, port); //初始化监听socket
    
    struct epoll_event events[MAX_EVENTS + 1];  //定义这个结构体数组，用来接收epoll_wait传出的满足监听事件的fd结构体
    printf("server running:port[%d]\n", port);

    int checkpos = 0;
    int i;
    while(1)
    {
    /*    long now = time(NULL);
        for(i=0; i < 100; i++, checkpos++)
        {
            if(checkpos == MAX_EVENTS);
                checkpos = 0;
            if(g_events[checkpos].status != 1)
                continue;
            long duration = now -g_events[checkpos].last_active;
            if(duration >= 60)
            {
                close(g_events[checkpos].fd);
                printf("[fd=%d] timeout\n", g_events[checkpos].fd);
                eventdel(g_efd, &g_events[checkpos]);
            }
        } */
        //调用eppoll_wait等待接入的客户端事件,epoll_wait传出的是满足监听条件的那些fd的struct epoll_event类型
        int nfd = epoll_wait(g_efd, events, MAX_EVENTS+1, 1000);
        if (nfd < 0)
        {
            printf("epoll_wait error, exit\n");
            exit(-1);
        }
        for(i = 0; i < nfd; i++)
        {
		    //evtAdd()函数中，添加到监听树中监听事件的时候将myevents_t结构体类型给了ptr指针
            //这里epoll_wait返回的时候，同样会返回对应fd的myevents_t类型的指针
            struct myevent_s *ev = (struct myevent_s *)events[i].data.ptr;
            //如果监听的是读事件，并返回的是读事件
            if((events[i].events & EPOLLIN) &&(ev->events & EPOLLIN))
            {
                ev->call_back(ev->fd, events[i].events, ev->arg);
            }
            //如果监听的是写事件，并返回的是写事件
            if((events[i].events & EPOLLOUT) && (ev->events & EPOLLOUT))
            {
                ev->call_back(ev->fd, events[i].events, ev->arg);
            }
        }
    }
    return 0;
}


