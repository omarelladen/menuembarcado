#include "tree.h"

Node* createNode(String label)
{
  Node* newNode = new Node;
  newNode->label = label;
  newNode->childCount = 0;
  newNode->parent = nullptr;
  return newNode;
}
void addChild(Node* parent, Node* child)
{
  parent->children[(parent->childCount)++] = child;
  child->parent = parent;
}
Node* initializeTree()
{
  Node* root = createNode(F("omar@arduino:\\n/$_")); //flash
  //Node* root = createNode(String(str_root)); //sram


  Node* login = createNode(F(">login"));
  addChild(root, login);

  Node* light = createNode(F(">lcd light"));
  addChild(root, light);
  addChild(light, createNode(F(">on")));
  addChild(light, createNode(F(">off")));


  Node* cronometer = createNode(F(">cronometer"));
  addChild(login, cronometer);
  //addChild(timer, createNode(F(">up/down")));
  addChild(cronometer, createNode(F(">start")));
  addChild(cronometer, createNode(F(">pause")));
  addChild(cronometer, createNode(F(">reset")));


  Node* counter = createNode(F(">counter"));
  addChild(login, counter);
  addChild(counter, createNode(F(">up")));
  addChild(counter, createNode(F(">down")));
  addChild(counter, createNode(F(">reset")));


  Node* system = createNode(F(">system"));
  addChild(login, system);

  Node* board = createNode(F(">board"));
  addChild(system, board);
  addChild(board, createNode(F("model:\\nArduino UNO R3")));

  Node* ucontroller = createNode(F(">ucontroller"));
  addChild(system, ucontroller);
  addChild(ucontroller, createNode(F("model:\\nATmega328P")));
  addChild(ucontroller, createNode(F("CPU:\\n8-bit AVR (RISC)")));
  addChild(ucontroller, createNode(F("max speed:\\n20 MIPS @ 20 MHz")));
  addChild(ucontroller, createNode(F("flash:\\n32 KB")));
  addChild(ucontroller, createNode(F("SRAM:\\n2 KB")));
  addChild(ucontroller, createNode(F("EEPROM:\\n1 KB")));

  Node* display_shield = createNode(F(">display"));
  addChild(system, display_shield);
  addChild(display_shield, createNode(F("shield:\\nLCD Keypad")));

  Node* firmware = createNode(F(">firmware"));
  addChild(system, firmware);
  addChild(firmware, createNode(F("language:\\nArduino C++")));
  addChild(firmware, createNode(F("compiler:\\navr-g++")));
  addChild(firmware, createNode(F("author:\\nOmar El Laden")));

  addChild(login, createNode(F(">logout")));

  return root;
}