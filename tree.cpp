#include "tree.h"

Node::Node(String label):
label(label),
parent(nullptr),
childCount(0)
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

void Node::setLabel(String new_label) {label = new_label;}

String Node::getLabel() const {return label;}

Node* Node::getChild(int8_t child_n) const {return children[child_n];}

int8_t Node::getChildCount() const {return childCount;}

Node* Node::getParent() const {return parent;}

void Node::addChild(Node* parent, Node* child)
{
  if (parent != nullptr and child != nullptr)
  {
    parent->children[(parent->childCount)++] = child;
    child->parent = parent;
  }
}

Node* Node::initializeTree()
{
  Node* root = new Node(F("omar@arduino:\\n/$_")); //flash


  Node* login = new Node(F(">login"));
  addChild(root, login);

  Node* keyboard = new Node(F(">keyboard"));
  addChild(login, keyboard);
  addChild(keyboard, new Node(F("0")));
  addChild(login, new Node(F(">backspace")));
  Node* enter = new Node(F(">enter"));
  addChild(login, enter);



  Node* light = new Node(F(">lcd light"));
  addChild(root, light);
  addChild(light, new Node(F(">on")));
  addChild(light, new Node(F(">off")));


  Node* cronometer = new Node(F(">cronometer"));
  addChild(enter, cronometer);
  addChild(cronometer, new Node(F(">start")));
  addChild(cronometer, new Node(F(">pause")));
  addChild(cronometer, new Node(F(">reset")));


  Node* counter = new Node(F(">counter"));
  addChild(enter, counter);
  addChild(counter, new Node(F(">up")));
  addChild(counter, new Node(F(">down")));
  addChild(counter, new Node(F(">reset")));


  Node* system = new Node(F(">system"));
  addChild(enter, system);

  Node* board = new Node(F(">board"));
  addChild(system, board);
  addChild(board, new Node(F("model:\\nArduino UNO R3")));

  Node* ucontroller = new Node(F(">ucontroller"));
  addChild(system, ucontroller);
  addChild(ucontroller, new Node(F("model:\\nATmega328P")));
  addChild(ucontroller, new Node(F("CPU:\\n8-bit AVR (RISC)")));
  addChild(ucontroller, new Node(F("max speed:\\n20 MIPS @ 20 MHz")));
  addChild(ucontroller, new Node(F("flash:\\n32 KB")));
  addChild(ucontroller, new Node(F("SRAM:\\n2 KB")));
  addChild(ucontroller, new Node(F("EEPROM:\\n1 KB")));

  // Node* display_shield = new Node(F(">display"));
  // addChild(system, display_shield);
  // addChild(display_shield, new Node(F("shield:\\nLCD Keypad")));

  Node* firmware = new Node(F(">firmware"));
  addChild(system, firmware);
  addChild(firmware, new Node(F("language:\\nArduino C++")));
  addChild(firmware, new Node(F("compiler:\\navr-g++")));
  addChild(firmware, new Node(F("author:\\nOmar El Laden")));

  addChild(enter, new Node(F(">logout")));

  return root;
}