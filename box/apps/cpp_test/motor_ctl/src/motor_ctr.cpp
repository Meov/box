#include <motor_ctr.hpp>

unsigned int CAN_Time_Out = 0;

Motor::Motor(){

}

Motor::~Motor(){

}

int Motor::CAN_RoboModule_DRV_Reset(unsigned char Group,unsigned char Number, CanMsg_info& tx_message)
{

	unsigned short can_id = 0x000;


	if((Group<=7)&&(Number<=15))
	{
		can_id |= Group<<8;
		can_id |= Number<<4;
	}
	else
	{
		return -1;
	}

	tx_message.stdid = can_id;     

	tx_message.data[0] = 0x55;
	tx_message.data[1] = 0x55;
	tx_message.data[2] = 0x55;
	tx_message.data[3] = 0x55;
	tx_message.data[4] = 0x55;
	tx_message.data[5] = 0x55;
	tx_message.data[6] = 0x55;
	tx_message.data[7] = 0x55;

	return 0;
}

/****************************************************************************************
  OpenLoop_Mode                       0x01
  Current_Mode                        0x02
  Velocity_Mode                       0x03
  Position_Mode                       0x04
  Velocity_Position_Mode              0x05
  Current_Velocity_Mode               0x06
  Current_Position_Mode               0x07
  Current_Velocity_Position_Mode      0x08
 *****************************************************************************************/
int Motor::CAN_RoboModule_DRV_Mode_Choice(unsigned char Group,unsigned char Number,unsigned char Mode,CanMsg_info& tx_message)
{

	unsigned short can_id = 0x001;

	if((Group<=7)&&(Number<=15))
	{
		can_id |= Group<<8;
		can_id |= Number<<4;
	}
	else
	{
		return -1;;
	}

	tx_message.stdid = can_id;

	tx_message.data[0] = Mode;
	tx_message.data[1] = 0x55;
	tx_message.data[2] = 0x55;
	tx_message.data[3] = 0x55;
	tx_message.data[4] = 0x55;
	tx_message.data[5] = 0x55;
	tx_message.data[6] = 0x55;
	tx_message.data[7] = 0x55;

	can_tx_success_flag = 0;
	return 0;

}

/****************************************************************************************
  开环模式下的数据指令
  Group   取值范围 0-7

  Number  取值范围 0-15，其中Number==0时，为广播发送

  temp_pwm的取值范围如下：
  0 ~ +5000，满值5000，其中temp_pwm = ±5000时，最大输出电压为电源电压

 *****************************************************************************************/
int Motor::CAN_RoboModule_DRV_OpenLoop_Mode(unsigned char Group,unsigned char Number,short Temp_PWM,CanMsg_info& tx_message)
{

	unsigned short can_id = 0x002;

	if((Group<=7)&&(Number<=15))
	{
		can_id |= Group<<8;
		can_id |= Number<<4;
	}
	else
	{
		return -1;
	}

	tx_message.stdid = can_id;      //帧ID为传入参数的CAN_ID

	if(Temp_PWM > 5000)
	{
		Temp_PWM = 5000;
	}
	else if(Temp_PWM < -5000)
	{
		Temp_PWM = -5000;
	}

	tx_message.data[0] = (unsigned char)((Temp_PWM>>8)&0xff);
	tx_message.data[1] = (unsigned char)(Temp_PWM&0xff);
	tx_message.data[2] = 0x55;
	tx_message.data[3] = 0x55;
	tx_message.data[4] = 0x55;
	tx_message.data[5] = 0x55;
	tx_message.data[6] = 0x55;
	tx_message.data[7] = 0x55;

	can_tx_success_flag = 0;

	return 0;
}


