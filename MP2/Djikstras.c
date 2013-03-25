#include "Djikstras.h"

void build_hop_table(NodeGraph* graph) {
  int num_routes, i, temp, j;
  Node *visited_nodes = malloc(graph->num_nodes*sizeof(Node));
  Node **unvisited_nodes = malloc(graph->num_nodes*sizeof(Node*));
  int num_unvisited_nodes = 0;
  free(graph->routes);
  graph->routes = malloc(graph->num_nodes * sizeof(Hop));
  num_routes = 0;

  for (i = 0; i < graph->num_nodes; i++) {
    if (graph->nodes[i] == graph->my_node) {
      graph->nodes[i]->distance = 0;
      graph->nodes[i]->previous = NULL;
      graph->nodes[i]->hop = NULL;
    }
    else {
      graph->nodes[i]->distance = INT_MAX;
      graph->nodes[i]->previous = NULL;
      graph->nodes[i]->hop = NULL;
    }
    unvisited_nodes[num_unvisited_nodes] = graph->nodes[i];
    num_unvisited_nodes++;
  }

  while (num_unvisited_nodes != 0) {  
    Node *node = get_and_remove_min(unvisited_nodes, num_unvisited_nodes);
    num_unvisited_nodes--;

    visited_nodes[num_routes] = *node;
    graph->routes[num_routes].destination_node = node;
    graph->routes[num_routes].next_hop = (Node *) node->hop;
    graph->routes[num_routes].cost = node->distance;
    num_routes++;

    if (node->distance == INT_MAX) {
      break;
    }

    for (j = 0; j < node->num_neighbours; j++) {
      Node *neigh_node = (Node *) node->neighbours[j];
      Link *link = (Link *) node->neighbour_links[j];
      
      if ( (link->cost != -1) && (contains(visited_nodes, num_routes, neigh_node) == -1) ) {
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
        }
      }

    }

  }

  while (num_unvisited_nodes != 0) {
    Node *node = get_and_remove_min(unvisited_nodes, num_unvisited_nodes);
    num_unvisited_nodes--;
    
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

Node* get_and_remove_min(Node **nodes, int num_nodes) {
  Node* min = NULL;
  int i, min_index = 0;

  for( i = 0; i < num_nodes; i++) {
    if (!min) {
      min = nodes[i];
      min_index = i;
    }
    else {
      if(nodes[i]->distance < min->distance) {
        min = nodes[i];
        min_index = i;
      }
    }
  }

  for( i = min_index; i < num_nodes - 1; i++ ) {
    nodes[i] = nodes[i+1];
  }
  return min;
}

int get_shortest_path(NodeGraph *nodegraph, int source, int destination, char * path)
{
  int i = 0;
  int j;
  Node* curr_node =  get_node(nodegraph, destination);
  char temp_path[1024];
  //temp_path[0] = destination;
  while(curr_node->node_number != source || curr_node->previous != NULL)
  {
    temp_path[i] = (char)(curr_node->node_number);
    curr_node = curr_node->previous;

    i++;
  }
  path[0] = i;
  
  for(j = i-1; j>=0; j--)
    path[i-j] = temp_path[j];
  path[i]= destination;
  
  path[i+1] = '\0';

  return i;
}
