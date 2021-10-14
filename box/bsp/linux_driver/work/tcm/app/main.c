#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../tcm_driver/tcm_interface.h"
#include "tcm_command.h"
#include "../serial_dir/dmd_uart.h"

static const char *uart0 = "/dev/ttyS0";
static const char *spi_tcm = "/dev/tcm";
#define TCM_DEV spi_tcm

/**
 * tpm2_pcr_read() - read a PCR value
 * @pcr_idx:    index of the PCR to read.
 * @ref_buf:    buffer to store the resulting hash,
 *
 * 0 is returned when the operation is successful. If a negative number is
 * returned it remarks a POSIX error code. If a positive number is returned
 * it remarks a TPM error.
 */
int tcm_pcr_read(struct tcm_dev *dev, int pcr_idx, u8 *res_buf)
{
	struct tcm_cmd cmd;
	uint16 crc_result;
	uint16 cmd_total_lenth;
	uint16 tcm_pcrread_lenth;
	
	int i = 0;
	int rc;
	u8 *buf;
	unsigned char *p;
	const struct cmd_header tcm_pcrread_heder = {
		.rx_tx_flag = SEND_CMD,
		.cmd_length = Reverse16(PCRREAD_CMD_LENTH+CRC_LENTH),
		.xor_result = SEND_CMD ^ (PCRREAD_CMD_LENTH+CRC_LENTH),
	};

	cmd.header = tcm_pcrread_heder;
	cmd.params.pcrread_in.cmd_common.flag = Reverse16(TCM_TAG_RQU_COMMAND);
	cmd.params.pcrread_in.cmd_common.data_lenth = Reverse32(PCRREAD_CMD_LENTH);
	cmd.params.pcrread_in.cmd_common.cmd_code = Reverse32(TCM_ORG_PCRRead);
	cmd.params.pcrread_in.pcr_index = Reverse32(pcr_idx);
	crc_result = crcCompute_dmt(&(cmd.params.pcrread_in),PCRREAD_CMD_LENTH);
	cmd.crc_result = Reverse16(crc_result);
	cmd_total_lenth = PCRREAD_CMD_LENTH + HEADER_SIZE_BYTES + CRC_LENTH;
	rc = tcm_transmit_cmd(dev,&cmd,cmd_total_lenth,"pcr read test");	

	/*read data back*/
	
	if (rc == 0) {
		cmd_total_lenth = Reverse32(cmd.params.pcrread_in.cmd_common.data_lenth);
		tcm_pcrread_lenth = Reverse32(cmd.params.pcrread_in.cmd_common.data_lenth) - TCM_CMD_COMMON_LENTH;
		buf = cmd.params.pcrread_out.digest;
		memcpy(res_buf, buf, TPM_DIGEST_SIZE);
	}

#ifdef TCM_DEBUG
	printf("\naddr of buf_offset: %p\n",&(cmd.params.pcrread_out.digest));
	printf("\ntcm pcr data lenth: %d  tcm pcrread: ",tcm_pcrread_lenth);
	for( i = 0; i < tcm_pcrread_lenth; i++){
        	if(i%16 == 0) printf("\n");
		printf("0x%02x ", res_buf[i]);
	}
#endif
        
	return rc;
}

