#include "router.h"

int main(int argc, char *argv[]){
  int sockfd, addr;
  char sendBuffer[MAXDATASIZE];
  char receiveBuffer[MAXDATASIZE];

  if (argc != 4) {
    fprintf(stderr,"usage: router hostname tcpport udpport\n");
    exit(1);
  }

  sockfd = establishTCPConnection(argv[1], argv[2]);

  strcpy(sendBuffer, "HELO\n");
  sendString(sockfd, sendBuffer);
  receiveAndPrint(sockfd, receiveBuffer, 0);
  addr = atoi(receiveBuffer+5);
  printf("manager replied with address %d\n", addr);

  //Clean up
  close(sockfd);
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
  // char receiveBuffer[MAXDATASIZE];

  if ((numbytes = recv(sockfd, receiveBuffer, MAXDATASIZE-1, 0)) == -1) {
      perror("recv");
      exit(1);
  }

  receiveBuffer[numbytes] = '\0';
  if (print != 0) {
    printf("Received: %s\n",receiveBuffer);
  }
}