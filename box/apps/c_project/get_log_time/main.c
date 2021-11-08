#include <iostream>
#include <time.h>
#include <cstdio>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#define LOG_FILE "log.txt"


using namespace std;
void time_str_get(char *time_str){

	time_t tt;
	time( &tt );
	tt = tt + 8*3600;  // transform the time zone
	tm* t= gmtime( &tt );
	cout << tt << endl;

	sprintf(time_str,"%d-%02d-%02d %02d:%02d:%02d",
			t->tm_year + 1900,
                        t->tm_mon + 1,
                        t->tm_mday,
                        t->tm_hour,
                        t->tm_min,
                        t->tm_sec);
#if 0
	printf("%d-%02d-%02d %02d:%02d:%02d\n",
			t->tm_year + 1900,
			t->tm_mon + 1,
			t->tm_mday,
			t->tm_hour,
			t->tm_min,
			t->tm_sec);
#endif
}


int main() {
	int fd;
	char time_str[100];
	FILE* fp = fopen(LOG_FILE, "a+");

//	fd = open(LOG_FILE, O_RDWR|O_CREAT|O_TRUNC, S_IRWXU);
	time_str_get(time_str);
	cout << time_str << endl;

	strcat(time_str,"\n");
	
	fseek(fp, 0, SEEK_END);
	fwrite(time_str,strlen(time_str),1,fp);
	fclose(fp);
	
	return 0;
}