/****************************************************************************************
                                   电流模式下的数据指令
Group   取值范围 0-7

Number  取值范围 0-15，其中Number==0时，为广播发送

temp_pwm的取值范围如下：
0 ~ +5000，满值5000，其中temp_pwm = 5000时，最大输出电压为电源电压

temp_current的取值范围如下：
-32768 ~ +32767，单位mA

*****************************************************************************************/
int Motor:: CAN_RoboModule_DRV_Current_Mode(unsigned char Group,unsigned char Number,short Temp_PWM,short Temp_Current,CanMsg_info& tx_message)
{
    
	unsigned short can_id = 0x003;

    if((Group<=7)&&(Number<=15))
    {
        can_id |= Group<<8;
        can_id |= Number<<4;
    }
    else
    {
        return -1;
    }
    
    tx_message.stdid = can_id;      //帧ID为传入参数的CAN_ID

    if(Temp_PWM > 5000)
    {
        Temp_PWM = 5000;
    }
    else if(Temp_PWM < -5000)
    {
        Temp_PWM = -5000;
    }
    
    if(Temp_PWM < 0)
    {
        Temp_PWM = abs(Temp_PWM);
    }
    
    tx_message.data[0] = (unsigned char)((Temp_PWM>>8)&0xff);
    tx_message.data[1] = (unsigned char)(Temp_PWM&0xff);
    tx_message.data[2] = (unsigned char)((Temp_Current>>8)&0xff);
    tx_message.data[3] = (unsigned char)(Temp_Current&0xff);
    tx_message.data[4] = 0x55;
    tx_message.data[5] = 0x55;
    tx_message.data[6] = 0x55;
    tx_message.data[7] = 0x55;

	return 0;

}

/****************************************************************************************
                                   速度模式下的数据指令
Group   取值范围 0-7

Number  取值范围 0-15，其中Number==0时，为广播发送

temp_pwm的取值范围如下：
0 ~ +5000，满值5000，其中temp_pwm = 5000时，最大输出电压为电源电压

temp_velocity的取值范围如下：
-32768 ~ +32767，单位RPM

*****************************************************************************************/
int Motor::CAN_RoboModule_DRV_Velocity_Mode(unsigned char Group,unsigned char Number,short Temp_PWM,
				short Temp_Velocity,CanMsg_info& tx_message)
{
    
	unsigned short can_id = 0x004;
    
    
    if((Group<=7)&&(Number<=15))
    {
        can_id |= Group<<8;
        can_id |= Number<<4;
    }
    else
    {
        return -1;
    }
    
    tx_message.stdid = can_id;      //帧ID为传入参数的CAN_ID

    if(Temp_PWM > 5000)
    {
        Temp_PWM = 5000;
    }
    else if(Temp_PWM < -5000)
    {
        Temp_PWM = -5000;
    }
    
    if(Temp_PWM < 0)
    {
        Temp_PWM = abs(Temp_PWM);
    }
    
    tx_message.data[0] = (unsigned char)((Temp_PWM>>8)&0xff);
    tx_message.data[1] = (unsigned char)(Temp_PWM&0xff);
    tx_message.data[2] = (unsigned char)((Temp_Velocity>>8)&0xff);
    tx_message.data[3] = (unsigned char)(Temp_Velocity&0xff);
    tx_message.data[4] = 0x55;
    tx_message.data[5] = 0x55;
    tx_message.data[6] = 0x55;
    tx_message.data[7] = 0x55;
    
	
	return 0;

		
}

/****************************************************************************************
                                   位置模式下的数据指令
Group   取值范围 0-7

Number  取值范围 0-15，其中Number==0时，为广播发送

temp_pwm的取值范围如下：
0 ~ +5000，满值5000，其中temp_pwm = 5000时，最大输出电压为电源电压

temp_position的取值范围如下：
-2147483648~+2147483647，单位qc

*****************************************************************************************/
int CAN_RoboModule_DRV_Position_Mode(unsigned char Group,unsigned char Number,short Temp_PWM,long Temp_Position,CanMsg_info& tx_message)
{
	
    unsigned short can_id = 0x005;
    
    if((Group<=7)&&(Number<=15))
    {
        can_id |= Group<<8;
        can_id |= Number<<4;
    }
    else
    {
        return -1;
    }
    
    tx_message.stdid = can_id;      //帧ID为传入参数的CAN_ID

    if(Temp_PWM > 5000)
    {
        Temp_PWM = 5000;
    }
    else if(Temp_PWM < -5000)
    {
        Temp_PWM = -5000;
    }
    
    if(Temp_PWM < 0)
    {
        Temp_PWM = abs(Temp_PWM);
    }
    
    tx_message.data[0] = (unsigned char)((Temp_PWM>>8)&0xff);
    tx_message.data[1] = (unsigned char)(Temp_PWM&0xff);
    tx_message.data[2] = 0x55;
    tx_message.data[3] = 0x55;
    tx_message.data[4] = (unsigned char)((Temp_Position>>24)&0xff);
    tx_message.data[5] = (unsigned char)((Temp_Position>>16)&0xff);
    tx_message.data[6] = (unsigned char)((Temp_Position>>8)&0xff);
    tx_message.data[7] = (unsigned char)(Temp_Position&0xff);
    
  	return 0;
		
}

