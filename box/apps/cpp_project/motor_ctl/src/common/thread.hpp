#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>

class Thread {
public:
    Thread();
    virtual ~Thread();

    virtual bool start();
    virtual void wait();
    virtual void stop();
    virtual bool is_alive();

protected:
    static   void *proc(void *data);
    virtual  void service() = 0;
    volatile bool _exit;

private:
    void init_threadid();
    pthread_t _threadid;
};

// template<typename X>
// class Thread {
//     static void* proc(void* data) {
//         (static_cast<X*>data)->service();
//         return nullptr;
//     }
// }

// class ABCThread :
//     public Thread<ABCThread>,
//     public Singleton<ABCThread> {
//     void service() {
//         dosomething;
//     }
// }
// ABCThread::get_instance()->start();

#endif