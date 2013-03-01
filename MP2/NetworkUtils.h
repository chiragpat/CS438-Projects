#ifndef NETWORKUTILS_H
#define NETWORKUTILS_H

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

#define MAXDATASIZE 1024

int establishTCPConnection(char *host, char *port);
int openUDPListenerSocket(char *port);
int sendUDPMessageTo(char *host, char *port, char *msg, int bytes);
void sendString(int sockfd, char * buffer);
void sendStringUDP(int sockfd, struct addrinfo * dest, char * buffer, int bytes);
void receiveAndPrint(int sockfd, char receiveBuffer[MAXDATASIZE], int print);
int receiveOneLineAndPrint(FILE* socket_file, char receiveBuffer[MAXDATASIZE], int print);
void receiveUDPMessageAndPrint(int sockfd, char receiveBuffer[MAXDATASIZE], int print);

#endif