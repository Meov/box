#include "can_trans.hpp"
#include <unistd.h>
#include <cassert>
#include <epoll.hpp>

Can_trans::Can_trans():
    _can_interface_name("can0"),
    _socket_can_fd(-1),
    _epoll_can_fd(-1)
{
    memset(&_frame,0,sizeof(can_frame));
    memset(&_ifr,0,sizeof(ifreq));
}

Can_trans::~Can_trans(){
    close(_socket_can_fd);
}

int Can_trans::open(std::string can_name){
    struct sockaddr_can addr;

    _can_interface_name = can_name;
    _socket_can_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if(_socket_can_fd < 0){
        std::cout << "Create socket failedi, error" << std::endl;
        return 1;
    }

    /* set up can interface */
    strcpy(_ifr.ifr_name, _can_interface_name.c_str());
    std::cout << "using can port: " << _ifr.ifr_name << std::endl;
    /* assign can device */
    ioctl(_socket_can_fd, SIOCGIFINDEX, &_ifr);
    addr.can_family = AF_CAN;
    addr.can_ifindex = _ifr.ifr_ifindex;
    /* bind can device */
    if(bind(_socket_can_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        std::cout << "Bind can device failed" << std::endl;;
        close(_socket_can_fd);
        return 2;
    }

    /* set filter for only receiving packet with can id 0x1F */
    _rfilter[0].can_id = 0x00;
    _rfilter[0].can_mask = CAN_SFF_MASK;
    if(setsockopt(_socket_can_fd, SOL_CAN_RAW, CAN_RAW_FILTER, &_rfilter, sizeof(_rfilter)) < 0)
    {
        std::cout << "set receiving filter error." << std::endl;
        close(_socket_can_fd);
        return 3;
    }

    return 0;
}

int Can_trans::init(const std::string can_name){
    /*bind can socket, 0 successed, ohters failed*/
    int ret = open(can_name);
    if(ret){
        std::cout << "bind can socket failed!" << std::endl;
        return ret;
    }
    std::cout << can_name << " open success!" << "ret: "<< ret <<std::endl;

#ifndef THREAD_RECV
    /*add can recv to epoll*/
    Epoll::get_instance()->epoll_add(_socket_can_fd, can_recv_cb, this);
#endif
    return 0;
}

void Can_trans::can_recv_cb(void *data) {
    assert(data != nullptr);
    return static_cast<Can_trans *>(data)->can_recv_impl();
}

void Can_trans::can_recv_impl(){
    int nbytes = 0;
    CanMsg_info can_msg;
    std::cout << "epoll recived something"  << std::endl;
    nbytes = read(_socket_can_fd, &_frame, sizeof(_frame));
    if(nbytes > 0)
    {
        for (int i=0; i < _frame.can_dlc; i++){
            can_msg.data[i] = _frame.data[i];
            printf("%#x ", _frame.data[i]);
        }
        printf("\n");
    }
    std::cout << "epoll recived end"  << std::endl;
}

int Can_trans::send(const CanMsg_info& can_msg){
    _frame.can_id = can_msg.stdid;
    _frame.can_dlc = 8;
    printf("%s ID=%#x data length=%d\n", _ifr.ifr_name, _frame.can_id, _frame.can_dlc);

    for (int i=0; i<8; i++)
    {
        _frame.data[i] = can_msg.data[i];
    }
    printf("Sent out\n");
    /* Sending data */
    if(write(_socket_can_fd, &_frame, sizeof(_frame)) < 0)
    {
        perror("Send failed");
        close(_socket_can_fd);
        return 2;
    }

    return 0;
}

int Can_trans::recv(CanMsg_info& can_msg){
    int nbytes = 0;

    /* set filter for only receiving packet with can id 0x1F */
    _rfilter[0].can_id = 0x00;
    _rfilter[0].can_mask = CAN_SFF_MASK;

    if(setsockopt(_socket_can_fd, SOL_CAN_RAW, CAN_RAW_FILTER, &_rfilter, sizeof(_rfilter)) < 0)
    {
        std::cout << "set receiving filter error." << std::endl;
        close(_socket_can_fd);
        return 3;
    }
    /* keep reading */
    while(!_exit){
        std::cout << "ready to recive data..." << std::endl;
        sleep(1);

        nbytes = read(_socket_can_fd, &_frame, sizeof(_frame));
        if(nbytes > 0)
        {
            for (int i=0; i < _frame.can_dlc; i++){
                can_msg.data[i] = _frame.data[i];
                printf("%#x ", _frame.data[i]);
            }
            printf("\n");
        }

    }

    return 0;
}

void Can_trans::service(void){
    CanMsg_info can_msg;
    std::cout << "ready to recive data using muilti thread..." << std::endl;
    recv(can_msg);
}
