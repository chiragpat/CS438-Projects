#include "receiver.h"


int run_receiver(char* portno, char* filename){

  /*
   * Your implementation should be self-contained in this file, no other source
   * or header file will be considered by our autograder.
   */

  int sockfd, receive_sockfd = -1, num_bytes = 0, num_packs = -1, num_packs_received = 0;
  unsigned int old_check_sum1 = 0, old_check_sum2 = 0;
  char receiveBuffer[MAX_PKTSIZE] = "", sendBuffer[MAX_PKTSIZE];
  char hostname[MAX_PKTSIZE] = "";
  gethostname(hostname, MAX_PKTSIZE);
  char port_number[6];
  FILE *file;
  packet_t rcv_packet;
  file = fopen(filename, "w");
  sockfd = openUDPListenerSocket(portno);


  while(num_packs_received != num_packs) 
  {
    num_bytes = receiveUDPMessageAndPrint(sockfd, (char*)(&rcv_packet), 0);
    if(sizeof(handshake_t) == num_bytes)
    {

      handshake_t * handshake = (handshake_t *)&rcv_packet;
      
      old_check_sum1 = handshake->check_sum1;
      old_check_sum2 = handshake->check_sum2;
      if((old_check_sum2 == old_check_sum1) && (old_check_sum1 == handshake_check_sum(handshake)))
      {
        strcpy(hostname, handshake->hostname);
        strcpy(port_number, handshake->port);
        num_packs = handshake->num_packets;
        receive_sockfd = sendUDPMessageTo(hostname, port_number, (char *)handshake, num_bytes, receive_sockfd);
        printf("Number Packets: %d\n", num_packs);
      }
    }
    else 
    {
      old_check_sum1 = rcv_packet.check_sum1;
      old_check_sum2 = rcv_packet.check_sum2;
      if((old_check_sum2 == old_check_sum1) && (old_check_sum1 == check_sum(&rcv_packet, MAX_PKTSIZE)))
      {
        if(rcv_packet.pack_number == num_packs_received)
        {
          printf("Packet Number Received: %d\n", num_packs_received);
          fwrite(rcv_packet.buffer,1, rcv_packet.size - 16, file);
          num_packs_received++;
        }
        num_bytes = sprintf(sendBuffer, "ACK: %d", rcv_packet.pack_number);
        sendUDPMessageTo(hostname, port_number, sendBuffer, num_bytes, receive_sockfd);  
      }
    }
  
    
  }
  printf("Total Packets Received: %d\n", num_packs);
  fclose(file);
  mp3_close(receive_sockfd);
  mp3_close(sockfd);
  return 0;
}
unsigned int handshake_check_sum(handshake_t *handshake);
