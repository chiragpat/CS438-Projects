#include "sender.h"

int run_sender(char* hostname, char *portno, char* filename)
{
  
  FILE *file;

  int num_packs, bytes_read, sockfd, receive_sockfd = -1, rv = 0, num_retry = 0;
  struct stat file_stat;
  struct sockaddr_in sin;
  socklen_t len = sizeof(sin);
  packet_t packet;
  handshake_t handshake;
  struct timeval time_out, start, finish;
  time_out.tv_sec = 2;
  time_out.tv_usec = 0;

  char sendBuffer[MAX_PKTSIZE], receive_Buffer[MAX_PKTSIZE];
  sockfd = openUDPListenerSocket(NULL);

  file = fopen(filename, "r");
  fstat(fileno(file), &file_stat);

  num_packs = (int)(ceil(((float)file_stat.st_size)/(MAX_PKTSIZE - 1 - 16)));
  bytes_read = sprintf(sendBuffer, "Size: %d",num_packs);
  handshake_t handshake1;
  handshake1.num_packets = 0;
  strcpy(handshake1.port,"");
  strcpy(handshake1.hostname,"");
  handshake.num_packets = num_packs;
  gethostname(handshake.hostname, 50);
  getsockname(sockfd, (struct sockaddr*)&sin, &len);
  sprintf(handshake.port, "%d", ntohs(sin.sin_port));


  while(!rv)
  { 
    handshake_check_sum(&handshake);
    gettimeofday(&start, NULL);
    receive_sockfd = sendUDPMessageTo(hostname, portno, (char *)&handshake, sizeof(handshake_t), receive_sockfd);
    rv = wait_for_receive(sockfd, (char*)&handshake1, time_out);
    gettimeofday(&finish, NULL);
  }
  
  time_out.tv_sec = 2*(finish.tv_sec - start.tv_sec);
  time_out.tv_usec = 2*(finish.tv_usec - start.tv_usec);
  packet.pack_number = 0;
  rv = 0;
  while ((bytes_read = fread(packet.buffer,1, (MAX_PKTSIZE-1)-16, file)) != 0) 
  {
    while(!rv)
    {
      packet.size = bytes_read + 16;
      check_sum(&packet, bytes_read + 16);
      sendUDPMessageTo(hostname, portno, (char*)(&packet), MAX_PKTSIZE, receive_sockfd);
      if(num_retry <50)
        rv = wait_for_receive(sockfd, receive_Buffer, time_out);  
      else
        break;
      if(packet.pack_number == num_packs - 1)
      {
        num_retry++;        
      }
    }
    rv = 0;
    packet.pack_number++;
  }

  printf("Packets to send: %d\n", num_packs);
  mp3_close(receive_sockfd);
  mp3_close(sockfd);
  fclose(file);
	return 0;
}

int wait_for_receive(int sockfd, char* receive_buffer, struct timeval tv)
{
  fd_set select_fd;
  int rv, ret = 0;
  FD_ZERO(&select_fd);
  FD_SET(sockfd, &select_fd);

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
