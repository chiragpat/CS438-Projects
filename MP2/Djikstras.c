#include "Djikstras.h"
#include "heap.h"

int compare(struct heap_node* a, struct heap_node* b) {
  Node *node1 = (Node *) a->value;
  Node *node2 = (Node *) b->value;

  if (node1->distance < node2->distance) {
    return 1;
  }
  else {
    return 0;
  }

}

struct heap_node * get_heap_node(struct heap_node *nodes, Node *node_to_find, int num_nodes) {
  int i;
  for ( i = 0; i < num_nodes; i++ ) {
    if (nodes[i].value == node_to_find) {
      return &nodes[i];
    }
  }
  return NULL;  
}

void build_hop_table(NodeGraph* graph) {
  int num_routes, i, temp, j;
  Node *visited_nodes = malloc(graph->num_nodes*sizeof(Node));
  struct heap *unvisited_nodes = malloc(sizeof(struct heap));
  struct heap_node unvisited_heap_nodes[graph->num_nodes];
  free(graph->routes);
  graph->routes = malloc(graph->num_nodes * sizeof(Hop));

  num_routes = 0;
  heap_init(unvisited_nodes);
  for (i = 0; i < graph->num_nodes; i++) {
    if (&graph->nodes[i] == graph->my_node) {
      graph->nodes[i].distance = 0;
      graph->nodes[i].previous = NULL;
      graph->nodes[i].hop = NULL;
    }
    else {
      graph->nodes[i].distance = INT_MAX;
      graph->nodes[i].previous = NULL;
      graph->nodes[i].hop = NULL;
    }
    heap_node_init(&unvisited_heap_nodes[i], &graph->nodes[i]);
    heap_insert(compare, unvisited_nodes, &unvisited_heap_nodes[i]);
  }

  while (!heap_empty(unvisited_nodes)) {
    Node *node;

    struct heap_node* h_node = heap_take(compare, unvisited_nodes);
    node = h_node->value;
    visited_nodes[num_routes] = *node;
    
    graph->routes[num_routes].destination_node = node;
    graph->routes[num_routes].next_hop = (Node *) node->hop;
    graph->routes[num_routes].cost = node->distance;
    num_routes++;

    if (node->distance == INT_MAX) {
      break;
    }

    for (j = 0; j < node->num_neighbours; j++) {
      Node *temp_node;
      Node *neigh_node = (Node *) node->neighbours[j];
      Link *link = (Link *) node->neighbour_links[j];
      printf("%d:%d\n", node->node_number, neigh_node->node_number);
      struct heap_node* neigh_h_node = get_heap_node(unvisited_heap_nodes, neigh_node, graph->num_nodes);
      if ( (link->cost != -1) && (heap_node_in_heap(neigh_h_node))) {
        
        int altdist = node->distance + link->cost;
        if (altdist < neigh_node->distance) {
          if (node->hop == NULL) {
            neigh_node->hop = (struct Node*) neigh_node;
          }
          else {
            neigh_node->hop = (struct Node*) node->hop;
          }
          neigh_node->distance = altdist;
          neigh_node->previous = (struct Node*) node;
          heap_decrease(compare, unvisited_nodes, neigh_h_node);
        }
      }

    }

  }

  while (!heap_empty(unvisited_nodes)) {
    Node *node;
    struct heap_node* h_node = heap_take(compare, unvisited_nodes);
    node = h_node->value;
    graph->routes[num_routes].destination_node = node;
    graph->routes[num_routes].next_hop = (Node *) node->hop;
    graph->routes[num_routes].cost = node->distance;
    num_routes++;
  }

  
  free(visited_nodes);
  free(unvisited_nodes);
}

int contains(Node *visited_nodes, int num_nodes, Node *node) {
  int i;
  for ( i = 0; i < num_nodes; i++) {
    if (visited_nodes[i].node_number == node->node_number) {
      return 1;
    }
  }
  return -1;
}