/****************************************************************************************
                                  速度位置模式下的数据指令
Group   取值范围 0-7

Number  取值范围 0-15，其中Number==0时，为广播发送

temp_pwm的取值范围如下：
0 ~ +5000，满值5000，其中temp_pwm = 5000时，最大输出电压为电源电压

temp_velocity的取值范围如下：
0 ~ +32767，单位RPM

temp_position的取值范围如下：
-2147483648~+2147483647，单位qc
*****************************************************************************************/
int Motor:: CAN_RoboModule_DRV_Velocity_Position_Mode(unsigned char Group,unsigned char Number,short Temp_PWM,short Temp_Velocity,long Temp_Position,CanMsg_info& tx_message)
{
    
	unsigned short can_id = 0x006;
    
    if((Group<=7)&&(Number<=15))
    {
        can_id |= Group<<8;
        can_id |= Number<<4;
    }
    else
    {
        return -1;
    }
    
    tx_message.stdid = can_id;      //帧ID为传入参数的CAN_ID

    if(Temp_PWM > 5000)
    {
        Temp_PWM = 5000;
    }
    else if(Temp_PWM < -5000)
    {
        Temp_PWM = -5000;
    }
    
    if(Temp_PWM < 0)
    {
        Temp_PWM = abs(Temp_PWM);
    }
    
    if(Temp_Velocity < 0)
    {
        Temp_Velocity = abs(Temp_Velocity);
    }
    
    tx_message.data[0] = (unsigned char)((Temp_PWM>>8)&0xff);
    tx_message.data[1] = (unsigned char)(Temp_PWM&0xff);
    tx_message.data[2] = (unsigned char)((Temp_Velocity>>8)&0xff);
    tx_message.data[3] = (unsigned char)(Temp_Velocity&0xff);
    tx_message.data[4] = (unsigned char)((Temp_Position>>24)&0xff);
    tx_message.data[5] = (unsigned char)((Temp_Position>>16)&0xff);
    tx_message.data[6] = (unsigned char)((Temp_Position>>8)&0xff);
    tx_message.data[7] = (unsigned char)(Temp_Position&0xff);
    
  	return 0;
		
}

/****************************************************************************************
                                  电流速度模式下的数据指令
Group   取值范围 0-7

Number  取值范围 0-15，其中Number==0时，为广播发送

temp_current的取值范围如下：
0 ~ +32767，单位mA

temp_velocity的取值范围如下：
-32768 ~ +32767，单位RPM

*****************************************************************************************/
int Motor::CAN_RoboModule_DRV_Current_Velocity_Mode(unsigned char Group,unsigned char Number,short Temp_Current,
													short Temp_Velocity,CanMsg_info& tx_message)
{
  
	unsigned short can_id = 0x007;
    
    if((Group<=7)&&(Number<=15))
    {
        can_id |= Group<<8;
        can_id |= Number<<4;
    }
    else
    {
        return -1;
    }
    
    tx_message.stdid = can_id;      //帧ID为传入参数的CAN_ID
    
    if(Temp_Current < 0)
    {
        Temp_Current = abs(Temp_Current);
    }
    
    tx_message.data[0] = (unsigned char)((Temp_Current>>8)&0xff);
    tx_message.data[1] = (unsigned char)(Temp_Current&0xff);
    tx_message.data[2] = (unsigned char)((Temp_Velocity>>8)&0xff);
    tx_message.data[3] = (unsigned char)(Temp_Velocity&0xff);
    tx_message.data[4] = 0x55;
    tx_message.data[5] = 0x55;
    tx_message.data[6] = 0x55;
    tx_message.data[7] = 0x55;
    
    return 0;
}

