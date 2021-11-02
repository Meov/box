#include "can_trans.hpp"
#include <unistd.h>

Can_trans::Can_trans():
	_can_interface_name("can0"),
	_socket_can(-1)
{
	memset(&_frame,0,sizeof(can_frame));
//	memset(_rfilter,0,sizeof(struct can_filter));
	memset(&_ifr,0,sizeof(struct ifreq));
}

Can_trans::~Can_trans(){
	std::cout << "close _socket_can" << std::endl;
	close(_socket_can);
}

int Can_trans::init(std::string can_name){
	struct sockaddr_can addr;
	struct ifreq ifr;

	_can_interface_name = can_name;	
	_socket_can = socket(PF_CAN, SOCK_RAW, CAN_RAW);
	if(_socket_can < 0){
		perror("Create socket failedi, error");
		return 1;
	}

	/* set up can interface */
	strcpy(_ifr.ifr_name, _can_interface_name.c_str());
	printf("can port is %s\n",ifr.ifr_name);
	/* assign can device */
	ioctl(_socket_can, SIOCGIFINDEX, &_ifr);
	addr.can_family = AF_CAN;
	addr.can_ifindex = _ifr.ifr_ifindex;
	/* bind can device */
	if(bind(_socket_can, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		perror("Bind can device failed");
		close(_socket_can);
		return 2;
	}

	return 0;	
}

int Can_trans::send(CanMsg_info& can_msg){
	_frame.can_id = can_msg.stdid;
	_frame.can_dlc = 8;
	printf("%s ID=%#x data length=%d\n", _ifr.ifr_name, _frame.can_id, _frame.can_dlc);

	for (int i=0; i<8; i++)
	{
		_frame.data[i] = can_msg.data[i];
	}
	printf("Sent out\n");
	/* Sending data */
	if(write(_socket_can, &_frame, sizeof(_frame)) < 0)
	{
		perror("Send failed");
		close(_socket_can);
		return 2;
	}

	return 0;
}

int Can_trans::recv(CanMsg_info& can_msg){
	int nbytes = 0;
	/* set filter for only receiving packet with can id 0x1F */
	_rfilter[0].can_id = 0x00;
	_rfilter[0].can_mask = CAN_SFF_MASK;
	if(setsockopt(_socket_can, SOL_CAN_RAW, CAN_RAW_FILTER, &_rfilter, sizeof(_rfilter)) < 0)
	{
		perror("set receiving filter error.");
		close(_socket_can);
		return 3;
	}
	/* keep reading */
	while(1){
		nbytes = read(_socket_can, &_frame, sizeof(_frame));
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

