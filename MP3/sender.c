#include "sender.h"
#define time_out 20

int run_sender(char* hostname, char *portno, char* filename)
{
  
  FILE *file;

  int num_packs, bytes_read, sockfd, receive_sockfd = -1, rv = 0;
  struct stat file_stat;
  packet_t packet;
  char sendBuffer[MAX_PKTSIZE], receive_Buffer[MAX_PKTSIZE];
  sockfd = openUDPListenerSocket(port_number);



  file = fopen(filename, "r");
  fstat(fileno(file), &file_stat);

  num_packs = (int)(ceil(((float)file_stat.st_size)/(MAX_PKTSIZE - 1 - 4)));

  bytes_read = sprintf(sendBuffer, "Size: %d",num_packs);
  receive_sockfd = sendUDPMessageTo(hostname, portno, sendBuffer, bytes_read, receive_sockfd);
  wait_for_receive(sockfd, receive_Buffer, 100);
  packet.pack_number = 0;

  while ((bytes_read = fread(packet.buffer,1, (MAX_PKTSIZE-1)-4, file)) != 0) 
  {
    while(!rv)
    {
      sendUDPMessageTo(hostname, portno, (char*)(&packet), bytes_read + 4, receive_sockfd);
      rv = wait_for_receive(sockfd, receive_Buffer, time_out);
    }
    rv = 0;
    packet.pack_number++;
  }

  
  mp3_close(receive_sockfd);
  mp3_close(sockfd);
  fclose(file);
	return 0;
}

int wait_for_receive(int sockfd, char* receive_buffer, uint timeout)
{
  fd_set select_fd;
  int rv, ret = 0;
  struct timeval tv;
  FD_ZERO(&select_fd);
  FD_SET(sockfd, &select_fd);

  tv.tv_sec = 0;
  tv.tv_usec = timeout * 1000;

  while((rv = select(sockfd + 1, &select_fd, NULL, NULL, &tv)))
  {
    if(FD_ISSET(sockfd, &select_fd))
    {
      receiveUDPMessageAndPrint(sockfd, receive_buffer, 0);
      ret = 1;
      break;
    }
    
    FD_ZERO(&select_fd);
    FD_SET(sockfd, &select_fd);
  }

  return ret;
}
