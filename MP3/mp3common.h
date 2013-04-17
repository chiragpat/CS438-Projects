/**
 * CS438 - Spring 2013 - MP3 - mp3common.h
 *
 * Use this file for the functions that your receiver and sender will share
 *
 */


#ifndef MP3COMMON_H_
#define MP3COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "mp3channel.h"

int openUDPListenerSocket(char *port);
int sendUDPMessageTo(char *host, char *port, char *msg, int bytes);
void sendStringUDP(int sockfd, struct addrinfo * dest, char * buffer, int bytes);
int receiveUDPMessageAndPrint(int sockfd, char receiveBuffer[MAX_PKTSIZE], int print);

static char port_number[5] = "4001";

#endif
