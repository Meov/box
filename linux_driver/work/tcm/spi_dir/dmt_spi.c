#include <stdio.h>  
#include <stdlib.h>  
#include <fcntl.h>  
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <linux/spi/spidev.h>
#include "dmt_spi.h"

void format_print(char *str, char *dat, int dat_len)
{
	int i;

	printf("%s [ data len=%d ]\r\n", str, dat_len);

	for(i = 0; i < dat_len; i++) {
		printf("%02x ", (unsigned char)dat[i]);
		if((i+1)%32 == 0)
			printf("\r\n");
	}
	printf("\r\n");

}



int main(int argc, char** argv) {

	unsigned char txbuf[1024];
	unsigned char rxbuf[68]={0};
	int mode = SPI_MODE_1;
	int speed = 12000000;
	int i,ret;

	int fd = open("/dev/tcm", O_RDWR);  
	if(fd<0)	
	{
		printf("Open Fail,path=%s,fd=%d\n","/dev/tcm",fd); 
		return 0;
	}	

	for(i=0;i<1024;i++)
	{
		txbuf[i] = i+1;
	}


/*
	ret = ioctl(fd, SPI_IOC_WR_MODE32,&mode);	
	if(ret<0)
	{
		rintf("SPI_IOC_WR_MODE32 fail,ret = %d\n",ret);
		goto __done;
	}

	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ,&speed);	
	if(ret<0)
	{
		printf("SPI_IOC_WR_MAX_SPEED_HZ fail,ret = %d\n",ret);
		goto __done;
	}
	ret = write(fd,txbuf,1024);
	if(ret<0)
	{
		printf("write fail,ret = %d\n",ret);
		goto __done;
	}
	printf("write success \n");
	sleep(1);

*/
	ret = write(fd,tcm_startup,sizeof(tcm_startup));
	printf(" sned tcm_startup  success \n");

	usleep(3000);
	ret = read(fd,rxbuf,sizeof(rxbuf));
	if(ret<0)
	{
		printf("read fail,ret = %d\n",ret);
		goto __done;
	}
	printf("read success \n");
	format_print("rxbuf",rxbuf,sizeof(rxbuf));	

__done:
	close(fd);
	

	return 0;
}