/****************************************************************************************
                                  电流位置模式下的数据指令
Group   取值范围 0-7

Number  取值范围 0-15，其中Number==0时，为广播发送

temp_current的取值范围如下：
0 ~ +32767，单位mA

temp_position的取值范围如下：
-2147483648~+2147483647，单位qc

*****************************************************************************************/
int Motor::CAN_RoboModule_DRV_Current_Position_Mode(unsigned char Group,unsigned char Number,short Temp_Current,long Temp_Position,CanMsg_info& tx_message)
{
   
	unsigned short can_id = 0x008;

    if((Group<=7)&&(Number<=15))
    {
        can_id |= Group<<8;
        can_id |= Number<<4;
    }
    else
    {
        return -1;
    }
    
    tx_message.stdid = can_id;      //帧ID为传入参数的CAN_ID

    
    if(Temp_Current < 0)
    {
        Temp_Current = abs(Temp_Current);
    }
    
    tx_message.data[0] = (unsigned char)((Temp_Current>>8)&0xff);
    tx_message.data[1] = (unsigned char)(Temp_Current&0xff);
    tx_message.data[2] = 0x55;
    tx_message.data[3] = 0x55;
    tx_message.data[4] = (unsigned char)((Temp_Position>>24)&0xff);
    tx_message.data[5] = (unsigned char)((Temp_Position>>16)&0xff);
    tx_message.data[6] = (unsigned char)((Temp_Position>>8)&0xff);
    tx_message.data[7] = (unsigned char)(Temp_Position&0xff);
    
	return 0;
}

/****************************************************************************************
                                  电流速度位置模式下的数据指令
Group   取值范围 0-7

Number  取值范围 0-15，其中Number==0时，为广播发送

temp_current的取值范围如下：
0 ~ +32767，单位mA

temp_velocity的取值范围如下：
0 ~ +32767，单位RPM

temp_position的取值范围如下：
-2147483648~+2147483647，单位qc

*****************************************************************************************/
int Motor::CAN_RoboModule_DRV_Current_Velocity_Position_Mode(unsigned char Group,unsigned char Number,short Temp_Current,short Temp_Velocity,
			long Temp_Position,CanMsg_info& tx_message)
{
    
	unsigned short can_id = 0x009;
    
    
    if((Group<=7)&&(Number<=15))
    {
        can_id |= Group<<8;
        can_id |= Number<<4;
    }
    else
    {
        return -1;
    }
    
    tx_message.stdid = can_id;      //帧ID为传入参数的CAN_ID
    
    if(Temp_Current < 0)
    {
        Temp_Current = abs(Temp_Current);
    }
    
    if(Temp_Velocity < 0)
    {
        Temp_Velocity = abs(Temp_Velocity);
    }
    
    tx_message.data[0] = (unsigned char)((Temp_Current>>8)&0xff);
    tx_message.data[1] = (unsigned char)(Temp_Current&0xff);
    tx_message.data[2] = (unsigned char)((Temp_Velocity>>8)&0xff);
    tx_message.data[3] = (unsigned char)(Temp_Velocity&0xff);
    tx_message.data[4] = (unsigned char)((Temp_Position>>24)&0xff);
    tx_message.data[5] = (unsigned char)((Temp_Position>>16)&0xff);
    tx_message.data[6] = (unsigned char)((Temp_Position>>8)&0xff);
    tx_message.data[7] = (unsigned char)(Temp_Position&0xff);
    
    return 0;
}

/****************************************************************************************
                                      配置指令
Temp_Time1的取值范围: 0 ~ 255，为0时候，为关闭电流速度位置反馈功能
Temp_Time2的取值范围: 0 ~ 255，为0时候，为关闭限位信号反馈功能
*****************************************************************************************/
int Motor::CAN_RoboModule_DRV_Config(unsigned char Group,unsigned char Number,unsigned char Temp_Time1,unsigned char Temp_Time2 ,CanMsg_info& tx_message)
{
   
	unsigned short can_id = 0x00A;
   
    if((Group<=7)&&(Number<=15))
    {
        can_id |= Group<<8;
        can_id |= Number<<4;
    }
    else
    {
        return -1;
    }
    
    tx_message.stdid = can_id;
    
    tx_message.data[0] = Temp_Time1;
    tx_message.data[1] = Temp_Time2;
    tx_message.data[2] = 0x55;
    tx_message.data[3] = 0x55;
    tx_message.data[4] = 0x55;
    tx_message.data[5] = 0x55;
    tx_message.data[6] = 0x55;
    tx_message.data[7] = 0x55;
    
    return 0;
   
}

/****************************************************************************************
                                      在线检测
*****************************************************************************************/
int Motor::CAN_RoboModule_DRV_Online_Check(unsigned char Group,unsigned char Number,CanMsg_info& tx_message)
{
    
	unsigned short can_id = 0x00F;
    
    if((Group<=7)&&(Number<=15))
    {
        can_id |= Group<<8;
        can_id |= Number<<4;
    }
    else
    {
        return -1;
    }
    
    tx_message.stdid = can_id;      //帧ID为传入参数的CAN_ID
    
    tx_message.data[0] = 0x55;
    tx_message.data[1] = 0x55;
    tx_message.data[2] = 0x55;
    tx_message.data[3] = 0x55;
    tx_message.data[4] = 0x55;
    tx_message.data[5] = 0x55;
    tx_message.data[6] = 0x55;
    tx_message.data[7] = 0x55;
    
    return 0;
   
}

