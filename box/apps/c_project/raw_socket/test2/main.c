#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <linux/if_ether.h>
//#include <linux/in.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_MAX 2048

int main(int argc, char **argv)
{
	int rawsock;
	char buffer[BUFFER_MAX];
	char * ethhead;
	char * iphead;
	char * phead;

	// create rawsocket
	if ((rawsock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_IP))) < 0){
		perror("rawsock create error");
		exit(1);
	}

	long framecount = 0;
	while ( 1 ) {
		int readnum = recvfrom(rawsock, buffer, 2048, 0 , NULL, NULL);
		if (readnum < 42) {
			printf("error: head is incomplete! \n");
			exit(1);
		}

		ethhead = (char *)buffer;
		phead = ethhead;
		int ethernetmask = 0XFF;
		framecount++;

		printf("--------------Analysis Packet[%ld]-------------\n", framecount);
		// printf src mac and dst mac
		fprintf(stderr, "MAC:");
		int i = 6;
		for (; i <= 11; i++)
			fprintf(stderr, "%.2x:", phead[i]&ethernetmask);
		fprintf(stderr, "--------->");
		for (i = 0; i <= 5; i++)
			fprintf(stderr, "%.2x", phead[i]&ethernetmask);
		printf("\n");

		iphead = ethhead + 14;
		phead = iphead + 12;

		// print ip address
		printf("IP:");
		for (i = 0; i<= 3; i++) {
			printf("%d", phead[i]&ethernetmask);
			if (i !=3) {
				printf(".");
			}
		}
		printf("---------->");
		for (i = 4; i <= 7; i++) {
			printf("%d", phead[i] & ethernetmask);
			if (i != 7) {
				printf(".");
			}
		}
		printf("\n");

		int prototype = (iphead + 9)[0];
		phead = iphead + 20;

		// print Protocal mesg 
		printf("Protocal:");

		switch(prototype) {
			case IPPROTO_ICMP:
				printf("ICMP\n");
				break;
			case IPPROTO_IGMP:
				printf("IGMP\n");
				break;
			case IPPROTO_TCP:
				printf("TCP | source port: %u |", (phead[0]<<8) & 0xFF00 | phead[1] & 0xFF);
				printf("destport: %u\n", (phead[2]<<8) & 0xFF00 | phead[3] & 0xFF);
				int i;
				printf("data: \n");
				for (i = 0; i < readnum -54; i++)
					putchar(phead[19 + i]);
				printf("\n");
				break;

			case IPPROTO_UDP:
				printf("UDP | source port: %u |", (phead[0]<<8) & 0xFF00 | phead[1]&0xFF);
				printf("destport: %u\n", (phead[2]<<8) & 0XFF00 | phead[3] & 0xFF);
				break;
			case IPPROTO_RAW:
				printf("RAW:\n");
				break;
			default:
				printf("Unknow\n");


		}
		printf("-----------------------------END-------------------------------\n");


	}
	return 0;
}




