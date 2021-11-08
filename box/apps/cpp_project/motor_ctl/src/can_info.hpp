#ifndef CAN_MSG_
#define CAN_MSG_

class CanMsg_info {
	public:

		CanMsg_info();
		~CanMsg_info();

		unsigned short stdid;
		unsigned char data[8];
};

#endif
