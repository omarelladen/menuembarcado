#pragma once
#include <WString.h>
#include "stack.h"
#include "defines.h"

class Node {
  private:
    static Stack menu_cursor_stack;
    static int8_t cursor;

    String label;
    Node* children[N_MAX_CHILD]; // Array de filhos 
    int8_t childCount; 
    Node* parent;

    // Node* previous_sibling;
    // Node* next_sibling;

  public:
    Node(String label);
    ~Node();

    static Stack& getMenuCursorStack();
    static int8_t getCursor();
    static void setCursor(int8_t _cursor);

    void setLabel(String new_label);
    String getLabel() const;
    Node* getChild(int8_t child_n) const;
    int8_t getChildCount() const;
    Node* getParent() const;
    Node* initializeTree();
    void addChild(Node* child);

    // void setPreviousSibling(Node* prev_sib);
    // void setNextSibling(Node* next_sib);
};