int tcm_start_up(struct tcm_dev *dev, u8 *res_buf)
{
        struct tcm_cmd cmd;
        uint16 crc_result;
        uint16 cmd_total_lenth;
        int i = 0;
        int rc;
        u8 *buf;
	unsigned char *p;
	uint16 cmd_length = sizeof(tcm_startup);
	u8 crc_lenth = CRC_LENTH;

        const struct cmd_header tcm_startup_heder = {
                .rx_tx_flag = SEND_CMD,
                .cmd_length = Reverse16(cmd_length+CRC_LENTH),
                .xor_result = SEND_CMD ^ (cmd_length+CRC_LENTH),
        };

        cmd.header = tcm_startup_heder;
        cmd.params.startup_in.cmd_common.flag = Reverse16(TCM_TAG_RQU_COMMAND);
        cmd.params.startup_in.cmd_common.data_lenth = Reverse32(cmd_length);
        cmd.params.startup_in.cmd_common.cmd_code = Reverse32(TCM_ORG_Startup);
        cmd.params.startup_in.startup_type = Reverse16(START_UP_TYPE_1);
        crc_result = crcCompute_dmt(&(cmd.params.startup_in),cmd_length);
        cmd.crc_result = Reverse16(crc_result);
        cmd_total_lenth = cmd_length + HEADER_SIZE_BYTES + CRC_LENTH;
        rc = tcm_transmit_cmd(dev,&cmd,cmd_total_lenth,"TCM startup");

	/*read data back*/
        if (rc == 0) {
                 buf = (u8 *)(&(cmd.params.startup_out.cmd_common));
                 memcpy(res_buf, buf, TCM_CMD_COMMON_LENTH);
        }

#ifdef TCM_DEBUG
        printf("\ntcm startup read:");
        for( i = 0; i < TCM_CMD_COMMON_LENTH; i++)
                if(i % 16 == 0)printf("\n");
	        printf("0x%02x ", res_buf[i]);
#endif
        return rc;
}


int tcm_pcr_extend(struct tcm_dev *dev,int pcr_idx, u8 *res_buf)
{
        struct tcm_cmd cmd;
        uint16 crc_result;
        uint16 cmd_total_lenth;
        int i = 0;
        int rc;
        u8 *buf;
        unsigned char *p;
        uint16 cmd_length = sizeof(tcm_pcrextend);
        u8 crc_lenth = CRC_LENTH;

        const struct cmd_header tcm_pcrextend_heder = {
                .rx_tx_flag = SEND_CMD,
                .cmd_length = Reverse16(cmd_length+CRC_LENTH),
                .xor_result = SEND_CMD ^ (cmd_length+CRC_LENTH),
        };

        cmd.header = tcm_pcrextend_heder;
        cmd.params.pcrextend_in.cmd_common.flag = Reverse16(TCM_TAG_RQU_COMMAND);
        cmd.params.pcrextend_in.cmd_common.data_lenth = Reverse32(cmd_length);
        cmd.params.pcrextend_in.cmd_common.cmd_code = Reverse32(TCM_ORG_PcrExtend);
        cmd.params.pcrextend_in.pcr_index = Reverse32(pcr_idx);

	memcpy(cmd.params.pcrextend_in.digest,degist_extend,sizeof(degist_extend));
#ifdef TCM_DEBUG
	printf("\ntcm extend send data: ");
	for(i = 0; i < cmd_length; i++){
                if(i % 16 == 0)printf("\n");
                printf("0x%02x ",((u8 *)&(cmd.params.pcrextend_in))[i]);
        }
	printf("\n");
#endif
	crc_result = crcCompute_dmt(&(cmd.params.pcrextend_in),cmd_length);
	cmd.crc_result = Reverse16(crc_result);
        cmd_total_lenth = cmd_length + HEADER_SIZE_BYTES + CRC_LENTH;

        rc = tcm_transmit_cmd(dev,&cmd,cmd_total_lenth,"TCM pcrextend");
        /*read data back*/
        if (rc == 0) {
                 buf = (u8 *)(&(cmd.params.pcrextend_out));
                 memcpy(res_buf, buf, sizeof(cmd.params.pcrextend_out));
        }

#ifdef TCM_DEBUG
        printf("\ntcm extend read data:");
        for( i = 0; i < sizeof(cmd.params.pcrextend_out); i++){
		if(i % 16 == 0)printf("\n");
                printf("0x%02x ", res_buf[i]);
	}
#endif
        return rc;
}

