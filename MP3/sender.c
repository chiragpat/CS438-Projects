#include "sender.h"
// #define WINDOW_SIZE 10

static int last_ack_pack;
static int num_retry ;
static int window_size = 10;
int run_sender(char* hostname, char *portno, char* filename)
{
  
  FILE *file;
  num_retry = 0;
  int i;
  int num_packs, bytes_read, sockfd, receive_sockfd = -1, rv = 0;
  struct stat file_stat;
  struct sockaddr_in sin;
  socklen_t len = sizeof(sin);
  packet_t packet;
  handshake_t handshake;
  struct timeval time_out, start, finish, send_finish, window;
  time_out.tv_sec = 1;
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
    gettimeofday(&send_finish, NULL);
    rv = wait_for_receive(sockfd, (char*)&handshake1, time_out, 1);
    gettimeofday(&finish, NULL);
  }
  
  window.tv_usec = (unsigned long)(send_finish.tv_usec - start.tv_usec);

  time_out.tv_sec = (unsigned long)(finish.tv_sec - start.tv_sec);
  time_out.tv_usec = (unsigned long)(finish.tv_usec - start.tv_usec);




  window_size = (int)((unsigned int)(time_out.tv_usec/(2*window.tv_usec)) % 1000 + 1);
  // printf("Window Size: %d\n", (int) (window_size));
  packet.pack_number = 0;
  rv = 0;
  last_ack_pack = -1;
  while (last_ack_pack + 1 < num_packs) 
  {
    packet.pack_number = last_ack_pack+1;
    fseek(file, packet.pack_number*((MAX_PKTSIZE-1)-16), SEEK_SET);
    for(i=0; i < window_size && ((bytes_read = fread(packet.buffer,1, (MAX_PKTSIZE-1)-16, file)) != 0); i++)
    {
      packet.size = bytes_read + 16;
      check_sum(&packet, bytes_read + 16);
      sendUDPMessageTo(hostname, portno, (char*)(&packet), MAX_PKTSIZE, receive_sockfd);
      packet.pack_number++;
    }

    if(num_retry < 10 * window_size || num_retry < 100)
        rv = wait_for_receive(sockfd, receive_Buffer, time_out, 0);  
    else
      break;
    
    if(last_ack_pack >= num_packs - window_size - 1)
    {
      num_retry++;        
    }

    rv = 0;
  }

  // printf("Packets to send: %d\n", num_packs);
  mp3_close(receive_sockfd);
  mp3_close(sockfd);
  fclose(file);
	return 0;
}

int wait_for_receive(int sockfd, char* receive_buffer, struct timeval tv, int handshake)
{
  fd_set select_fd;
  int rv, ret = 0;
  FD_ZERO(&select_fd);
  FD_SET(sockfd, &select_fd);
  int num_ack = 0;
  int bytes_received;
   while((rv = select(sockfd + 1, &select_fd, NULL, NULL, &tv)))
  {
    if(FD_ISSET(sockfd, &select_fd))
    {
      bytes_received = receiveUDPMessageAndPrint(sockfd, receive_buffer, 0);
      if(sizeof(ack_t) == bytes_received)
        
        if( (((ack_t *)receive_buffer)->check_sum1 == ((ack_t *)receive_buffer)->check_sum2) 
            && (((ack_t *)receive_buffer)->pack_number == last_ack_pack + 1))
        {
          last_ack_pack++;
          num_retry = 0;
          // printf("ACK: %d\n", ((ack_t *)receive_buffer)->pack_number);
        }
      ret = 1;
      if(handshake == 1)
        break;
      else
      {
        num_ack++;
        if(num_ack == window_size)
          break;
      }
    }
    
    FD_ZERO(&select_fd);
    FD_SET(sockfd, &select_fd);
  }

  return ret;
}
