#include "sender.h"


int run_sender(char* hostname, char *portno, char* filename)
{
  
  FILE *file;

  int num_packs, bytes_read, sockfd, receive_sockfd = -1;
  struct stat file_stat;
  char sendBuffer[MAX_PKTSIZE], receive_Buffer[MAX_PKTSIZE];
  sockfd = openUDPListenerSocket(port_number);

  file = fopen(filename, "r");
  fstat(fileno(file), &file_stat);

  num_packs = (int)(ceil(((float)file_stat.st_size)/(MAX_PKTSIZE - 1)));

  bytes_read = sprintf(sendBuffer, "Size: %d",num_packs);
  receive_sockfd = sendUDPMessageTo(hostname, portno, sendBuffer, bytes_read, receive_sockfd);
  receiveUDPMessageAndPrint(sockfd, receive_Buffer, 0);

  while ((bytes_read = fread(sendBuffer,1, MAX_PKTSIZE-1, file)) != 0) 
  {
    sendUDPMessageTo(hostname, portno, sendBuffer, bytes_read, receive_sockfd);
    receiveUDPMessageAndPrint(sockfd, receive_Buffer, 0);
  }

  
  mp3_close(receive_sockfd);
  mp3_close(sockfd);
  fclose(file);
	return 0;
}


