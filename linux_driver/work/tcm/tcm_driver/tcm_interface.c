#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tcm_interface.h"
#include "../crc/crc.h"
#include "../serial_dir/dmd_uart.h"
//#define TCM_DEBUG

uint32 Reverse32(uint32 x)
{
    return ((x&0x000000ff)<<24)|((x&0x0000ff00)<<8)|((x&0x00ff0000)>>8)|((x&0xff000000)>>24);
}

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
                printf("send TCM data err\n");
                return -1;
        }

        usleep(10000);
        nByte = read(dev, rx_buffer, 512);
        if(nByte < 0){
                printf("Error: read TCM data err\n");
                return -1;
        }
        memcpy(rx, rx_buffer, nByte);
        return nByte;
}

static int tcm_transmit_cmd_origin(struct tcm_dev *dev, void *cmd,int cmd_length/*, unsigned int flags, const char *desc*/){
	int i = 0;
	struct dmt_cmd *d_cmd;
	u8 *tx;
	u8 rx[512];
	int tx_len,rx_len;

	d_cmd = (struct dmt_cmd *)cmd;
	tx_len = HEADER_SIZE_BYTES+d_cmd->cmd_h.cmd_length;
	tx = (u8 *)malloc(tx_len);	
	tx[0] = d_cmd->cmd_h.rx_tx_flag;
	tx[1] = d_cmd->cmd_h.cmd_length>>8;
	tx[2] = d_cmd->cmd_h.cmd_length;
	tx[3] = d_cmd->cmd_h.xor_result;
	memcpy(&(tx[4]),d_cmd->tcm_data_buf,d_cmd->cmd_h.cmd_length-2);
	tx[4+d_cmd->cmd_h.cmd_length-2] = d_cmd->crc_result >> 8;
	tx[4+d_cmd->cmd_h.cmd_length-1] = d_cmd->crc_result;


	rx_len = send_recive(dev->fd,tx,rx,tx_len);
	if(rx_len < 0){
		printf("tcm transmit cmd failed! err: %d\n",rx_len);
	}

	for(i = 0; i < rx_len; i++){
               	 if(i % 16 == 0) printf("\n");
		 printf("0x%02x ",*(rx+i));
        }
	printf("\n");

	free(tx);
	tx = NULL;
	return 0;
}


static int unpacking_tcm_data(void *cmd, u8* tx_buf, uint16 cmd_len){
	u8 *dmd_tx = tx_buf;
	u8 *tcm_buf = (u8 *)cmd;
	int i = 0;
	uint16 offset = 0;
	uint16 tcm_cmd_data_len = cmd_len - HEADER_SIZE_BYTES - CRC_LENTH;
	/*
        cmd : [cmd_header(4byts) crc_value[2bytes]  tcm_cmd(nbytes)]
        */
        /*1. get cmd header*/
        memcpy(dmd_tx,tcm_buf,HEADER_SIZE_BYTES);

        /*2. get standard tcm cmd*/
        offset = HEADER_SIZE_BYTES;
        memcpy(&dmd_tx[offset],&tcm_buf[offset+CRC_LENTH],tcm_cmd_data_len);

        /*3. get crc result*/
        offset = HEADER_SIZE_BYTES+tcm_cmd_data_len;
        memcpy(&dmd_tx[offset],&tcm_buf[HEADER_SIZE_BYTES],CRC_LENTH);
	
#ifdef TCM_DEBUG
	printf("\ntx:");
	for( i = 0; i < cmd_len; i++){
		if(i % 16 == 0) printf("\n");
		printf("0x%02x ",dmd_tx[i]);
	}
#endif
	return 0;
}

