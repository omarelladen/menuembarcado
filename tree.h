#pragma once
#include <Arduino.h>

struct Node {
  String label;
  String info;
  Node* children[7]; // Array de filhos 
  int8_t childCount; 
  Node* parent;
};

Node* initializeTree();
void addChild(Node* parent, Node* child);
Node* createNode(String label);