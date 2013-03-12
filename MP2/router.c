#include "router.h"

int main(int argc, char *argv[]){
  int sockfd, udpfd, addr, udpport, new_cost, rv, maxfd;
  int src, dest, numbytes;
  char *msg, *host;
  FILE* socket_file;
  char sendBuffer[MAXDATASIZE], receiveBuffer[MAXDATASIZE], destPort[MAXDATASIZE], receiveBuffer2[MAXDATASIZE];
  struct sockaddr_storage their_addr;
  struct timeval tv;
  socklen_t addr_len;
  addr_len = sizeof(their_addr);

  NodeGraph * nodegraph = malloc(sizeof(NodeGraph));
  fd_set fds;
  
  if (argc != 4) {
    fprintf(stderr,"usage: router hostname tcpport udpport\n");
    exit(1);
  }

  udpport = atoi(argv[3]);
  host = argv[1];
  sockfd = establishTCPConnection(argv[1], argv[2]);
  udpfd = openUDPListenerSocket(argv[3]);
  socket_file = fdopen(sockfd, "r");

  sendString(sockfd, "HELO\n");
  receiveAndPrint(sockfd, receiveBuffer, 0);
  addr = atoi(receiveBuffer+5);

  init_graph(nodegraph, addr, udpport);
  printf("manager replied with address %d\n", addr);

  sprintf(sendBuffer, "HOST localhost %d\n", udpport);
  sendString(sockfd, sendBuffer);
  receiveAndPrint(sockfd, receiveBuffer, 0);

  getAndSetupNeighbours(nodegraph, sockfd, socket_file);
  broadcastLinkInfo(nodegraph, udpfd);

  print_graph(nodegraph);
  
  sendString(sockfd, "READY\n");

  sendString(sockfd, "LOG ON\n");
  receiveAndPrint(sockfd, receiveBuffer, 1);

  FD_ZERO(&fds);
  FD_SET(sockfd, &fds);
  FD_SET(udpfd, &fds);

  maxfd = sockfd;
  if ( maxfd < udpfd ) {
    maxfd = udpfd;
  }

  while( (rv=select(maxfd+1, &fds, NULL, NULL, NULL)) >=0 ){

    if (FD_ISSET(sockfd, &fds)) {
      receiveOneLineAndPrint(socket_file, receiveBuffer, 1);

      if (strncmp(receiveBuffer, "LINKCOST", strlen("LINKCOST")) == 0) {
        LinkMessage message = updateNodeList(receiveBuffer, addr, nodegraph);
        broadcastOneLinkInfo(nodegraph, message, udpfd);
        sprintf(sendBuffer, "COST %d OK\n", message.cost);
        sendString(sockfd, sendBuffer);
        print_graph(nodegraph);
      }

      if (strcmp(receiveBuffer, "END\n") == 0) {
        sendString(sockfd, "BYE\n");
        break;
      }
    }

    if (FD_ISSET(udpfd, &fds)) {
      receiveUDPMessageAndPrint(udpfd, receiveBuffer, 0);

      int controlInt = (int)receiveBuffer[0];
      if (controlInt == 1) {
        dest = byteToInt(receiveBuffer+1);
        msg = receiveBuffer + 3;

        printf("Destination: %d, Message: %s\n", dest, msg);

        sprintf(sendBuffer, "LOG FWD %d %s\n", dest, msg);
        sendString(sockfd, sendBuffer);
        receiveAndPrint(sockfd, receiveBuffer2, 1);

        Link *link = get_link(nodegraph, addr, dest);
        Node *destinationNode = get_node(nodegraph, dest);

        if (destinationNode != NULL && link != NULL && link->cost != -1) {
          sprintf(destPort, "%d", destinationNode->node_port);
          memcpy(sendBuffer, receiveBuffer, (size_t) strlen(msg)+3);
          sendBuffer[0] = (char) 2;
          strcat(sendBuffer, "\n");
          sendUDPMessageTo("127.0.0.1", destPort, sendBuffer, strlen(msg)+3);
        }
      }
      else if (controlInt == 2) {
        msg = receiveBuffer + 3;

        printf("Message: %s\n", msg);
        sprintf(sendBuffer, "RECEIVED %s\n", msg);
        sendString(sockfd, sendBuffer);
      }
      else {
        LinkMessage message;
        memcpy(&message, receiveBuffer, sizeof(LinkMessage));
        printf("%d <--> %d : %d\n", message.node0_number, message.node1_number, message.cost);

        Link *link = get_link(nodegraph, message.node0_number, message.node1_number);

        if (link == NULL || link->cost != message.cost) {
          Node *node0 = get_node(nodegraph, message.node0_number);
          Node *node1 = get_node(nodegraph, message.node1_number);

          if (node0 == NULL) {
            add_link_for_new_node(nodegraph, message.node1_number, message.node0_number, message.node0_port, message.cost);
          }
          else if (node1 == NULL) {
            add_link_for_new_node(nodegraph, message.node0_number, message.node1_number, message.node1_port, message.cost);
          }
          else if (node0 == NULL && node1 == NULL) {
            add_node(nodegraph, message.node0_number, message.node0_port);
            add_link_for_new_node(nodegraph, message.node0_number, message.node1_number, message.node1_port, message.cost);
          }
          else {
            if (link) {
              edit_link(nodegraph, message.node0_number, message.node1_number, message.cost);
            }
            else {
              add_link(nodegraph, message.node0_number, message.node1_number, message.cost);
            }
            
          }

          broadcastOneLinkInfo(nodegraph, message, udpfd);
          print_graph(nodegraph);
        }
      }

    }

    FD_ZERO(&fds);
    FD_SET(sockfd,&fds);
    FD_SET(udpfd,&fds);
  }
  
  if (rv == -1) {
    perror("select()");   
    exit(1);
  }

  //Clean up
  close(sockfd);
  destroy_graph(nodegraph);
  free(nodegraph);
  return 0;
}