#if 0
//short Real_Current_Value[4] = {0};
//short Real_Velocity_Value[4] = {0};
//long Real_Position_Value[4] = {0};
//char Real_Online[4] = {0};
//char Real_Ctl1_Value[4] = {0};
//char Real_Ctl2_Value[4] = {0};

//本接收数据的函数，默认为4个驱动器，都挂在0组，编号为1、2、3、4
/*************************************************************************
                          CAN1_RX0_IRQHandler
描述：CAN1的接收中断函数
*************************************************************************/
int Motor::C CAN1_RX0_IRQHandler(void)
{
    
	CanRxMsgTypeDef rx_message;
    
    if (CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET)
	{
       
        CAN_Receive(&rx_message);//获取can 数据
        if((rx_message.IDE == CAN_ID_STD)&&(rx_message.IDE == CAN_RTR_DATA)&&(rx_message.DLC == 8)) //标准帧、数据帧、数据长度为8
        {
            if(rx_message.stdid == 0x1B)
            {
                Real_Current_Value[0] = (rx_message.data[0]<<8)|(rx_message.data[1]);
                Real_Velocity_Value[0] = (rx_message.data[2]<<8)|(rx_message.data[3]);
                Real_Position_Value[0] = ((rx_message.data[4]<<24)|(rx_message.data[5]<<16)|(rx_message.data[6]<<8)|(rx_message.data[7]));
            }
            else if(rx_message.stdid == 0x2B)
            {
                Real_Current_Value[1] = (rx_message.data[0]<<8)|(rx_message.data[1]);
                Real_Velocity_Value[1] = (rx_message.data[2]<<8)|(rx_message.data[3]);
                Real_Position_Value[1] = ((rx_message.data[4]<<24)|(rx_message.data[5]<<16)|(rx_message.data[6]<<8)|(rx_message.data[7]));
            }
            else if(rx_message.stdid == 0x3B)
            {
                Real_Current_Value[2] = (rx_message.data[0]<<8)|(rx_message.data[1]);
                Real_Velocity_Value[2] = (rx_message.data[2]<<8)|(rx_message.data[3]);
                Real_Position_Value[2] = ((rx_message.data[4]<<24)|(rx_message.data[5]<<16)|(rx_message.data[6]<<8)|(rx_message.data[7]));
            }
            else if(rx_message.stdid == 0x4B)
            {
                Real_Current_Value[3] = (rx_message.data[0]<<8)|(rx_message.data[1]);
                Real_Velocity_Value[3] = (rx_message.data[2]<<8)|(rx_message.data[3]);
                Real_Position_Value[3] = ((rx_message.data[4]<<24)|(rx_message.data[5]<<16)|(rx_message.data[6]<<8)|(rx_message.data[7]));
            }
            else if(rx_message.stdid == 0x1F)
            {
                Real_Online[0] = 1;
            }
            else if(rx_message.stdid == 0x2F)
            {
                Real_Online[1] = 1;
            }
            else if(rx_message.stdid == 0x3F)
            {
                Real_Online[2] = 1;
            }
            else if(rx_message.stdid == 0x4F)
            {
                Real_Online[3] = 1;
            }
            else if(rx_message.stdid == 0x1C)
            {
                Real_Ctl1_Value[0] = rx_message.data[0];
                Real_Ctl2_Value[0] = rx_message.data[1];
            }
            else if(rx_message.stdid == 0x2C)
            {
                Real_Ctl1_Value[1] = rx_message.data[0];
                Real_Ctl2_Value[1] = rx_message.data[1];
            }
            else if(rx_message.stdid == 0x3C)
            {
                Real_Ctl1_Value[2] = rx_message.data[0];
                Real_Ctl2_Value[2] = rx_message.data[1];
            }
            else if(rx_message.stdid == 0x4C)
            {
                Real_Ctl1_Value[3] = rx_message.data[0];
                Real_Ctl2_Value[3] = rx_message.data[1];
            }
        }
                
    }
}

#endif
