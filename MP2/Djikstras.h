#ifndef DJIKSTRAS_H
#define DJIKSTRAS_H

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include "NodeGraph.h"

void build_hop_table(NodeGraph* graph);
Node* get_and_remove_min(Node **nodes, int num_nodes);

#endif