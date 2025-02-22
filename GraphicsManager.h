#pragma once
#include <Arduino.h>
#include <LiquidCrystal.h>
#include "tree.h"

#include "cronometer.h"

// Pin
#define PIN_D4 4
#define PIN_D5 5
#define PIN_D6 6
#define PIN_D7 7
#define PIN_RS 8
#define PIN_EN 9
#define PIN_BACK_LIGHT 10

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