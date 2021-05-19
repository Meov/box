#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <string.h>

#if 0
static char tcm_tx_buffer[18] = {0x3A,0x00,0x0E,0x34,0x00,0xC1,0x00,0x00,0x00,0x0C,0x00,0x00,0x80,0x99,0x00,0x01,0xB8,0x97};

static char tcm_pcr_extend[52] = {0x3a, 0x00, 0x30, 0x0a, 0x00, 0xc1, 0x00, 0x00, 0x00, 0x2e, 0x00, 0x00, 0x80, 0x14, 0x00, 0x00, 0x00, 0x01, 0x12, 0x43, 0x56, 0x84, 0x78, 0x91, 0x27, 0x94, 0x77, 0x59, 0x88, 0xab, 0xcd, 0xeb, 0xac, 0xdb, 0xac, 0xeb, 0xad, 0xe1, 0x23, 0xa7, 0x89, 0xb9, 0x81, 0xb4, 0x1c, 0xd3, 0xe1, 0x82, 0xad, 0xc1, 0x78, 0x9b};

char *str = "hello uart assistant!";
#endif

int uart_init(const char *uart_name){
	int fd;	

	if((fd = open(uart_name, O_RDWR|O_NONBLOCK))<0){//非阻塞读方式
                //printf("open %s failed",uart0);
                perror("Error: open uart0 failed");
		return -1;	
	}else{
                printf("open %s successfully!\n",uart_name);
		return fd;
	}

}

int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop)
{
	struct termios newtio,oldtio;
	if  ( tcgetattr( fd,&oldtio)  !=  0) { 
		perror("SetupSerial 1");
		return -1;
	}
	bzero( &newtio, sizeof( newtio ) );
	newtio.c_cflag  |=  CLOCAL | CREAD;
	newtio.c_cflag &= ~CSIZE;
 
	switch( nBits )
	{
		case 7:
			newtio.c_cflag |= CS7;
			break;
		case 8:
			newtio.c_cflag |= CS8;
			break;
	}
 
	switch( nEvent )
	{
	case 'O':
		newtio.c_cflag |= PARENB;
		newtio.c_cflag |= PARODD;
		newtio.c_iflag |= (INPCK | ISTRIP);
		break;
	case 'E': 
		newtio.c_iflag |= (INPCK | ISTRIP);
		newtio.c_cflag |= PARENB;
		newtio.c_cflag &= ~PARODD;
		break;
	case 'N':  
		newtio.c_cflag &= ~PARENB;
		break;
	}
 
	switch( nSpeed )
	{
		case 2400:
			cfsetispeed(&newtio, B2400);
			cfsetospeed(&newtio, B2400);
			break;
		case 4800:
			cfsetispeed(&newtio, B4800);
			cfsetospeed(&newtio, B4800);
			break;
		case 9600:
			cfsetispeed(&newtio, B9600);
			cfsetospeed(&newtio, B9600);
			break;
		case 115200:
			cfsetispeed(&newtio, B115200);
			cfsetospeed(&newtio, B115200);
			break;
		case 460800:
			cfsetispeed(&newtio, B460800);
			cfsetospeed(&newtio, B460800);
			break;
		default:
			cfsetispeed(&newtio, B9600);
			cfsetospeed(&newtio, B9600);
			break;
	}
	if( nStop == 1 )
		newtio.c_cflag &=  ~CSTOPB;
	else if ( nStop == 2 )
		newtio.c_cflag |=  CSTOPB;
		newtio.c_cc[VTIME]  = 100;///* 设置超时10 seconds*/
		newtio.c_cc[VMIN] = 0;
		tcflush(fd,TCIFLUSH);
	if((tcsetattr(fd,TCSANOW,&newtio))!=0)
	{
		perror("com set error");
		return -1;
	}
	
	//	printf("set done!\n\r");
	return 0;
}

#if 0
int send_recive(int dev, char *tx, char *rx, int tx_len){
	int nByte = 0;
	int i = 0;
	int ret;
	char rx_buffer[512];
	memset(rx_buffer, 0, sizeof(rx_buffer));
	
	if((tx == NULL)&&(rx == NULL)){
		return -1;
	}	

	nByte = write(dev,tx,tx_len);
	if(nByte < 0){
		printf("send uart0 err\n");
		return -1;
	}

	usleep(10000);
	nByte = read(dev, rx_buffer, 512);
	if(nByte < 0){
		printf("Error: read uart0 err\n");
		return -1;
	}
#if 0
	printf("read: \n");
	for(i = 0; i<nByte; i++){
		printf("0x%02x ",rx_buffer[i]);
	}
	printf("\n");
#endif
	memcpy(rx, rx_buffer, nByte);
	return nByte;
}
#endif

int close_uart(int fd){
	
	close(fd);
}
#if 0 
int main(int argc, char argv[])
{
	int fd,nByte,flag=1;
	int i = 0;
	int ret;
	char buffer[512];
	char *uart_out = "Please input,waiting....\r\n";
	char *uart_demo = "Linux uart demo\r\n";
	fd = uart_init(uart0);
	

	if(fd < 0){
		return -1;
	}

	if(set_opt(fd, 115200, 8, 'N', 1) < 0){
		return -1;
	}

	printf("sending data:\n");
	usleep(100000);
	memset(buffer, 0, sizeof(buffer));
	
	send_recive(fd,tcm_tx_buffer,buffer,sizeof(tcm_tx_buffer));
	send_recive(fd,tcm_pcr_extend,buffer,sizeof(tcm_pcr_extend));
#if 0
	while(1){
#if 0
		ret = write(fd,tcm_tx_buffer,sizeof(tcm_tx_buffer));
		if(ret < 0)
		{	 
            		printf("Write() error:%s\n",strerror(errno)) ;
            		goto cleanup ;
        	}
		sleep(3);
#else
		while((nByte = read(fd, buffer, 512))>0){
			buffer[nByte+1] = '\0';			

			for(i = 0; i<nByte; i++){
				printf("0x%02x ",buffer[i]);
			}
			printf("\n");

			write(fd,tcm_tx_buffer,sizeof(tcm_tx_buffer));
			memset(buffer, 0, strlen(buffer));
			nByte = 0;
		}
#endif
	}

#endif
cleanup:
	close(fd);	
}
 
#endif
