#include <stdio.h>
#include <unistd.h> 
#include <sys/epoll.h>
#include <assert.h>
#include "epoll.hpp"
#include <iostream>

Epoll::Epoll() : _epoll_fd(-1) {
}

Epoll::~Epoll() {
    uninit();
}

int Epoll::init() {
    assert(_epoll_fd == -1);
    _epoll_fd = epoll_create1(0);
    if (_epoll_fd == -1) {
        perror("epoll create fail\n");
        return 1;
    }

    return 0;
}

void Epoll::uninit() {
    if (_epoll_fd != -1) {
        std::map<int, CallBackInfo>::iterator it;
        for (it = _callback_map.begin(); it != _callback_map.end();) {
            epoll_delete(it->first);
            _callback_map.erase(it++);
        }
        _callback_map.clear();

        close(_epoll_fd);
        _epoll_fd = -1;
    }
}

void Epoll::service() {
    assert(_epoll_fd != -1);

    epoll_event event[MAX_EVENT_NUM];
    std::map<int, CallBackInfo>::const_iterator it;
    std::cout << "epoll service begin" << std::endl;
    while(!_exit) {
        int num = epoll_wait(_epoll_fd, event, MAX_EVENT_NUM, 3*1000);
        for (int i = 0; i < num; ++i) {
            it = _callback_map.find(event[i].data.fd);
            if ((it != _callback_map.end())
             && (it->second.callback != nullptr)
             && (it->second.object   != nullptr)) {
                it->second.callback(it->second.object);
            }
        }
    }
    std::cout << "epoll service end" << std::endl;
}

int Epoll::epoll_add(int fd, void (*cb)(void *), void *data) {
    assert(_epoll_fd != -1);

    epoll_event event;
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = fd;
    int result = epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, fd, &event); 
    if (result == -1) {
        printf("Epoll add object %d fail\n", fd);
        return 1;
    }

    CallBackInfo callback_info;
    callback_info.callback = cb;
    callback_info.object   = data;
    _callback_map[fd] = callback_info;

    return 0;
}

void Epoll::epoll_delete(int fd) {
    assert(_epoll_fd != -1);

    epoll_event event;  
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = fd;

    epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, fd, &event);

}
