#include "tree.h"

Stack Node::menu_cursor_stack;
int8_t Node::cursor=0;

Node::Node(String label):
label(label),
childCount(0),
parent(nullptr)
// next_sibling(nullptr),
// previous_sibling(nullptr)
{
  for(int i=0; i<N_MAX_CHILD; i++)
    children[i] = nullptr;
}

Node::~Node()
{
    for(int i=0;i<childCount;i++)
    {
      if(children[i] != nullptr)
      {
        delete children[i];
        children[i] = nullptr;
      }
    }
    childCount = 0;
}

Stack& Node::getMenuCursorStack() {return menu_cursor_stack;}

int8_t Node::getCursor() {return cursor;}

void Node::setCursor(int8_t _cursor)
{
  cursor = _cursor;
}


void Node::setLabel(String new_label) {label = new_label;}

String Node::getLabel() const {return label;}

Node* Node::getChild(int8_t child_n) const {return children[child_n];}

int8_t Node::getChildCount() const {return childCount;}

Node* Node::getParent() const {return parent;}

void Node::addChild(Node* child)
{
  if (child != nullptr)
  {
    children[childCount++] = child;
    child->parent = this;
  }
}

// void Node::setPreviousSibling(Node* prev_sib)
// {
//   if(prev_sib != nullptr)
//     previous_sibling = prev_sib;
// }
// void Node::setNextSibling(Node* next_sib)
// {
//   if(next_sib != nullptr)
//     next_sibling = next_sib;
// }

Node* Node::initializeTree()
{
  Node* root = new Node(F("omar@arduino:\\n/$_")); //flash


  Node* login = new Node(F(">login"));
  root->addChild(login);

  Node* keyboard = new Node(F(">keyboard"));
  login->addChild(keyboard);
  keyboard->addChild(new Node(F("0")));
  login->addChild(new Node(F(">backspace")));
  Node* enter = new Node(F(">enter"));
  login->addChild(enter);



  Node* light = new Node(F(">lcd light"));

  root->addChild(light);
  light->addChild(new Node(F(">on")));
  light->addChild(new Node(F(">off")));


  Node* cronometer = new Node(F(">cronometer"));
  enter->addChild(cronometer);
  cronometer->addChild(new Node(F(">start")));
  cronometer->addChild(new Node(F(">pause")));
  cronometer->addChild(new Node(F(">reset")));


  Node* counter = new Node(F(">counter"));
  enter->addChild(counter);
  counter->addChild(new Node(F(">up")));
  counter->addChild(new Node(F(">down")));
  counter->addChild(new Node(F(">reset")));


  Node* system = new Node(F(">system"));
  enter->addChild(system);

  Node* board = new Node(F(">board"));
  system->addChild(board);
  board->addChild(new Node(F("model:\\nArduino UNO R3")));

  Node* ucontroller = new Node(F(">ucontroller"));
  system->addChild(ucontroller);
  ucontroller->addChild(new Node(F("model:\\nATmega328P")));
  ucontroller->addChild(new Node(F("CPU:\\n8-bit AVR (RISC)")));
  ucontroller->addChild(new Node(F("max speed:\\n20 MIPS @ 20 MHz")));
  ucontroller->addChild(new Node(F("flash:\\n32 KB")));
  ucontroller->addChild(new Node(F("SRAM:\\n2 KB")));
  ucontroller->addChild(new Node(F("EEPROM:\\n1 KB")));

  // Node* display_shield = new Node(F(">display"));
  // addChild(system, display_shield);
  // addChild(display_shield, new Node(F("shield:\\nLCD Keypad")));

  Node* firmware = new Node(F(">firmware"));
  system->addChild(firmware);
  firmware->addChild(new Node(F("language:\\nArduino C++")));
  firmware->addChild(new Node(F("compiler:\\navr-g++")));
  firmware->addChild(new Node(F("author:\\nOmar El Laden")));

  enter->addChild(new Node(F(">logout")));

  return root;
}