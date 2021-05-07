#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tcm_cmd.h"
#include "../crc/crc.h"
#include "../serial_dir/dmd_uart.h"


#define HEADER_SIZE_BYTES 4
typedef unsigned char u8;
static const char *uart0 = "/dev/ttyS0";
static int dev;

static int tcm_transmit_cmd(const void *cmd,int len/*, unsigned int flags, const char *desc*/){
	int i = 0;
	struct dmt_cmd *d_cmd;
	u8 *tx;
	u8 rx[512];
	int tx_len,rx_len;
	
	d_cmd = (struct dmt_cmd *)cmd;
	tx_len = HEADER_SIZE_BYTES+d_cmd->cmd_h.cmd_length;
	tx = (u8 *)malloc(tx_len);	
	tx[0] = d_cmd->cmd_h.rx_tx_flag.send_flag;
	tx[1] = d_cmd->cmd_h.cmd_length>>8;
	tx[2] = d_cmd->cmd_h.cmd_length;
	tx[3] = d_cmd->cmd_h.xor_result;
	memcpy(&(tx[4]),d_cmd->tcm_data_buf,d_cmd->cmd_h.cmd_length-2);
	tx[4+d_cmd->cmd_h.cmd_length-2] = d_cmd->crc_result >> 8;
	tx[4+d_cmd->cmd_h.cmd_length-1] = d_cmd->crc_result;


	rx_len = send_recive(dev,tx,rx,tx_len);

	for(i = 0; i < rx_len; i++){
               	 if(i % 16 == 0) printf("\n");
		 printf("0x%02x ",*(rx+i));
        }
	printf("\n");


#if 0 //check tcm command for dmd chip
	for(i = 0; i < d_cmd->cmd_h.cmd_length+4; i++){
                printf("0x%02x, ",*(tx+i));
        }
	printf("\n");
#endif	
	free(tx);
	tx = NULL;
	return 0;
}


static int tcm_cmmmond_xfer(const u8 * tcm_cmd, int cmd_lenth, u8* rx){
	width_t crc_result;
	struct dmt_cmd dmd_tcm_cmd;
	memset(&dmd_tcm_cmd,0,sizeof(dmd_tcm_cmd));
	dmd_tcm_cmd.cmd_h.rx_tx_flag.send_flag = SEND_CMD;
	dmd_tcm_cmd.cmd_h.cmd_length = cmd_lenth + 2;
	dmd_tcm_cmd.cmd_h.xor_result = dmd_tcm_cmd.cmd_h.rx_tx_flag.send_flag ^ dmd_tcm_cmd.cmd_h.cmd_length;
	dmd_tcm_cmd.tcm_data_buf = tcm_cmd;
	dmd_tcm_cmd.crc_result = crcCompute_dmt(tcm_cmd,cmd_lenth);
	tcm_transmit_cmd(&dmd_tcm_cmd, cmd_lenth);
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


int main(int argc,char *argv[]){
	int fd;

	int i;
	u8 rx[500];
	char cmd;
	char d = 0;
	//crcInit();
	fd = uart_init(uart0);
	if(fd < 0){
                return -1;
        }
	if(set_opt(fd, 115200, 8, 'N', 1) < 0){
                return -1;
        }

	dev = fd;
        show_help(); 
	printf("waiting for you enter: \n");

	do{
		scanf("%c", &cmd);
		fflush(stdin);
		switch (cmd)
		{
			case 'h':
				show_help();
				break;
			case 'r':
				printf("pcr read ('h' for help)");
				tcm_cmmmond_xfer(tcm_pcrread,sizeof(tcm_pcrread),rx);
				break;
			case 'R':
				printf("pcr reset ('h' for help)");
				tcm_cmmmond_xfer(tcm_pcrreset,sizeof(tcm_pcrreset),rx);
				break;
			case 'S':
				printf("TCM start up ('h' for help)");
				tcm_cmmmond_xfer(tcm_startup,sizeof(tcm_startup),rx);
				break;
			case 't':
                                printf("TCM self test full ('h' for help)");
                                tcm_cmmmond_xfer(tcm_self_test_full,sizeof(tcm_self_test_full),rx);
                                printf("get self test result:");
				tcm_cmmmond_xfer(tcm_get_test_result,sizeof(tcm_get_test_result),rx);
				break;
			case 'g':
				printf("TCM self test full ('h' for help)");
                                tcm_cmmmond_xfer(tcm_get_test_result,sizeof(tcm_get_test_result),rx);
                                break;
			case 'q':
				goto out;
			case 'e':
				printf("pcr extend ('h' for help)");
				tcm_cmmmond_xfer(tcm_pcrextend,sizeof(tcm_pcrextend),rx);
				break;
	#if 0 
			default:
				printf("enter err!  ");
				show_help();
				break;
	#endif
		}
		fflush(stdin);
	}while(1);

out:
	close_uart(fd);	
	return 0;
}

