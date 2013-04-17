#include "receiver.h"

char hostname[10] = "127.0.0.1";
char ack[4] = "ACK";
int run_receiver(char* portno, char* filename){

  /*
   * Your implementation should be self-contained in this file, no other source
   * or header file will be considered by our autograder.
   */

  int sockfd, num_bytes = 0;
  char receiveBuffer[MAX_PKTSIZE] = "";
  FILE *file;

  file = fopen(filename, "w");
  sockfd = openUDPListenerSocket(portno);

  while(1) {
    num_bytes = receiveUDPMessageAndPrint(sockfd, receiveBuffer, 0);
    sendUDPMessageTo(hostname, port_number, ack, 3);
    
    if(strcmp(receiveBuffer, "DONE") == 0)
      break;
    else
      fwrite(receiveBuffer,1, num_bytes, file);
  }
  
  fclose(file);
  close(sockfd);
  return 0;
}

