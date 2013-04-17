#include "receiver.h"

int run_receiver(char* portno, char* filename){

	/*
	 * Your implementation should be self-contained in this file, no other source
	 * or header file will be considered by our autograder.
	 */

  int sockfd;
  char receiveBuffer[MAX_PKTSIZE] = "";


  sockfd = openUDPListenerSocket(portno);

  while(1) {
    receiveUDPMessageAndPrint(sockfd, receiveBuffer, 1);
  }
	

  close(sockfd);
  return 0;
}

