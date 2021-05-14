#ifndef _DMD_UART_H
#define _DMD_UART_H

int uart_init(const char * uart_name);
int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop);
#if UES_UART
int send_recive(int dev, char *tx, char *rx, int tx_len);
#endif
int close_uart(int fd);
#endif
