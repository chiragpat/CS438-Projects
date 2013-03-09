#include "NodeGraph.h"

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