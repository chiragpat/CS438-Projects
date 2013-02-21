#include "router.h"

int main(int argc, char *argv[]){
  int sockfd, addr, udpport, new_cost;
  FILE* socket_file;
  char sendBuffer[MAXDATASIZE], receiveBuffer[MAXDATASIZE];
  node_list *nodeList = malloc(sizeof(node_list));
  init_list(nodeList);
  char * tok;
  
  
  if (argc != 4) {
    fprintf(stderr,"usage: router hostname tcpport udpport\n");
    exit(1);
  }

  udpport = atoi(argv[3]);
  sockfd = establishTCPConnection(argv[1], argv[2]);
  socket_file = fdopen(sockfd, "r");

  sendString(sockfd, "HELO\n");
  receiveAndPrint(sockfd, receiveBuffer, 0);
  addr = atoi(receiveBuffer+5);
  printf("manager replied with address %d\n", addr);

  sprintf(sendBuffer, "HOST localhost %d\n", udpport);
  sendString(sockfd, sendBuffer);
  receiveAndPrint(sockfd, receiveBuffer, 1);

  getAndSetupNeighbours(nodeList, sockfd, socket_file);
  print_list(nodeList);
  

  sendString(sockfd, "READY\n");
  while ( strcmp(receiveBuffer, "END\n") != 0 ) {
    receiveOneLineAndPrint(socket_file, receiveBuffer, 1);
    if (strncmp(receiveBuffer, "LINKCOST", strlen("LINKCOST")) == 0) {
      new_cost = updateNodeList(receiveBuffer, addr, nodeList);
      print_list(nodeList);
      sprintf(sendBuffer, "COST %d OK\n", new_cost);
      sendString(sockfd, sendBuffer);
    }
  }

  sendString(sockfd, "BYE\n");
  //Clean up
  close(sockfd);
  destroy_list(nodeList);
  free(nodeList);
  return 0;
}

/**
 * Establish a tcp conenction to the passed in host and port
 * @param  host Host String
 * @param  port Port String
 * @return socket file descriptor 
 */
int establishTCPConnection(char *host, char *port){
  char s[INET6_ADDRSTRLEN];
  struct addrinfo hints, *servinfo, *p;
  int sockfd, rv;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if ((rv = getaddrinfo(host, port, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

  // loop through all the results and connect to the first we can
  for(p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype,
        p->ai_protocol)) == -1) {
      perror("client: socket");
      continue;
    }

    if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      perror("client: connect");
      continue;
    }

    break;
  }

  if (p == NULL) {
    fprintf(stderr, "client: failed to connect\n");
    exit(2);
  }

  freeaddrinfo(servinfo); // all done with this structure
  return sockfd;
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

/**
 * Sends a message to through the passed in socket with the passed in buffer as the message
 * @param sockfd Socket
 * @param buffer string to send
 */
void sendString(int sockfd, char * buffer){
  int numbytes = send(sockfd, buffer, strlen(buffer), 0);
  if (numbytes == -1) {
    perror("send");
    exit(1);
  }
}

/**
 * Receives on the passed in socket and prints the data if the print parameter is not 0
 * @param  sockfd Socket
 * @param  receiveBuffer the array to hold the return string
 * @param  print 
 */
void receiveAndPrint(int sockfd, char receiveBuffer[MAXDATASIZE], int print){
  int numbytes;

  if ((numbytes = recv(sockfd, receiveBuffer, MAXDATASIZE-1, 0)) == -1) {
      perror("recv");
      exit(1);
  }

  receiveBuffer[numbytes] = '\0';
  if (print != 0) {
    printf("Received: %s",receiveBuffer);
  }
}

/**
 * Receives one line on the passed in socket and prints the data if the print parameter is not 0
 * @param  sockfd Socket
 * @param  receiveBuffer the array to hold the return string
 * @param  print 
 * @return -1 on failure and 1 on success
 */
int receiveOneLineAndPrint(FILE* socket_file, char receiveBuffer[MAXDATASIZE], int print){
  if (fgets(receiveBuffer, MAXDATASIZE-1, socket_file) != NULL) {
      if (print != 0) {
        printf("Received: %s",receiveBuffer);
      }
      return 1;
  }
  else {
    return -1;
  }
}