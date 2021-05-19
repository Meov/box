#ifndef _TCM_INTERFACE_H
#define _TCM_INTERFACE_H
#define TPM_MAX_RNG_DATA	128
#define TPM_DIGEST_SIZE 32

#define TCM_TAG_RQU_COMMAND 0x00c1
#define TCM_ORG_PCRRead 0x00008015
#define TCM_ORG_Startup 0x00008099
#define TCM_ORG_PcrExtend 0x00008014
#define TCM_ORG_GetRandom 0x00008046


#define CRC_LENTH 2

typedef unsigned char u8;
typedef unsigned int  uint32;
typedef unsigned short int  uint16;

//#define TCM_DEBUG   /*for tcm debug */

#define Reverse16(x) \
         ((uint16)( \
                      (((uint16)(x) & (uint16)0x00ff) << 8 ) | \
                      (((uint16)(x) & (uint16)0xff00) >> 8 ) ))

struct tcm_cmd_common{
	uint16 flag;
	uint32 data_lenth;
	uint32 cmd_code;
} __attribute__ ((packed));

struct cmd_header{
        char  rx_tx_flag;
        uint16 cmd_length;
        char xor_result;
}__attribute__ ((packed));

enum flag {
        SEND_CMD = 0x3A,
        RECV_CMD = 0x3B,
};
enum sartup_type {
        START_UP_TYPE_1 = 0X0001,
        START_UP_TYPE_2 = 0X0002,
        START_UP_TYPE_3 = 0X0003,
};

struct tcm_startup_in {
	struct tcm_cmd_common cmd_common;
	uint16 startup_type;
} __attribute__ ((packed));

struct tcm_startup_out {
	struct tcm_cmd_common cmd_common;
//	uint16 startup_type;
} __attribute__ ((packed));


struct  tcm_pcr_read_in{
	struct tcm_cmd_common cmd_common;
        uint32 pcr_index;
} __attribute__ ((packed));

struct  tcm_pcr_read_out{
	struct tcm_cmd_common cmd_common;
        u8     digest[TPM_DIGEST_SIZE];
} __attribute__ ((packed));

struct tcm_get_random_in {
	struct tcm_cmd_common cmd_common;
        uint32  size;
} __attribute__ ((packed));;

struct tcm_get_random_out {
	struct tcm_cmd_common cmd_common;
        uint32  size;
        u8      buffer[TPM_MAX_RNG_DATA];
} __attribute__ ((packed));

struct  tcm_self_test_in{
	u8      full_test;
} __attribute__ ((packed));

struct  tcm_pcr_extend_in{
	struct tcm_cmd_common cmd_common;
	uint32	pcr_index;
        u8      digest[TPM_DIGEST_SIZE];
} __attribute__ ((packed));

struct  tcm_pcr_extend_out{
	struct tcm_cmd_common cmd_common;
	u8     digest[TPM_DIGEST_SIZE];
} __attribute__ ((packed));


union tcm_cmd_params {
        struct  tcm_startup_in         startup_in;
        struct  tcm_startup_out        startup_out;
        struct  tcm_self_test_in       selftest_in;
        struct  tcm_pcr_read_in        pcrread_in;
        struct  tcm_pcr_read_out       pcrread_out;
        struct  tcm_pcr_extend_in      pcrextend_in;
	struct  tcm_pcr_extend_out     pcrextend_out;
        struct  tcm_get_random_in      getrandom_in;
        struct  tcm_get_random_out     getrandom_out;
};

struct tcm_cmd{
	struct cmd_header      header;
	uint16 crc_result;
	/* union as tcm cmd data must place at the end of struct*/	
	union  tcm_cmd_params  params;
} __attribute__ ((packed));

struct dmt_cmd{
        struct cmd_header cmd_h;
        const u8* tcm_data_buf;
        uint32 crc_result;
};

struct uart_dev{
	const char *uart_name;
	uint32 fd;
};

struct tcm_dev {
	const char *dev_name;
	uint32 fd;
};

#define TCM_CMD_COMMON_LENTH  sizeof(struct tcm_cmd_common)
#define HEADER_SIZE_BYTES sizeof(struct cmd_header)

uint32 tcm_transmit_cmd(struct tcm_dev *dev,  void* cmd, uint16 len,const char *desc);
void show_help(void);
uint32 Reverse32(uint32 x);

#endif
