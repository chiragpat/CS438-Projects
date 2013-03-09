#ifndef NODEGRAPH_H
#define NODEGRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define MAXDATASIZE 1024
#define DEFAULTARRAYSIZE 50

typedef struct {
  int node_number;
  int node_port;
  int num_neighbours;
  int neighbours_size;
  struct Node **neighbours;
  struct Link **neighbour_links;
} Node;

typedef struct {
  Node *node0;
  Node *node1;
  int cost;
} Link;

typedef struct {
  Node *my_node;

  Node *nodes;
  int num_nodes;
  int nodes_size;

  Link *links;
  int num_links;
  int links_size;
} NodeGraph;

void init_graph(NodeGraph* graph, int my_node_number, int node_port);
void add_node(NodeGraph* graph, int node_number, int node_port);
Node* get_node(NodeGraph* graph, int node_number);
void add_link(NodeGraph* graph, int node0_number, int node1_number, int cost);
void add_link_for_new_node(NodeGraph* graph, int node0_number, int new_node_number, int new_node_port, int cost);
Link* get_link(NodeGraph* graph, int node0_number, int node1_number);
void edit_link(NodeGraph* graph, int node0_number, int node1_number, int new_cost);
void print_graph(NodeGraph* graph);
void destroy_graph(NodeGraph* graph);

#endif