#include "router.h"

int main(int argc, char *argv[]){
  int sockfd, udpfd, addr, udpport, new_cost, rv, maxfd;
  int src, dest;
  char *msg, *host;
  FILE* socket_file;
  char sendBuffer[MAXDATASIZE], receiveBuffer[MAXDATASIZE], destPort[MAXDATASIZE];
  node_list *nodeList = malloc(sizeof(node_list));
  fd_set fds;

  init_list(nodeList);
  
  if (argc != 4) {
    fprintf(stderr,"usage: router hostname tcpport udpport\n");
    exit(1);
  }

  udpport = atoi(argv[3]);
  host = argv[1];
  sockfd = establishTCPConnection(argv[1], argv[2]);
  udpfd = openUDPListenerSocket(argv[3]);
  socket_file = fdopen(sockfd, "r");

  sendString(sockfd, "HELO\n");
  receiveAndPrint(sockfd, receiveBuffer, 0);
  addr = atoi(receiveBuffer+5);
  printf("manager replied with address %d\n", addr);

  sprintf(sendBuffer, "HOST localhost %d\n", udpport);
  sendString(sockfd, sendBuffer);
  receiveAndPrint(sockfd, receiveBuffer, 0);

  getAndSetupNeighbours(nodeList, sockfd, socket_file);
  print_list(nodeList);
  

  sendString(sockfd, "READY\n");

  FD_ZERO(&fds);
  FD_SET(sockfd, &fds);
  FD_SET(udpfd, &fds);

  maxfd = sockfd;
  if ( maxfd < udpfd ) {
    maxfd = udpfd;
  }

  while( (rv=select(maxfd+1, &fds, NULL, NULL, NULL)) >=0 ){

    if (FD_ISSET(sockfd, &fds)) {
      receiveOneLineAndPrint(socket_file, receiveBuffer, 1);

      if (strncmp(receiveBuffer, "LINKCOST", strlen("LINKCOST")) == 0) {
        new_cost = updateNodeList(receiveBuffer, addr, nodeList);
        sprintf(sendBuffer, "COST %d OK\n", new_cost);
        sendString(sockfd, sendBuffer);
        // print_list(nodeList);
      }

      if (strcmp(receiveBuffer, "END\n") == 0) {
        sendString(sockfd, "BYE\n");
        break;
      }
    }

    if (FD_ISSET(udpfd, &fds)) {
      receiveUDPMessageAndPrint(udpfd, receiveBuffer, 0);

      int controlInt = (int)receiveBuffer[0];
      if (controlInt == 1) {
        dest = byteToInt(receiveBuffer+1);
        msg = receiveBuffer + 3;
        printf("Destination: %d, Message: %s\n", dest, msg);

        node *destinationNode = get_by_node_number_list(nodeList, dest);
        if (destinationNode != NULL) {
          sprintf(destPort, "%d", destinationNode->node_port);
          memcpy(sendBuffer, receiveBuffer, (size_t) strlen(msg)+3);
          sendBuffer[0] = (char) 2;
          strcat(sendBuffer, "\n");
          sendUDPMessageTo("127.0.0.1", destPort, sendBuffer, strlen(msg)+3);
        }
      }
      else if (controlInt == 2) {
        msg = receiveBuffer + 3;

        printf("Message: %s\n", msg);
        sprintf(sendBuffer, "RECEIVED %s\n", msg);
        sendString(sockfd, sendBuffer);
      }
      else {
        printf("Message without controlInt: %s\n", receiveBuffer); 
      }

    }

    FD_ZERO(&fds);
    FD_SET(sockfd,&fds);
    FD_SET(udpfd,&fds);
  }
  
  if (rv == -1) {
    perror("select()");   
    exit(1);
  }

  //Clean up
  close(sockfd);
  destroy_list(nodeList);
  free(nodeList);
  return 0;
}

void getAndSetupNeighbours(node_list* nodeList, int sockfd, FILE* socket_file) {
  int ret, i, node_number, node_port, cost;
  char temp[MAXDATASIZE], receiveBuffer[MAXDATASIZE];
  char *tok;

  sendString(sockfd, "NEIGH?\n");
  while ( strcmp(receiveBuffer, "DONE\n") != 0 ) {
    ret = receiveOneLineAndPrint(socket_file, receiveBuffer, 0);
    if (ret == 1 && strcmp(receiveBuffer, "DONE\n") != 0) {
      i = 0;
      strcpy(temp, receiveBuffer);
      tok = strtok(temp, " \n");
      while (tok != NULL) {
        if (i == 1) {
          node_number = atoi(tok);
        }

        if (i == 3) {
          node_port = atoi(tok);
        }

        if (i == 4) {
          cost = atoi(tok);  
        }
        tok = strtok(NULL, " \n");
        i++; 
      }
      append_list(nodeList, node_number, node_port, cost);
    }
  }
}

int updateNodeList(char receiveBuffer[MAXDATASIZE], int addr, node_list *nodeList){
  int i, first_node_number, second_node_number, node_number, new_cost;
  char temp[MAXDATASIZE];
  char *tok;

  i = 0;
  strcpy(temp, receiveBuffer);
  tok = strtok(temp, " \n");
  while(tok != NULL) {
    if (i == 1) {
      first_node_number = atoi(tok);
    }

    if (i == 2) {
      second_node_number = atoi(tok);
    }

    if (i == 3) {
      new_cost = atoi(tok);
    }
    tok = strtok(NULL, " \n");
    i++;
  }

  if (first_node_number == addr) {
    node_number = second_node_number;
  }
  else {
    node_number = first_node_number;
  }

  edit_cost(nodeList, node_number, new_cost);
  return new_cost;
}

int byteToInt(char* p) {
  int result = 0;  // initialize;
  char* rv = (char*)&result;
  rv[0] = p[1];
  rv[1] = p[0];
  return result;
}