#pragma once
#include <WString.h>

#define N_MAX_CHILD 7

class Node {
  private:
    String label;
    Node* children[N_MAX_CHILD]; // Array de filhos 
    int8_t childCount; 
    Node* parent;

  public:
    Node(String label);
    ~Node();
    void setLabel(String new_label);
    String getLabel() const;
    Node* getChild(int8_t child_n) const;
    int8_t getChildCount() const;
    Node* getParent() const;
    Node* initializeTree();
    void addChild(Node* parent, Node* child);
};