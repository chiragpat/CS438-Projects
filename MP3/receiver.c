#include "receiver.h"

int run_receiver(char* portno, char* filename){

  /*
   * Your implementation should be self-contained in this file, no other source
   * or header file will be considered by our autograder.
   */

  int sockfd;
  char receiveBuffer[MAX_PKTSIZE] = "";
  FILE *file;

  file = fopen(filename, "w+");
  sockfd = openUDPListenerSocket(portno);

  while(1) {
    receiveUDPMessageAndPrint(sockfd, receiveBuffer, 1);
    if (strcmp(receiveBuffer, "DONE\n") != 0) {
      break;
    }
    printf("%d\n",fputs(receiveBuffer, file));
  }
  
  fclose(file);
  close(sockfd);
  return 0;
}

