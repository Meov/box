#include "can_info.hpp"
#include <string.h>

CanMsg_info::CanMsg_info():
	stdid(0)
{
	memset(data,0,sizeof(data));
}

CanMsg_info::~CanMsg_info(){

}
