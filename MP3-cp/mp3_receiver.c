/**
 * CS438 - Spring 2013 - MP3
 *
 * Do not modify this file, we will replace it with a different one
 * when we grade your assignment. Your client implementation should be
 * limited to receiver.c, and receiver.h
 *
 */


#include "receiver.h"
#include "stdio.h"
#include "stdlib.h"
#include "mp3channel.h"

int main(int argc, char** argv)
{

	//check the correct usage
	if (argc != 3){
		printf("Usage: %s <port> <filename>\n", argv[0]);
		exit(1);
	}

	/*
	 * initialize the mp3channel library, you can modify the parameters of
	 * setMP3Params:
	 * 	1st - maximum queue size
	 * 	2nd - 0 for no congestion / 1 activates random traffic
	 * 	3rd - packet transmission delay
	 * 	4th - probability of packet loss (value between 0 and 1)
	 * 	5th - probability of corrupting one byte of a packet that was not dropped
	 *
	 *	DO NOT use this function anywhere else in your code, or your score will be 0
	 */
	mp3_init();
	setMP3Params(1000, 1, 10000, 0.2, 0.2);

	// Run start the client
	run_receiver(argv[1], argv[2]);

	// Once the client has received the file, print stats on the client side and exit
	printMP3Statistics();

	return 0;
}
