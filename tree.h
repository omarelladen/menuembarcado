#pragma once
#include <Arduino.h>

#define MAX_N_CHILD 7


struct Node {
  String label;
  String info;
  Node* children[MAX_N_CHILD]; // Array de filhos 
  int8_t childCount; 
  Node* parent;
};

Node* initializeTree();
void addChild(Node* parent, Node* child);
Node* createNode(String label);