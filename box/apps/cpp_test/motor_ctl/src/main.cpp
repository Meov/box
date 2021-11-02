#include <iostream>
#include <iomanip>
#include <motor_ctr.hpp>
#include <can_info.hpp>
#include <can_trans.hpp>

void dumo_canmsg(CanMsg_info& can_message){
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

int main(){ 
	int ret = 0;

	Motor motor;
	CanMsg_info tx_message;
	ret = Can_trans::get_instance()->init("can0");
	    if (ret) {
	    return ret;
	}

	ret =  motor.CAN_RoboModule_DRV_Reset(0,1,tx_message);		
	ret = Can_trans::get_instance()->send(tx_message);

	sleep(1);

	motor.CAN_RoboModule_DRV_Mode_Choice(0,1,1,tx_message);
	ret = Can_trans::get_instance()->send(tx_message);

	sleep(1);

	motor.CAN_RoboModule_DRV_OpenLoop_Mode(0,1,5000,tx_message);
	ret = Can_trans::get_instance()->send(tx_message);

	sleep(1);
	motor.CAN_RoboModule_DRV_Config(0,1,10,10,tx_message);
	ret = Can_trans::get_instance()->send(tx_message);

	std::cout << "done" << std::endl;
	return 0;
}
