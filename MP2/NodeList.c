#include "NodeList.h"

void init_list(node_list* list) {
  list->head = NULL;
  list->tail = NULL;
  list->length = 0;
}

void append_list(node_list* list, int node_number, int node_port) {
  node *new_node = malloc(sizeof(node));
  new_node->node_number = node_number;
  new_node->node_port = node_port;
  new_node->next = NULL;

  if (list->head == NULL) {
    list->head = new_node;
    list->tail = list->head;
  }
  else {
    list->tail->next = (struct node *) new_node;
    list->tail = new_node;
  }
  list->length++;
}

void print_list(node_list* list) {
  node *current_node = list->head;

  while (current_node != NULL) {
    printf("Node %d: %d\n", current_node->node_number, current_node->node_port);
    current_node = (node *) current_node->next;
  }
}

void destroy_list(node_list* list) {
  node *current_node = list->head;

  while (current_node != NULL) {
    node *node_to_remove = current_node;
    current_node = (node *) current_node->next;
    free(node_to_remove);
  }
}