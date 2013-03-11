#include "Djikstras.h"

int compare(void *key1, void *key2) {
  Node *node1 = (Node *) key1;
  Node *node2 = (Node *) key2;

  if (node1->distance > node2->distance) {
    return 1;
  }
  else if (node1->distance < node2->distance) {
    return -1;
  }
  else {
    return 0;
  }
}

void build_hop_table(NodeGraph* graph) {
  int num_routes, i, temp, j;
  Node *visited_nodes = malloc(graph->num_nodes*sizeof(Node));
  heap *unvisited_nodes = malloc(sizeof(heap));
  free(graph->routes);
  graph->routes = malloc(graph->num_nodes * sizeof(Hop));

  num_routes = 0;
  heap_create(unvisited_nodes, graph->num_nodes, compare);
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
    heap_insert(unvisited_nodes, &graph->nodes[i], NULL);
  }

  while (heap_size(unvisited_nodes) != 0) {
    Node *node;

    heap_delmin(unvisited_nodes, (void *) &node, (void *) &temp);
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

      if ( (link->cost != -1) && (contains(visited_nodes, num_routes, neigh_node) == -1)) {
        printf("%d:%d\n", node->node_number, neigh_node->node_number);
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
          heap_delmin(unvisited_nodes, (void *) &temp_node, (void *) &temp);
          heap_insert(unvisited_nodes, (void *) temp_node, NULL);
        }
      } 
    }

  }

  while (heap_size(unvisited_nodes) != 0) {
    Node *node;
    heap_delmin(unvisited_nodes, (void *) &node, (void *) &temp);
    graph->routes[num_routes].destination_node = node;
    graph->routes[num_routes].next_hop = (Node *) node->hop;
    graph->routes[num_routes].cost = node->distance;
    num_routes++;
  }

  heap_destroy(unvisited_nodes);
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
