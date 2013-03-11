#include "NodeGraph.h"
#include "Djikstras.h"

int main(int argc, char *argv[]){
  NodeGraph *graph = malloc(sizeof(NodeGraph));
  init_graph(graph, 1, 8080);
  add_node(graph, 2, 8081);
  add_link(graph, 1, 2, 500);
  add_link_for_new_node(graph, 2, 3, 8082, 1000);
  add_link_for_new_node(graph, 3, 4, 8083, 10000);
  print_graph(graph);
  
  // print_graph(graph);

  build_hop_table(graph);
  print_graph(graph);
  edit_link(graph, 2, 3, -1);
  build_hop_table(graph);
  print_graph(graph);
  destroy_graph(graph);
  free(graph);
  return 0;
}