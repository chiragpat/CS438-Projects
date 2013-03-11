#ifndef DJIKSTRAS_H
#define DJIKSTRAS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include "NodeGraph.h"
#include "heap.h"

int compare(void *key1, void *key2);
void build_hop_table(NodeGraph* graph);

#endif