#ifndef SINGLETON_H
#define SINGLETON_H

template <typename T>
class Singleton {
public:
    static T *get_instance() {
        if (_s_instance == nullptr) {
            _s_instance = new T();
        }

        return _s_instance;
    }

    static void release() {
        delete _s_instance;
        _s_instance = nullptr;
    }
protected:
    Singleton() { }
    virtual ~Singleton() { }

private:
    Singleton(const Singleton &) { }
    Singleton &operator=(const Singleton &) { }

    static T *_s_instance;
};

template <typename T>
T *Singleton<T>::_s_instance = nullptr;

#endif