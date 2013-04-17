#include "sender.h"


int run_sender(char* hostname, char *portno, char* filename)
{
  
  FILE *file;
  int bytes_read = 0, sockfd;
  char sendBuffer[MAX_PKTSIZE], receive_Buffer[MAX_PKTSIZE];
  sockfd = openUDPListenerSocket(port_number);

  file = fopen(filename, "r");

  while ((bytes_read = fread(sendBuffer,1, MAX_PKTSIZE-1, file)) != 0) 
  {
    sendUDPMessageTo(hostname, portno, sendBuffer, bytes_read);
    receiveUDPMessageAndPrint(sockfd, receive_Buffer, 0);
   
  }

  strcpy(sendBuffer, "DONE");

  sendUDPMessageTo(hostname, portno, sendBuffer, 4);

  close(sockfd);
  fclose(file);
	return 0;
}


