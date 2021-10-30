#include "can_info.hpp"
#include <string.h>

CanMsg_info::CanMsg_info():
	StdId(0)

{
	memset(Data,0,sizeof(Data));
}

CanMsg_info::~CanMsg_info(){

}
