#ifndef EPOLL_H
#define EPOLL_H

#include <map>

#include "singleton.hpp"
#include "thread.hpp"

class Epoll :
    public Thread,
    public Singleton<Epoll> {
private:
    friend class Singleton<Epoll>;
    Epoll();
    ~Epoll();

public:
    int  init();
    void uninit();
    void service();
    int  epoll_add(int fd, void (*cb)(void *), void *data);
    void epoll_delete(int fd);

private:
    enum EpollParam {
        MAX_EVENT_NUM = 20,
    };

    struct CallBackInfo {
        void (*callback)(void *objcet);
        void *object;
    };

    int _epoll_fd;
    std::map<int, CallBackInfo> _callback_map; 
};

#endif
