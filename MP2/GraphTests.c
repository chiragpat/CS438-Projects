#include "NodeGraph.h"
#include "Djikstras.h"

int main(int argc, char *argv[]){
  NodeGraph *graph = malloc(sizeof(NodeGraph));
  init_graph(graph, 1, 8080);
  add_node(graph, 2, 8081);
  add_link(graph, 1, 2, 10, NULL);
  add_link_for_new_node(graph, 1, 3, 8082, 20, NULL);
  add_link_for_new_node(graph, 1, 4, 8083, 9, NULL);
  add_link(graph, 2, 3, 5, NULL);
  add_link_for_new_node(graph, 4, 5, 8084, 1, NULL);
  add_link_for_new_node(graph, 5, 6, 8085, 11, NULL);
  print_graph(graph);
  build_hop_table(graph);
  print_graph(graph);
  edit_link(graph, 2, 3, -1, NULL);
  edit_link(graph, 4, 5, -1, NULL);
  build_hop_table(graph);
  print_graph(graph);
  destroy_graph(graph);
  free(graph);
  return 0;
}