static int packing_tcm_data(void *cmd, u8* rx_buf){
	u8 *dmd_rx = rx_buf;
        u8 *tcm_buf = (u8 *)cmd;
	uint16 offset = 0;
        int ret = 0;
	int i = 0;
	struct cmd_header heder;
        struct tcm_cmd_common cmd_common;
	int actual_recv_data_len = 0;

	/* dmd tcm chip use 0x3B for recieve data */	
	memcpy(&heder,dmd_rx,sizeof(struct cmd_header));
        if(heder.rx_tx_flag != RECV_CMD){
                ret = -2;
		return ret;
        }
	/*
		WARING: UART recive lenth is differnt form SPI method!
		The actual data length is stored in heder.cmd_length according dmt chip mannual
	*/
	actual_recv_data_len = Reverse16(heder.cmd_length)+4;

        memcpy(&cmd_common,&dmd_rx[HEADER_SIZE_BYTES],TCM_CMD_COMMON_LENTH);
	/*packing cmd struct*/
        /*1. fill in cmd header */
        memcpy(tcm_buf,&heder,HEADER_SIZE_BYTES);
        /*2. fill in  CRC result */
        memcpy(&tcm_buf[HEADER_SIZE_BYTES],&dmd_rx[actual_recv_data_len-2],CRC_LENTH); //last 2 bytes are crc result
        /*3. fill in TCM commom header */
        memcpy(&tcm_buf[HEADER_SIZE_BYTES+CRC_LENTH],&cmd_common,TCM_CMD_COMMON_LENTH);
        /*4. fill in TCM data */
        offset = HEADER_SIZE_BYTES + CRC_LENTH + TCM_CMD_COMMON_LENTH;
        memcpy(&tcm_buf[offset],&dmd_rx[HEADER_SIZE_BYTES + TCM_CMD_COMMON_LENTH],actual_recv_data_len-offset);

#ifdef TCM_DEBUG
	printf("\nactual_recv_data_len :%d rx:",actual_recv_data_len);
	for(i = 0; i < actual_recv_data_len; i++){
		if(i % 16 == 0) printf("\n");
		printf("0x%02x ",*(dmd_rx+i));
	}
	printf("\n");

	printf("offs addr of buf_offset: %p, offset :%d\n",&(tcm_buf[offset]),offset);
	printf("TCM data:");
	for(i = 0; i < actual_recv_data_len - offset; i++){
		if(i % 16 == 0) printf("\n");
		printf("0x%02x ",tcm_buf[i+offset]);
	}
	printf("\n");
#endif
	return ret;
}

uint32 tcm_transmit_cmd(struct tcm_dev *dev,void *cmd, uint16 len,const char *desc){
	int i = 0;
        u8 *tx;
        u8 rx[512];
	int tx_len,rx_len;
	u8 *buf = (u8 *)cmd;
	int ret = 0;

	tx_len = len;  
	tx = (char *)malloc(len);
#ifdef TCM_DEBUG
	printf("\n------------TCM %s transmit on going...",desc);
#endif	
	ret = unpacking_tcm_data(buf,tx,len);
	/*ready to send data*/
	rx_len = send_recive(dev->fd,tx,rx,tx_len);
	if(rx_len < 0){
		printf("TCM err occur  %s :\n",desc);
		ret = -1;
		goto out;
	}
	/*ready to packing data as dmd cmd struct data*/
	ret = packing_tcm_data(cmd,rx);
	if(ret < 0){
		goto out;
	}
out:	
	free(tx);
	tx = NULL;
	return ret;
}

int tcm_cmmmond_xfer(struct tcm_dev *dev, const u8 * tcm_cmd, int cmd_lenth, u8* rx){
	width_t crc_result;
	struct dmt_cmd dmd_tcm_cmd;
	memset(&dmd_tcm_cmd,0,sizeof(dmd_tcm_cmd));
	dmd_tcm_cmd.cmd_h.rx_tx_flag = SEND_CMD;
	dmd_tcm_cmd.cmd_h.cmd_length = cmd_lenth + 2;
	dmd_tcm_cmd.cmd_h.xor_result = dmd_tcm_cmd.cmd_h.rx_tx_flag ^ dmd_tcm_cmd.cmd_h.cmd_length;
	dmd_tcm_cmd.tcm_data_buf = tcm_cmd;
	dmd_tcm_cmd.crc_result = crcCompute_dmt(tcm_cmd,cmd_lenth);
	tcm_transmit_cmd_origin(dev, &dmd_tcm_cmd, cmd_lenth);

	return 0;
}

int tcm_dev_init(struct tcm_dev *dev){
     	int fd;
	/* Judge using spi or uart to communacate with TCM
	   if we use UART we should set its baurd rate
	*/
	if(strcmp(dev->dev_name,"/dev/tcm")){
		if((fd = open(dev->dev_name, O_RDWR|O_NONBLOCK))<0){//非阻塞读方式
			printf("open %s failed",dev->dev_name);
			return -1;
		}else{
			printf("open %s successfully!\n",dev->dev_name);
			return fd;
		}


		if(set_opt(fd, 115200, 8, 'N', 1) < 0){
			close(fd);
			return -1;
		}
	}
	else{
		fd = open(dev->dev_name, O_RDWR);
		if(fd<0)
		{
			printf("Open Fail,path=%s,fd=%d\n",dev->dev_name,fd);
			return -1;
		}

	}
	dev->fd = fd;
	printf("open %s success fd: %d!\n",dev->dev_name,dev->fd);
	return 0;
}

int tcm_dev_release(struct tcm_dev *dev){
	close(dev->fd);			
	free(dev);
	dev = NULL;
	return 0;
}

void show_help(void){
	printf("Usage:\n");
	printf("        h :       help\n");
	printf("        S :       tcm startup\n");
	printf("        r :       read pcr vaule\n");
	printf("        e :       extend pcr value\n");
	printf("        R :       reset the pcr\n");
	printf("        t :       tcm self test full\n");
	printf("        q :       quit\n");
}