void getAndSetupNeighbours(NodeGraph* nodegraph, int sockfd, FILE* socket_file) {
  int ret, i, node_number, node_port, cost;
  char temp[MAXDATASIZE], receiveBuffer[MAXDATASIZE];
  char *tok;

  strcpy(receiveBuffer, "");
  sendString(sockfd, "NEIGH?\n");
  while ( strcmp(receiveBuffer, "DONE\n") != 0 ) {
    ret = receiveOneLineAndPrint(socket_file, receiveBuffer, 0);
    if (ret == 1 && strcmp(receiveBuffer, "DONE\n") != 0) {
      i = 0;
      strcpy(temp, receiveBuffer);
      tok = strtok(temp, " \n");
      while (tok != NULL) {
        if (i == 1) {
          node_number = atoi(tok);
        }

        if (i == 3) {
          node_port = atoi(tok);
        }

        if (i == 4) {
          cost = atoi(tok);  
        }
        tok = strtok(NULL, " \n");
        i++; 
      }
      add_link_for_new_node(nodegraph, nodegraph->my_node->node_number, node_number, node_port, cost);
    }
  }
}

LinkMessage updateNodeList(char receiveBuffer[MAXDATASIZE], int addr, NodeGraph *nodegraph){
  int i, first_node_number, second_node_number, node_number, new_cost;
  char temp[MAXDATASIZE];
  char *tok;
  LinkMessage message;

  i = 0;
  strcpy(temp, receiveBuffer);
  tok = strtok(temp, " \n");
  while(tok != NULL) {
    if (i == 1) {
      first_node_number = atoi(tok);
    }

    if (i == 2) {
      second_node_number = atoi(tok);
    }

    if (i == 3) {
      new_cost = atoi(tok);
    }
    tok = strtok(NULL, " \n");
    i++;
  }

  edit_link(nodegraph, first_node_number, second_node_number, new_cost);
  message.controlInt = 3;
  message.node0_number = first_node_number;
  message.node1_number = second_node_number;
  message.cost = new_cost;

  return message;
}

void broadcastLinkInfo(NodeGraph* graph, int udpfd) {
  int i, j;
  Node *my_node = graph->my_node;
  LinkMessage message;

  for (i = 0; i < my_node->num_neighbours; i++) {
    Node *node = (Node *) my_node->neighbours[i];
    Link *link = (Link *) my_node->neighbour_links[i];
    message.controlInt = 3;
    message.node0_number = my_node->node_number;
    message.node0_port = my_node->node_port;
    message.node1_number = node->node_number;
    message.node1_port = node->node_port;
    message.cost = link->cost;

    broadcastOneLinkInfo(graph, message, udpfd);
  }
}

void broadcastOneLinkInfo(NodeGraph* graph, LinkMessage message, int udpfd) {
  int i;
  char destPort[MAXDATASIZE];
  Node *my_node = graph->my_node;

  for (i = 0; i < my_node->num_neighbours; i++) {
    Node *node = (Node *) my_node->neighbours[i];
    sprintf(destPort, "%d", node->node_port);
    sendUDPMessageTo("127.0.0.1", destPort, (char *) &message, sizeof(LinkMessage));
  }
}

int byteToInt(char* p) {
  int result = 0;  // initialize;
  char* rv = (char*)&result;
  rv[0] = p[1];
  rv[1] = p[0];
  return result;
}
