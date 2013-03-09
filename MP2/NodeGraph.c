#include "NodeGraph.h"

void init_graph(NodeGraph* graph, int my_node_number, int node_port) {
  graph->nodes_size = DEFAULTARRAYSIZE;
  graph->links_size = DEFAULTARRAYSIZE;
  graph->num_nodes = 0;
  graph->nodes = malloc(DEFAULTARRAYSIZE * sizeof(Node));
  graph->num_links = 0;
  graph->links = malloc(DEFAULTARRAYSIZE * sizeof(Link));

  add_node(graph, my_node_number, node_port);
  graph->my_node = &graph->nodes[0];
}

void add_node(NodeGraph* graph, int node_number, int node_port) {
  if (graph->nodes_size == graph->num_nodes) {
    graph->nodes = realloc(graph->nodes, 2*graph->nodes_size);
    graph->nodes_size = 2 * graph->nodes_size;
  }

  int num_nodes = graph->num_nodes;
  graph->nodes[num_nodes].node_number = node_number;
  graph->nodes[num_nodes].node_port = node_port;
  graph->nodes[num_nodes].num_neighbours = 0;
  graph->nodes[num_nodes].neighbours_size = DEFAULTARRAYSIZE;
  graph->nodes[num_nodes].neighbours = malloc(DEFAULTARRAYSIZE * sizeof(Node*));
  graph->nodes[num_nodes].neighbour_links = malloc(DEFAULTARRAYSIZE * sizeof(Link*));

  graph->num_nodes++;
}

Node* get_node(NodeGraph* graph, int node_number) {
  int i;

  for (i = 0; i < graph->num_nodes; i++) {
    if (graph->nodes[i].node_number == node_number) {
      return &graph->nodes[i];
    }
  }
  return NULL;
}

void add_link(NodeGraph* graph, int node0_number, int node1_number, int cost) {
  Node *node0 = get_node(graph, node0_number);
  Node *node1 = get_node(graph, node1_number);
  Link link;
  link.node0 = node0;
  link.node1 = node1;
  link.cost = cost;
  
  if (node0 != NULL && node1 != NULL) {
    if (graph->links_size == graph->num_links) {
      graph->links = realloc(graph->links, 2*graph->links_size);
      graph->links_size = 2 * graph->links_size;
    }

    graph->links[graph->num_links] = link;
    

    if (node0->neighbours_size == node0->num_neighbours) {
      node0->neighbours = realloc(node0->neighbours, 2*node0->neighbours_size);
      node0->neighbour_links = realloc(node0->neighbour_links, 2*node0->neighbours_size);
      node0->neighbours_size = 2 * node0->neighbours_size;
    }

    if (node1->neighbours_size == node1->num_neighbours) {
      node1->neighbours = realloc(node1->neighbours, 2*node1->neighbours_size);
      node1->neighbour_links = realloc(node1->neighbour_links, 2*node1->neighbours_size);
      node1->neighbours_size = 2 * node1->neighbours_size;
    }

    node0->neighbours[node0->num_neighbours] = (struct Node*) node1;
    node0->neighbour_links[node0->num_neighbours] = (struct Link*) &graph->links[graph->num_links];

    node1->neighbours[node1->num_neighbours] = (struct Node*) node0;
    node1->neighbour_links[node1->num_neighbours] = (struct Link*) &graph->links[graph->num_links];

    graph->num_links++;
    node0->num_neighbours++;
    node1->num_neighbours++;
  }
}

void add_link_for_new_node(NodeGraph* graph, int node0_number, int new_node_number, int new_node_port, int cost) {
  add_node(graph, new_node_number, new_node_port);
  add_link(graph, node0_number, new_node_number, cost);
}

Link* get_link(NodeGraph* graph, int node0_number, int node1_number) {
  int i;
  Node* node0 = get_node(graph, node0_number);

  for (i = 0; i < node0->num_neighbours; i++) {
    Node *node = (Node *) node0->neighbours[i];
    if (node->node_number == node1_number) {
      return (Link *)node0->neighbour_links[i];
    }
  }

  return NULL;
}

void edit_link(NodeGraph* graph, int node0_number, int node1_number, int new_cost) {
  Link *link = get_link(graph, node0_number, node1_number);
  link->cost = new_cost;
}

void print_graph(NodeGraph* graph) {
  int i, j;
  Node node;
  
  printf("My Node: %d addr %d port\n", graph->my_node->node_number, graph->my_node->node_port);

  printf("-------\nNodes\n");
  for (i = 0; i < graph->num_nodes; i++) {
    node = graph->nodes[i];
    printf("   Node: %d addr %d port\n", node.node_number, node.node_port);

    for (j = 0; j < node.num_neighbours; j++) {
      Node *neighbour = (Node *) node.neighbours[j];
      Link *neighbour_link = (Link *) node.neighbour_links[j];

      printf("\t %d --> %d\n", neighbour->node_number, neighbour_link->cost);
    }
  }
}

void destroy_graph(NodeGraph* graph) {
  int i, j;

  for (i = 0; i < graph->num_nodes; i++) {
    Node node = graph->nodes[i];
    free(node.neighbours);
    free(node.neighbour_links);
  }

  free(graph->nodes);
  free(graph->links);
}

/*
  Testing Graph Library
 */
int main(int argc, char *argv[]){
  NodeGraph *graph = malloc(sizeof(NodeGraph));
  init_graph(graph, 1, 8080);
  add_node(graph, 2, 8081);
  add_link(graph, 1, 2, 500);
  add_link_for_new_node(graph, 2, 3, 8082, 1000);
  print_graph(graph);
  edit_link(graph, 1, 2, -1);
  print_graph(graph);
  destroy_graph(graph);
  free(graph);
  return 0;
}