#include <iostream>
#include <iomanip>
#include <motor_ctr.hpp>
#include <can_info.hpp>
#include <can_trans.hpp>
#include <csignal>
#include <unistd.h>
#include <epoll.hpp>

#define THREAD_RECV

class Manager {
public:
    Manager();
    ~Manager();
    int  init();
    int  run();
    void wait();
    static void stop(int signum);
    void uninit();
};

Manager::Manager() {
}

Manager::~Manager() {
    uninit();
}

void dump_canmsg(CanMsg_info& can_message){
    std::cout << std::setfill('0')
        <<std::setw(3)
        << std::uppercase
        << std::hex<< can_message.stdid << " ";

    for(int i = 0; i < 8;i++){
        std::cout << std::setfill('0') 
            << std::setw(2) 
            << std::uppercase 
            << std::hex << (0xFF & can_message.data[i]) << " ";         
    }
}

int Manager::init(){

    int ret = Epoll::get_instance()->init();
    if (ret) {
        return ret;
    }

    ret = Can_trans::get_instance()->init("can0");
    if (ret) {
        return ret;
    }

    return 0;
}

int Manager::run(){
#ifdef THREAD_RECV
    Can_trans::get_instance()->start();
#else
    Epoll::get_instance()->start();
#endif
    return 0;
}

void Manager::wait() {
    /*exit when major thread quit!*/
    Can_trans::get_instance()->wait();
    Epoll::get_instance()->wait();
}

void Manager::stop(int signum) {
    std::cout << "Interrupt signal:" << signum << " received\n" << std::endl;
    Motor motor;
    CanMsg_info tx_message;
    motor.CAN_RoboModule_DRV_OpenLoop_Mode(0,1,0,tx_message);
    Can_trans::get_instance()->send(tx_message);

    Can_trans::get_instance()->stop();
    Epoll::get_instance()->stop();
}

void Manager::uninit() {
    std::cout << "manager uninit " <<std::endl;
    Can_trans::release();
    Epoll::release();
}

int driver_init(int mode){
    int ret = 0;    
    CanMsg_info tx_message;
    Motor motor;

    motor.CAN_RoboModule_DRV_Reset(0,1,tx_message);
    ret = Can_trans::get_instance()->send(tx_message);
    if(ret){
        std::cout << "driver init failed" << std::endl;
        return ret;
    }
    sleep(1);

    motor.CAN_RoboModule_DRV_Mode_Choice(0,1,mode,tx_message);
    ret = Can_trans::get_instance()->send(tx_message);
    if(ret){
        std::cout << "driver mode set failed" << std::endl;
        return ret;
    }
    sleep(1);
 
    return 0;
}

int open_loop_test(void){
    int ret = 0;
    Motor motor;
    CanMsg_info tx_message;

    if(driver_init(OpenLoop_Mode)){
        std::cout << "driver init failed" << std::endl;
        return ret;
    }

    motor.CAN_RoboModule_DRV_OpenLoop_Mode(0,1,5000,tx_message);
    ret = Can_trans::get_instance()->send(tx_message);
    sleep(1);

    motor.CAN_RoboModule_DRV_Config(0,1,20,20,tx_message);
    ret = Can_trans::get_instance()->send(tx_message);

    std::cout << "open loop done" << std::endl;

    return 0;
}

int velocity_postion_test(void){
    int ret = 0;
    Motor motor;
    CanMsg_info tx_message;

    if(driver_init(Velocity_Position_Mode)){
        std::cout << "driver init failed" << std::endl;
        return ret;
    }

    motor.CAN_RoboModule_DRV_Velocity_Position_Mode(0,1,5000,10000,65536,tx_message);
    ret = Can_trans::get_instance()->send(tx_message);
    sleep(1);

    motor.CAN_RoboModule_DRV_Config(0,1,20,20,tx_message);
    ret = Can_trans::get_instance()->send(tx_message);

    std::cout << "velocity postion done" << std::endl;

    return 0;
}

int main(int argc, char* argv[]){
    int opt = -1;

    while ((opt = getopt(argc, argv, "d")) != -1) {
        switch (opt) {
            case 'd':
                if (daemon(0, 0) < 0) {
                    std::cout << "start daemon err" <<std::endl;
                }
                break;
            default:
                break;
        }
    }


    Manager manager;
    int ret = manager.init();
    if(ret){
        return ret;
    }

    ret =  manager.run();
    if(ret){
        return ret;
    }

    signal(SIGINT, Manager::stop);
    
    open_loop_test();
    velocity_postion_test();

    manager.wait();
    return 0;
}

