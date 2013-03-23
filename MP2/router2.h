#ifndef ROUTER_H
#define ROUTER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include "NetworkUtils.h"
#include "NodeGraph.h"
#include "Djikstras.h"

#define MAXDATASIZE 1401

typedef struct {
  int controlInt;
  int node0_number;
  int node1_number;
  int node0_port;
  int node1_port;
  int cost;
  time_t t;
} LinkMessage;

typedef struct {
  int destination_number;
  char message[MAXDATASIZE];
} Message;

void getAndSetupNeighbours(NodeGraph *nodegraph, int sockfd, FILE* socket_file);
LinkMessage updateNodeList(char receiveBuffer[MAXDATASIZE], int addr, NodeGraph *nodegraph);
void broadcastLinkInfo(NodeGraph* graph, int udpfd);
void broadcastOneLinkInfo(NodeGraph* graph, LinkMessage message, int udpfd);
unsigned short byteToInt(char* p);

#endif