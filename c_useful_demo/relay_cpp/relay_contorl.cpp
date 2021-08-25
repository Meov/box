#include <stdio.h>
#include <iostream>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

//#define DEBUG
using namespace std;

#pragma pack(1)
struct Relay_cmd{
	unsigned char cmd_head[2];
	unsigned short relay_nums;
	unsigned char relay_onoff[2];
	unsigned short relay_crc;
};
#pragma pack()

unsigned short BLEndianUshort(unsigned short value)
{
	    return ((value & 0x00FF) << 8 ) | ((value & 0xFF00) >> 8);
}

class Relay{
	private:
		char cmd_head[2] = {0x01,0x05};
		int port;
		const char *ip;
		struct sockaddr_in  servaddr;
		int sockfd;
		int relay_socket_connect(void);
		int relay_socket_cmd_send(unsigned char *cmd, int len);
		int relay_socket_cmd_generate(int num,int onoff);
		unsigned int relay_mudbus_crc_16(unsigned char *buf,int len);

		const unsigned char relay_cmd_head[2] = {0x01,0x05};
		const unsigned char relay_cmd_on[2] = {0xFF,0x00};
		const unsigned char relay_cmd_off[2] = {0x00,0x00};

		struct Relay_cmd relay_cmd;

	public:
		Relay();
		void relay_socket_init(const char *relay_ip, int relay_port);
		int relay_control(int num, int onoff);
		int relay_all_control(int onoff);

};
void Relay::relay_socket_init(const char *relay_ip, int relay_port){
	memset(&(this->servaddr), 0, sizeof(this->servaddr));
	this->servaddr.sin_family = AF_INET;
	this->ip = relay_ip;
	this->port = relay_port;
	cout << "ip: "<<this->ip<<endl;
	cout << "port: " <<this->port<<endl;

	relay_socket_connect();

}

Relay::Relay(){
	cout<<"object created" << endl;
}

int Relay::relay_socket_connect(){
	const char *str = "nihao\n";	
	if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);
		return -1;
	}

	servaddr.sin_port = htons(6666);
	inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);

	if( connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
		printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
		return -2;
	}else{	
		printf("connect sucsess!\n");
	}

	printf("sockfd: %d in %s\n",sockfd,__func__);

	return 0;
}
int Relay::relay_socket_cmd_send(unsigned char *cmd, int len){

	const char *str = "nihao";
	char t[1] = {'A'};
	char test[10] = {'1','2','3','4','5','6','7','8','9'};

	for(int i = 0; i < len; i++){
		printf("0x%02x ",cmd[i]);
	}
	printf("\n");
#if 1
	if(send(sockfd, cmd, len, 0) < 0){
		printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
		return 0;
	}
#else
	if(send(sockfd, str, strlen(str), 0) < 0){
		printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
		return 0;
	}
#endif
	close(sockfd);
	return 0;
}
int Relay::relay_socket_cmd_generate(int num,int onoff){
	
	short crc;
	memset(&(this->relay_cmd),0,sizeof(this->relay_cmd));

	/*fill in cmd header*/
	memcpy(&(relay_cmd.cmd_head),&relay_cmd_head,sizeof(relay_cmd_head));

	/*fill in relay nums*/
	relay_cmd.relay_nums = BLEndianUshort(num);
	
	/*fill in relay state*/
	if(onoff){
		memcpy(&(relay_cmd.relay_onoff),&relay_cmd_on,sizeof(relay_cmd_on));
	}else{
		memcpy(&(relay_cmd.relay_onoff),&relay_cmd_off,sizeof(relay_cmd_off));
	}
	
	/*fill in crc data*/
	crc = (short)relay_mudbus_crc_16((unsigned char *)(&relay_cmd),sizeof(relay_cmd)-sizeof(relay_cmd.relay_crc));

	relay_cmd.relay_crc = BLEndianUshort(crc); 
	
#ifdef DEBUG
	for(int i = 0; i < sizeof(relay_cmd); i++){
		printf("0x%02x ",((unsigned char *)(&relay_cmd))[i]);
	}
	printf("\n");
#endif
	relay_socket_cmd_send((unsigned char *)(&relay_cmd),sizeof(relay_cmd));
	return 0;
}

unsigned int Relay::relay_mudbus_crc_16(unsigned char *buf,int len){
	unsigned int  crc = 0xFFFF;
	for (int pos = 0; pos < len; pos++)
	{
		crc ^= (unsigned int)buf[pos];
		for (int i = 8; i != 0; i--)
		{
			if ((crc & 0x0001) != 0)
			{
				crc >>= 1;
				crc ^= 0xA001;
			}
			else 
			{
				crc >>= 1;
			}
		}
	}
	crc = ((crc & 0x00ff) << 8) | ((crc & 0xff00) >> 8);
    	return crc;	 
}

int Relay::relay_control(int num, int onoff){
	relay_socket_cmd_generate(num,onoff);
	return 0;
}
int Relay::relay_all_control(int onoff){
	
	return 0;
}


int main (int argc, char *argv[]){
	const char *ip = "127.0.0.1";
	int port = 6666; 
	unsigned char cmd_test[] = {0x01,0x05,0x00,0x00,0xff,0x00};
	Relay relay;

	cout << "test begin" <<endl;
	relay.relay_socket_init(ip,port);
	relay.relay_control(0,1);
#ifdef DEBUG
	for(int i = 0; i < 32; i++){
		relay.relay_control(i,1);
		relay.relay_control(i,0);
	}
#endif
	return 0;
}


