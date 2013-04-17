#include "sender.h"


int run_sender(char* hostname, char *portno, char* filename)
{
  
  FILE *file;
  int bytes_read, sockfd, receive_sockfd;
  char sendBuffer[MAX_PKTSIZE], receive_Buffer[MAX_PKTSIZE];
  sockfd = openUDPListenerSocket(port_number);

  file = fopen(filename, "r");

  while ((bytes_read = fread(sendBuffer,1, MAX_PKTSIZE-1, file)) != 0) 
  {
    receive_sockfd = sendUDPMessageTo(hostname, portno, sendBuffer, bytes_read);
    receiveUDPMessageAndPrint(sockfd, receive_Buffer, 0);
    close(receive_sockfd); //wait for sender before closing sockfd and clearing queue
  }

  strcpy(sendBuffer, "DONE");
  receive_sockfd = sendUDPMessageTo(hostname, portno, sendBuffer, 4);
  
  /*acknowledging reciever knows transmission is done*/
  receiveUDPMessageAndPrint(sockfd, receive_Buffer, 0);
  receive_sockfd = sendUDPMessageTo(hostname, portno, sendBuffer, 4);
 
  sleep(1); //not sure how else to end it without closing the queue immediatly...
  close(receive_sockfd);
  close(sockfd);
  fclose(file);
	return 0;
}


