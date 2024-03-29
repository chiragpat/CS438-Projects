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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <sys/select.h>
#include <sys/time.h>

typedef struct packet
{
    int pack_number;
    unsigned int check_sum1;
    unsigned int check_sum2;
    int size;
    char buffer[MAX_PKTSIZE - 12];
} packet_t;

typedef struct ack
{
    int pack_number;
    unsigned int check_sum1;
    unsigned int check_sum2;
} ack_t;

typedef struct handshake
{
    int num_packets;
    unsigned int check_sum1;
    unsigned int check_sum2;
    char hostname[50];
    char  port[6];
} handshake_t;

int openUDPListenerSocket(char *port);
int setup_addr(struct addrinfo *servinfo, char* port);
int sendUDPMessageTo(char *host, char *port, char *msg, int bytes, int sock);
void sendStringUDP(int sockfd, struct addrinfo * dest, char * buffer, int bytes);
int receiveUDPMessageAndPrint(int sockfd, char receiveBuffer[MAX_PKTSIZE], int print);
unsigned int check_sum(packet_t * pack, int size);
unsigned int handshake_check_sum(handshake_t *handshake);
unsigned int ack_check_sum(ack_t * ack);


#endif
