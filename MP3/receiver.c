#include "receiver.h"

char hostname[10] = "127.0.0.1";
int run_receiver(char* portno, char* filename){

  /*
   * Your implementation should be self-contained in this file, no other source
   * or header file will be considered by our autograder.
   */

  int sockfd, receive_sockfd = -1, num_bytes = 0, num_packs = 0, num_packs_received = 0;
  char receiveBuffer[MAX_PKTSIZE] = "", sendBuffer[MAX_PKTSIZE];
  FILE *file;
  packet_t rcv_packet;
  file = fopen(filename, "w");
  sockfd = openUDPListenerSocket(portno);

  num_bytes = receiveUDPMessageAndPrint(sockfd, receiveBuffer, 0);
  num_packs = atoi(receiveBuffer+5);
  num_bytes = sprintf(sendBuffer, "Size: %d", num_packs);

  receive_sockfd = sendUDPMessageTo(hostname, port_number, sendBuffer, num_bytes, receive_sockfd);  

  printf("Number Packets: %d\n", num_packs);
  while(num_packs_received != num_packs) 
  {
    num_bytes = receiveUDPMessageAndPrint(sockfd, (char*)(&rcv_packet), 0);
    if(strncmp((char*)(&rcv_packet), "Size: ", 6) == 0)
    {
      num_bytes = sprintf(sendBuffer, "Size: %d", num_packs);
      sendUDPMessageTo(hostname, port_number, sendBuffer, num_bytes, receive_sockfd);
    }
    else 
    {
      if(rcv_packet.pack_number == num_packs_received)
      {
        printf("%d\n", num_packs_received);
        fwrite(rcv_packet.buffer,1, num_bytes - 4, file);
        num_packs_received++;
      }

      num_bytes = sprintf(sendBuffer, "ACK: %d", rcv_packet.pack_number);
      sendUDPMessageTo(hostname, port_number, sendBuffer, num_bytes, receive_sockfd);  
    }
  
    
  }
  fclose(file);
  mp3_close(receive_sockfd);
  mp3_close(sockfd);
  return 0;
}

