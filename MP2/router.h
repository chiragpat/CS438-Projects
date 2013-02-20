#ifndef ROUTER_H
#define ROUTER_H

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
#include "NodeList.h"

#define MAXDATASIZE 1024

int establishTCPConnection(char *host, char *port);
void sendString(int sockfd, char * buffer);
void receiveAndPrint(int sockfd, char receiveBuffer[MAXDATASIZE], int print);

#endif