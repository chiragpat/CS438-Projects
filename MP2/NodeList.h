#ifndef NODELIST_H
#define NODELIST_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

typedef struct  {
  int node_number;
  int node_port;
  struct node *next;
} node;

typedef struct {
  node *head;
  node *tail;
  int length;
} node_list;

void init_list(node_list* list);
void append_list(node_list* list, int node_number, int node_port);
void print_list(node_list* list);
void destroy_list(node_list* list);

#endif