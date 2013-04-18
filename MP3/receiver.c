#include "receiver.h"

char hostname[10] = "127.0.0.1";
char ack[4] = "ACK";
int run_receiver(char* portno, char* filename){

  /*
   * Your implementation should be self-contained in this file, no other source
   * or header file will be considered by our autograder.
   */

  int sockfd, receive_sockfd, num_bytes = 0;
  char receiveBuffer[MAX_PKTSIZE] = "";
  FILE *file;

  file = fopen(filename, "w");
  sockfd = openUDPListenerSocket(portno);

  num_bytes = receiveUDPMessageAndPrint(sockfd, receiveBuffer, 0);
  fwrite(receiveBuffer,1, num_bytes, file);

  while(1) 
  {
    receive_sockfd = sendUDPMessageTo(hostname, port_number, ack, 3);  
    num_bytes = receiveUDPMessageAndPrint(sockfd, receiveBuffer, 0);
    
    if(strcmp(receiveBuffer, "DONE") == 0)
    {
      /*acknowledging transmission is done*/
      receive_sockfd = sendUDPMessageTo(hostname, port_number, ack, 3);
      break;
    }
    else
      fwrite(receiveBuffer,1, num_bytes, file);
  }
  
  fclose(file);
  mp3_close(receive_sockfd);
  mp3_close(sockfd);
  return 0;
}

