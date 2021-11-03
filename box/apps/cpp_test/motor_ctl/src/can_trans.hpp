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
#include <thread.hpp>

class Can_trans : public Singleton<Can_trans>,
				  public Thread{
	public:
		int send(const CanMsg_info& can_msg);
		int init(const std::string can_name);
		void service(void);

	private:
		friend class Singleton<Can_trans>;	 
		Can_trans();
		~Can_trans();
		int recv(CanMsg_info& can_msg);
		int open(const std::string can_name);
		static void can_recv_cb(void *data);
		void can_recv_impl(void);

		std::string _can_interface_name;
		int _socket_can_fd;
		int _epoll_can_fd;
		struct ifreq _ifr;
		struct can_frame _frame;
		struct can_filter _rfilter[1];

};
