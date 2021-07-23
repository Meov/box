/*
 * dmesg.c -- Print out the contents of the kernel ring buffer
 *
 * Copyright (C) 1993 Theodore Ts'o <tytso@athena.mit.edu>
 * Copyright (C) 2011 Karel Zak <kzak@redhat.com>
 *
 * This program comes with ABSOLUTELY NO WARRANTY.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

#include "conf.h"

static int verbose;
static int trace_interval;

static void sig_handler_quit(int signum)
{
    printf("\ncaught signal %d, setting flaq to quit.\n", signum);

    if(signum != SIGABRT){
        signal(SIGABRT,SIG_DFL);
        abort();
    }
}

void *trace_pthread(void *arg)
{
	struct timespec current;
	struct timespec last_stat;

	clock_gettime(CLOCK_MONOTONIC, &last_stat);

	while(1)
	{
		clock_gettime(CLOCK_MONOTONIC, &current);
		if (current.tv_sec - last_stat.tv_sec >= trace_interval) {
			if (verbose) {
				printf("trace pthread call.\n");
			}
			last_stat = current;
		}
	}
}

int trace(void)
{
	int err = -1;
	pthread_t pid;
	
	err = pthread_create(&pid, NULL, trace_pthread, NULL);
	if(0 != err)
	{
		printf("pthread_create failed\r\n");
		return -1;
	}
	
	return 0;
}

int main(int argc, char *argv[])
{
	int i;
	int c;

	trace_interval = 1;

	for (i = 1; i < argc; i++) {
		printf("argc(%d) argv[%d] (%s)\n", argc, i, argv[i]);
	}

	while ((c = getopt(argc, argv, "hvi:")) != EOF) {
		switch (c) {
		case 'h':
			goto usage;
		case 'v':
			verbose++;
			continue;
		case 'i':
			trace_interval = atoi(optarg);
			continue;
usage:
		printf("usage: [-h] [-v] [-i interval].\n");
		printf("       [-h]           ----- help.\n");
		printf("       [-v]           ----- verbose.\n");
		printf("       [-i interval]  ----- trace interval.\n");
		return -1;
		}
	}

	printf("%s %d: app test.\n", __func__, __LINE__);
	
	signal(SIGINT, sig_handler_quit);
	
	trace();

	while (1) {
		sleep(1);
	}

	return 0;
}
