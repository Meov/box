#ifndef __MOTORCONTROL_H
#define __MOTORCONTROL_H
#include <can_info.hpp>


#define OpenLoop_Mode                       0x01
#define Current_Mode                        0x02
#define Velocity_Mode                       0x03
#define Position_Mode                       0x04
#define Velocity_Position_Mode              0x05
#define Current_Velocity_Mode               0x06
#define Current_Position_Mode               0x07
#define Current_Velocity_Position_Mode      0x08
#define abs(x) ((x)>0? (x):(-(x)))


class Motor{

	public:
		Motor();
		~Motor();
		int CAN_RoboModule_DRV_Reset(unsigned char Group,unsigned char Number,CanMsg_info& tx_message);
		int CAN_RoboModule_DRV_Mode_Choice(unsigned char Group,unsigned char Number,unsigned char Mode,CanMsg_info& tx_message);
		int CAN_RoboModule_DRV_OpenLoop_Mode(unsigned char Group,unsigned char Number,short Temp_PWM,CanMsg_info& tx_message);
		int CAN_RoboModule_DRV_Current_Mode(unsigned char Group,unsigned char Number,short Temp_PWM,short Temp_Current,CanMsg_info& tx_message);
		int CAN_RoboModule_DRV_Velocity_Mode(unsigned char Group,unsigned char Number,short Temp_PWM,short Temp_Velocity,CanMsg_info& tx_message);

		int CAN_RoboModule_DRV_Position_Mode(unsigned char Group,unsigned char Number,short Temp_PWM,long Temp_Position,CanMsg_info& tx_message);
		int CAN_RoboModule_DRV_Velocity_Position_Mode(unsigned char Group,unsigned char Number,short Temp_PWM,
				short Temp_Velocity,long Temp_Position,CanMsg_info& tx_message);
		int CAN_RoboModule_DRV_Current_Velocity_Mode(unsigned char Group,unsigned char Number,short Temp_Current,
				short Temp_Velocity,CanMsg_info& tx_message);
		int CAN_RoboModule_DRV_Current_Position_Mode(unsigned char Group,unsigned char Number,
				short Temp_Current,long Temp_Position,CanMsg_info& tx_message);
		int CAN_RoboModule_DRV_Current_Velocity_Position_Mode(unsigned char Group,unsigned 
				char Number,short Temp_Current,short Temp_Velocity,long Temp_Position,CanMsg_info& tx_message);
		int CAN_RoboModule_DRV_Config(unsigned char Group,unsigned char Number,unsigned char Temp_Time,
				unsigned char Ctl1_Ctl2,CanMsg_info& tx_message);
		int CAN_RoboModule_DRV_Online_Check(unsigned char Group,unsigned char Number,CanMsg_info& tx_message);
		CanMsg_info get_can_txmsg(void);

		unsigned char can_tx_success_flag = 0;

	private:

};

#endif
