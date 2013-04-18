#include "receiver.h"

char hostname[10] = "127.0.0.1";
char ack[4] = "ACK";
int run_receiver(char* portno, char* filename){

  /*
   * Your implementation should be self-contained in this file, no other source
   * or header file will be considered by our autograder.
   */

  int sockfd, receive_sockfd = -1, num_bytes = 0, num_packs = 0;
  char receiveBuffer[MAX_PKTSIZE] = "";
  FILE *file;
  packet_t rcv_packet;
  file = fopen(filename, "w");
  sockfd = openUDPListenerSocket(portno);

  num_bytes = receiveUDPMessageAndPrint(sockfd, receiveBuffer, 0);
  num_packs = atoi(receiveBuffer+5);
  receive_sockfd = sendUDPMessageTo(hostname, port_number, ack, 3, receive_sockfd);  

  while(num_packs != 0) 
  {
    num_bytes = receiveUDPMessageAndPrint(sockfd, (char*)(&rcv_packet), 0);
    receive_sockfd = sendUDPMessageTo(hostname, port_number, ack, 3, receive_sockfd);  
    fwrite(rcv_packet.buffer,1, num_bytes - 4, file);
    num_packs--;
  }
  fclose(file);
  mp3_close(receive_sockfd);
  mp3_close(sockfd);
  return 0;
}

