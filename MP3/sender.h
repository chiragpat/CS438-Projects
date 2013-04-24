#ifndef CLIENT_H_
#define CLIENT_H_

#include <stdio.h>
#include "mp3channel.h"
#include "mp3common.h"


int run_sender(char* hostname, char* port, char* filename);
int wait_for_receive(int sockfd, char* receive_buffer, struct timeval timeout);

#endif
