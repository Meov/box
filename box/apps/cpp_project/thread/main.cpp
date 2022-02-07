#include <iostream>
#include <thread>
#include <vector>
#include <atomic>

#if 0
void hello()
{
    std::cout << "Hello world" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout << "Hello world" << std::endl;
}

int main()
{
    std::thread t(hello);
    t.join(); // 没有这句话，会Debug Error的
    return 0;
}
#endif


std::atomic<int> i(0);
void greeting() {       // 2.
    i++;
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    i--;
    std::cout << "i: "<<i<<std::endl;
    return;
}

int main() {
    std::vector<std::thread> thread_arrs;
    for(int i = 0; i < 100; i++){
        thread_arrs.push_back(std::thread(greeting));
    }
    
    for(auto iter = thread_arrs.begin(); iter != thread_arrs.end(); iter++){
        iter->join();
    }
    std::cout << "主线程执行 i: "<<i << std::endl;
    return 0;
}
