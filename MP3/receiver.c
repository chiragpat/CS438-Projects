#include "receiver.h"


int run_receiver(char* portno, char* filename){

  /*
   * Your implementation should be self-contained in this file, no other source
   * or header file will be considered by our autograder.
   */

  int sockfd, receive_sockfd = -1, num_bytes = 0, num_packs = -1, expected_packet_number = 0;
  unsigned int old_check_sum1 = 0, old_check_sum2 = 0;
  char receiveBuffer[MAX_PKTSIZE] = "", sendBuffer[MAX_PKTSIZE];
  char hostname[MAX_PKTSIZE] = "";
  gethostname(hostname, MAX_PKTSIZE);
  char port_number[6];
  FILE *file;
  packet_t rcv_packet;
  file = fopen(filename, "w");
  sockfd = openUDPListenerSocket(portno);
  ack_t ack;

  while(expected_packet_number != num_packs) 
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
      }
    }
    else 
    {
      old_check_sum1 = rcv_packet.check_sum1;
      old_check_sum2 = rcv_packet.check_sum2;
      if((old_check_sum2 == old_check_sum1) && (old_check_sum1 == check_sum(&rcv_packet, MAX_PKTSIZE)))
      {
        if(rcv_packet.pack_number == expected_packet_number)
        {
          // printf("Packet Number Received: %d\n", expected_packet_number);
          fwrite(rcv_packet.buffer,1, rcv_packet.size - 16, file);
          expected_packet_number++;
        }
        ack.pack_number = rcv_packet.pack_number;
        ack_check_sum(&ack);
        sendUDPMessageTo(hostname, port_number, (char *)&ack, sizeof(ack_t), receive_sockfd);  
      }
    }
  
    
  }
  // printf("Total Packets Received: %d\n", num_packs);
  fclose(file);
  mp3_close(receive_sockfd);
  mp3_close(sockfd);
  return 0;
}

