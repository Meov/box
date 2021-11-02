#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <can_info.hpp>
#include <singleton.hpp>

class Can_trans : public Singleton<Can_trans>{

	public:
		int send(CanMsg_info& can_msg);
		int recv(CanMsg_info& can_msg);
		int init(std::string can_name);

	private:
		friend class Singleton<Can_trans>;	 
		Can_trans();
		~Can_trans();

		std::string _can_interface_name;
		int _socket_can;
		struct ifreq _ifr;
		struct can_frame _frame;
		struct can_filter _rfilter[1];

};
