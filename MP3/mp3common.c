/**
 * CS438 - Spring 2013 - MP3 - mp3common.c
 *
 * Use this file for the functions that your receiver and sender will share
 *
 */

#include "mp3common.h"

/**
 * Open a UDP listening socket at the passed in port
 * @param  port Port
 * @return the socketfd the listener is listening on
 */
int openUDPListenerSocket(char *port){
  int sockfd;
  struct addrinfo hints, *servinfo, *p;
  int rv;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE; // use my IP

  if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

  // loop through all the results and bind to the first we can
  for(p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype,
        p->ai_protocol)) == -1) {
      perror("listener: socket");
      continue;
    }

    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      perror("listener: bind");
      continue;
    }

    break;
  }

  if (p == NULL) {
    fprintf(stderr, "listener: failed to bind socket\n");
    return 2;
  }

  freeaddrinfo(servinfo);

  printf("listener: waiting to recvfrom...%s\n", port);

  return sockfd;
}

/**
 * Establish a udp conenction to the passed in host and port and send the passed in message
 * @param  host Host String
 * @param  port Port String
 * @param  msg Message String
 * @return 0 on success
 */
int sendUDPMessageTo(char *host, char *port, char *msg, int bytes){
  struct addrinfo hints, *servinfo, *p;
  int sockfd, rv;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;

  if ((rv = getaddrinfo(host, port, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

  // loop through all the results and make a socket
  for(p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype,
        p->ai_protocol)) == -1) {
      perror("talker: socket");
      continue;
    }

    break;
  }

  if (p == NULL) {
    fprintf(stderr, "talker: failed to bind socket\n");
    return 2;
  }

  printf("Sending to %s:%s\n", host, port);
  sendStringUDP(sockfd, p, msg, bytes);
  //Clean Up
  freeaddrinfo(servinfo);
  // close(sockfd);

  return sockfd;
}

/**
 * Sends a UDP message to through the passed in socket with the passed in buffer as the message
 * @param sockfd Socket
 * @param dest Destination addressInfo
 * @param buffer string to send
 */
void sendStringUDP(int sockfd, struct addrinfo * dest, char * buffer, int bytes){
  int numbytes = mp3_sendto(sockfd, buffer, bytes, 0, dest->ai_addr, dest->ai_addrlen);
  if (numbytes == -1) {
    perror("talker: mp3_sendto");
    exit(1);
  }
}

/**
 * Receives on the passed in udp socket and prints the data if the print parameter is not 0
 * @param  sockfd Socket
 * @param  receiveBuffer the array to hold the return string
 * @param  print 
 */
int receiveUDPMessageAndPrint(int sockfd, char receiveBuffer[MAX_PKTSIZE], int print){
  int numbytes;
  struct sockaddr_storage their_addr;
  socklen_t addr_len;

  addr_len = sizeof(their_addr);

  if ((numbytes = recvfrom(sockfd, receiveBuffer, MAX_PKTSIZE - 1, 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) {
    perror("recvfrom");
    exit(1);
  }
  receiveBuffer[numbytes] = '\0';

  if (print != 0) {
    printf("Received: %s", receiveBuffer);
  }
  return numbytes;
}
