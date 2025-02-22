#pragma once
#include <Arduino.h>
#include <LiquidCrystal.h>
#include "tree.h"
#include "cronometer.h"
#include "defines.h"

class GraphicsManager {
  private:
    LiquidCrystal lcd;
    bool lcd_is_clean;
    //Node* currentNode;

    static GraphicsManager* instance;
    
    //lista dos utilitarios em execucao pra chamar seus metodos print

  public:
    GraphicsManager();
    ~GraphicsManager();
    static GraphicsManager* getInstance();
    void setLcdIsClean(bool value);
    void toggleLight();
    void clearScreen();
    void displayCurrentNode(Node* currentNode, int16_t cont_counter, Stack pw, Cronometer cronometer);
    void displayScreen() const;
    void execute();
};