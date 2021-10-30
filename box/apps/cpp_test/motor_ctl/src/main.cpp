#include <iostream>
#include <iomanip>
#include <motor_ctr.hpp>
#include <can_info.hpp>

int main(){ 
	int ret = 0;

	Motor motor;
	CanMsg_info tx_message;

	ret =  motor.CAN_RoboModule_DRV_Reset(0,1,tx_message);		
	
	if(ret){
		std::cout << "error" << std::endl;
	}
	else{
	
		std::cout << "ok" << std::endl;

	}

	std::cout << std::setfill('0')
		<<std::setw(3)
		<< std::uppercase
		<< std::hex<< tx_message.StdId << " ";

	for(int i = 0; i < 8;i++){
		std::cout << std::setfill('0') 
			<< std::setw(2) 
			<< std::uppercase 
			<< std::hex << (0xFF & tx_message.Data[i]) << " ";		    
	}


	std::cout << "done" << std::endl;
	return 0;
}
