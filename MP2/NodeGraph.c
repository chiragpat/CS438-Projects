#include "NodeGraph.h"

void init_graph(NodeGraph* graph, int my_node_number, int node_port) {
  graph->nodes_size = DEFAULTARRAYSIZE;
  graph->links_size = DEFAULTARRAYSIZE;
  graph->num_nodes = 0;
  graph->nodes = (Node **) malloc(DEFAULTARRAYSIZE * sizeof(Node*));
  graph->num_links = 0;
  graph->links = (Link **) malloc(DEFAULTARRAYSIZE * sizeof(Link*));
  graph->routes = NULL;
  graph->run_djikstras = 1;

  add_node(graph, my_node_number, node_port);
  graph->my_node = graph->nodes[0];
}

void add_node(NodeGraph* graph, int node_number, int node_port) {
  if (graph->nodes_size == graph->num_nodes) {
    graph->nodes = realloc(graph->nodes, 2*graph->nodes_size*sizeof(Node*));
    graph->nodes_size *= 2;
  }

  int num_nodes = graph->num_nodes;
  graph->nodes[num_nodes] = (Node *) malloc(sizeof(Node));
  graph->nodes[num_nodes]->node_number = node_number;
  graph->nodes[num_nodes]->node_port = node_port;
  graph->nodes[num_nodes]->distance = -10000;
  graph->nodes[num_nodes]->previous = NULL;
  graph->nodes[num_nodes]->hop = NULL;
  graph->nodes[num_nodes]->num_neighbours = 0;
  graph->nodes[num_nodes]->neighbours_size = DEFAULTARRAYSIZE;
  graph->nodes[num_nodes]->neighbours = (struct Node**) malloc(DEFAULTARRAYSIZE * sizeof(struct Node*));
  graph->nodes[num_nodes]->neighbour_links = (struct Link**) malloc(DEFAULTARRAYSIZE * sizeof(struct Link*));
  graph->num_nodes++;
}

Node* get_node(NodeGraph* graph, int node_number) {
  int i;

  for (i = 0; i < graph->num_nodes; i++) {
    if (graph->nodes[i]->node_number == node_number) {
      return graph->nodes[i];
    }
  }
  return NULL;
}

void add_link(NodeGraph* graph, int node0_number, int node1_number, int cost, time_t *t) {
  Node *node0 = get_node(graph, node0_number);
  Node *node1 = get_node(graph, node1_number);

  if (node0 != NULL && node1 != NULL) {
    if (graph->links_size == graph->num_links) {
      graph->links = realloc(graph->links, 2*graph->links_size*sizeof(Link));
      graph->links_size = 2 * graph->links_size;
    }

    graph->links[graph->num_links] = (Link *) malloc(sizeof(Link));
    graph->links[graph->num_links]->node0 = node0;    
    graph->links[graph->num_links]->node1 = node1;    
    graph->links[graph->num_links]->cost = cost;

    if ( t == NULL ) {
      graph->links[graph->num_links]->t = time(NULL);
    } 
    else {
      graph->links[graph->num_links]->t = *t;  
    }   
    
    


    if (node0->neighbours_size == node0->num_neighbours) {
      node0->neighbours = realloc(node0->neighbours, 2*node0->neighbours_size*sizeof(struct Node*));
      node0->neighbour_links = realloc(node0->neighbour_links, 2*node0->neighbours_size*sizeof(struct Link*));
      node0->neighbours_size *= 2;
    }

    if (node1->neighbours_size == node1->num_neighbours) {
      node1->neighbours = realloc(node1->neighbours, 2*node1->neighbours_size*sizeof(struct Node*));
      node1->neighbour_links = realloc(node1->neighbour_links, 2*node1->neighbours_size*sizeof(struct Link*));
      node1->neighbours_size = 2 * node1->neighbours_size;
    }

    node0->neighbours[node0->num_neighbours] = (struct Node*) node1;
    node0->neighbour_links[node0->num_neighbours] = (struct Link*) graph->links[graph->num_links];

    node1->neighbours[node1->num_neighbours] = (struct Node*) node0;
    node1->neighbour_links[node1->num_neighbours] = (struct Link*) graph->links[graph->num_links];

    graph->num_links++;
    node0->num_neighbours++;
    node1->num_neighbours++;
  }

}

void add_link_for_new_node(NodeGraph* graph, int node0_number, int new_node_number, int new_node_port, int cost, time_t* t) {
  add_node(graph, new_node_number, new_node_port);
  add_link(graph, node0_number, new_node_number, cost, t);
}

Link* get_link(NodeGraph* graph, int node0_number, int node1_number) {
  int i;
  Node* node0 = get_node(graph, node0_number);

  for (i = 0; i < node0->num_neighbours; i++) {
    Node *node = (Node *) node0->neighbours[i];
    if (node->node_number == node1_number) {
      return (Link *) node0->neighbour_links[i];
    }
  }

  return NULL;
}

Link* edit_link(NodeGraph* graph, int node0_number, int node1_number, int new_cost, time_t* new_time) {
  Link *link = get_link(graph, node0_number, node1_number);

  if ( link != NULL ){
    link->cost = new_cost;
    if (new_time == NULL) {
      link->t = time(NULL);
    }
    else {
      link->t = *new_time;
    }
  }
}

void print_graph(NodeGraph* graph) {
  int i, j;
  Node node;
  
  printf("My Node: %d addr %d port\n", graph->my_node->node_number, graph->my_node->node_port);

  printf("-------\nNodes\n");
  for (i = 0; i < graph->num_nodes; i++) {
    node = *graph->nodes[i];
    int pre = -1;
    int hop = -1;
    if (node.previous != NULL) {
      Node *n = (Node *) node.previous;
      pre = n->node_number;
      n = (Node *) node.hop;
      hop = n->node_number;
    }
    printf("   Node: %d addr %d port %d previous %d hop\n", node.node_number, node.node_port, pre, hop);

    for (j = 0; j < node.num_neighbours; j++) {
      Node *neighbour = (Node *) node.neighbours[j];
      Link *neighbour_link = (Link *) node.neighbour_links[j];

      printf("\t %d --> %d\n", neighbour->node_number, neighbour_link->cost);
    }
  }

  printf("-------\nRoutes\n");

  if (graph->routes != NULL) {
    for (i = 0; i < graph->num_nodes; i++) {
      Node *destination = graph->routes[i].destination_node;
      Node *hop = graph->routes[i].next_hop;
      int hop_num = -1;
      if ( hop != NULL ){
        hop_num = hop->node_number;
      }
      printf("   Destination: %d via %d\n", destination->node_number, hop_num);
    }
  }

  printf("-------\n");
}

Node* get_hop(NodeGraph* graph, int destination_number) {
  int i;

  if ( graph->routes != NULL ) {
    for (i = 0; i < graph->num_nodes; i++) {
      Hop hop = graph->routes[i];
      if (hop.destination_node->node_number == destination_number) {
        return hop.next_hop;
      }
    }
  }
  return NULL;
}

void destroy_graph(NodeGraph* graph) {
  int i, j;

  for (i = 0; i < graph->num_nodes; i++) {
    Node *node = graph->nodes[i];
    free(node->neighbours);
    free(node->neighbour_links);
    free(node);
  }

  for (i = 0; i < graph->num_links; i++) {
    free(graph->links[i]);
  }
  
  free(graph->nodes);
  free(graph->links);
  free(graph->routes);
}