int tcm_get_random(struct tcm_dev *dev,int random_lenth, u8 *res_buf)
{
        struct tcm_cmd cmd;
        uint16 crc_result;
        uint16 cmd_total_lenth;
        int i = 0;
        int rc;
        u8 *buf;
        unsigned char *p;
        uint16 cmd_length = sizeof(tcm_get_random_data);
        u8 crc_lenth = CRC_LENTH;

        const struct cmd_header tcm_getrandom_heder = {
                .rx_tx_flag = SEND_CMD,
                .cmd_length = Reverse16(cmd_length+CRC_LENTH),
                .xor_result = SEND_CMD ^ (cmd_length+CRC_LENTH),
        };

        cmd.header = tcm_getrandom_heder;
        cmd.params.getrandom_in.cmd_common.flag = Reverse16(TCM_TAG_RQU_COMMAND);
        cmd.params.getrandom_in.cmd_common.data_lenth = Reverse32(cmd_length);
        cmd.params.getrandom_in.cmd_common.cmd_code = Reverse32(TCM_ORG_GetRandom);
        cmd.params.getrandom_in.size = Reverse32(random_lenth);

//#ifdef TCM_DEBUG
#if 1
        printf("\ntcm get random tcm data: ");
        for(i = 0; i < cmd_length; i++){
                if(i % 16 == 0)printf("\n");
                printf("0x%02x ",((u8 *)&(cmd.params.getrandom_in))[i]);
        }
        printf("\n");
#endif
        crc_result = crcCompute_dmt(&(cmd.params.getrandom_in),cmd_length);
        cmd.crc_result = Reverse16(crc_result);
        cmd_total_lenth = cmd_length + HEADER_SIZE_BYTES + CRC_LENTH;
        rc = tcm_transmit_cmd(dev,&cmd,cmd_total_lenth,"TCM getrandom");
        /*read data back*/
        if (rc == 0) {
                 buf = (u8 *)(&(cmd.params.getrandom_out));
                 memcpy(res_buf, buf, sizeof(cmd.params.getrandom_out));
        }

//#ifdef TCM_DEBUG
#if 1
	struct tcm_get_random_out * buf_get = (struct tcm_get_random_out *)res_buf;
        printf("\ntcm get random rx data:");
        for( i = 0; i < Reverse32(cmd.params.getrandom_out.size); i++){
                if(i % 16 == 0)printf("\n");
                printf("0x%02x ", buf_get->buffer[i]);
        }
#endif
        return rc;
}


int main(int argc,char *argv[]){
	int fd;

	int i;
	u8 rx[500];
	char cmd;
	char d = 0;
	int status;
	//crcInit();
	struct tcm_dev *dev;
	dev = malloc(sizeof(*dev));
	if(dev == NULL){
		printf("err dev malloced err!\n");
	}	

	dev->dev_name = TCM_DEV; 
	status = tcm_dev_init(dev);

	if(status < 0){
		free(dev);
		exit(status);
	}
	show_help();
	printf(" use dev: %s fd: %d waiting for you enter: \n",dev->dev_name,
							       dev->fd);
	tcm_start_up(dev,rx);
	tcm_pcr_read(dev,1,rx);
	tcm_pcr_extend(dev,1,rx);
	tcm_get_random(dev,0x10,rx);
#if 0
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
				tcm_cmmmond_xfer(dev,tcm_pcrread,sizeof(tcm_pcrread),rx);
				break;
			case 'R':
				printf("pcr reset ('h' for help)");
				tcm_cmmmond_xfer(dev,tcm_pcrreset,sizeof(tcm_pcrreset),rx);
				break;
			case 'S':
				printf("TCM start up ('h' for help)");
				tcm_cmmmond_xfer(dev,tcm_startup,sizeof(tcm_startup),rx);
				break;
			case 't':
                                printf("TCM self test full ('h' for help)");
                                tcm_cmmmond_xfer(dev,tcm_self_test_full,sizeof(tcm_self_test_full),rx);
                                printf("get self test result:");
				tcm_cmmmond_xfer(dev,tcm_get_test_result,sizeof(tcm_get_test_result),rx);
				break;
			case 'g':
				printf("TCM self test full ('h' for help)");
                                tcm_cmmmond_xfer(dev,tcm_get_test_result,sizeof(tcm_get_test_result),rx);
                                break;
			case 'q':
				goto out;
			case 'e':
				printf("pcr extend ('h' for help)");
				tcm_cmmmond_xfer(dev,tcm_pcrextend,sizeof(tcm_pcrextend),rx);
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
#endif
	tcm_dev_release(dev);
	return 0;
}

