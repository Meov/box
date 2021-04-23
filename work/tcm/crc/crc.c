#include "crc.h"  
/* 
 * An array containing the pre-computed intermediate result for each 
 * possible byte of input. This is used to speed up the computation. 
 */  
static width_t crcTable[256];  

/** 
 * Initialize the CRC lookup table. 
 * This table is used by crcCompute() to make CRC computation faster. 
 */  
void crcInit(void)  
{  
	width_t remainder;  
	width_t dividend;  
	int bit;  
	/* Perform binary long division, a bit at a time. */  
	for(dividend = 0; dividend < 256; dividend++)  
	{  
		/* Initialize the remainder.  */  
		remainder = dividend << (WIDTH - 8);  
		/* Shift and XOR with the polynomial.   */  
		for(bit = 0; bit < 8; bit++)  
		{  
			/* Try to divide the current data bit.  */  
			if(remainder & TOPBIT)  
			{  
				remainder = (remainder << 1) ^ POLYNOMIAL;  
			}  
			else  
			{  
				remainder = remainder << 1;  
			}  
		}  
		/* Save the result in the table. */  
		crcTable[dividend] = remainder;  
	}  
} /* crcInit() */  

/** 
 * Compute the CRC checksum of a binary message block. 
 * @para message, 用来计算的数据 
 * @para nBytes, 数据的长度 
 * @note This function expects that crcInit() has been called 
 *       first to initialize the CRC lookup table. 
 */  

/*标准的CRC 如果需要修改CRC16的协议方式，修改crc.h中的多项式值以及初始值*/
width_t crcCompute(unsigned char * message, unsigned int nBytes)  
{  
	unsigned int offset;  
	unsigned char byte;  
	width_t remainder = INITIAL_REMAINDER;  
	/* Divide the message by the polynomial, a byte at a time. */  
	for( offset = 0; offset < nBytes; offset++)  
	{  
		byte = (remainder >> (WIDTH - 8)) ^ message[offset];  
		remainder = crcTable[byte] ^ (remainder << 8);  
	}  
	/* The final remainder is the CRC result. */  
	return (remainder ^ FINAL_XOR_VALUE);  
} /* crcCompute() */  


width_t crcCompute_dmt(unsigned char * message, unsigned int nBytes){
	int crc = 0x6363;
	int i = 0;
	width_t  crc_res;
	unsigned char tmp;
	for(i = 0; i < nBytes; i++){
		tmp = (char)(message[i] ^ (char)(crc & 0x00ff));
		tmp = tmp ^ (tmp << 4);
		crc = ((crc >> 8)^(tmp << 8)^(tmp<<3)^(tmp >> 4));
	}

	crc_res = crc;
	return crc_res;
}
