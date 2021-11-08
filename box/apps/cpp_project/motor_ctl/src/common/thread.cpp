#include <assert.h>
#include <stdio.h>
#include "thread.hpp"

Thread::Thread() :
    _exit(false) {
    init_threadid();
}

Thread::~Thread() {
    stop(); 
}

bool Thread::start() {
    assert(!is_alive());

    int pthread_err = pthread_create(&_threadid, nullptr, Thread::proc, this);
    if (pthread_err != 0) {
        printf("thread packet_engine create fail, err num is %d\n", pthread_err);
        return false;
    }

    return true;
}

void Thread::wait() {
    if (is_alive()) {
        pthread_join(_threadid, nullptr);
        init_threadid();
    }
}

void Thread::stop() {
    _exit = true;
}

bool Thread::is_alive() {
    return _threadid != 0;
}

void *Thread::proc(void *data) {
    static_cast<Thread*>(data)->service();
    return nullptr;
}

void Thread::init_threadid() {
    _threadid = 0;
}
