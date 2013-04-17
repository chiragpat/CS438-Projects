#include "sender.h"

int run_sender(char* hostname, char *portno, char* filename) {

	/*
	 * Your implementation should be self-contained in this file, no other source
	 * or header file will be considered by our autograder.
	 */

  FILE *file;
  char sendBuffer[MAX_PKTSIZE];

  file = fopen(filename, "r");
  
  int count = 0;
  while(fgets(sendBuffer, MAX_PKTSIZE, file) != NULL) {
    sendUDPMessageTo(hostname, portno, sendBuffer, MAX_PKTSIZE);
    printf("%s", sendBuffer);
    count++;
  }  

  printf("%d\n", count);
  fclose(file);
	return 0;
}


