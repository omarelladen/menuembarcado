#pragma once
#include <Arduino.h>
#include <LiquidCrystal.h>
#include "StateManager.h"
#include "defines.h"

class EventManager {
  private:
    int8_t estadoBotaoAnt;
    unsigned long bt_delay;
    StateManager* p_ger_estados; //pra notificar o bt pressionado

  public:
    EventManager();
    ~EventManager();
    uint8_t checkButtonPress();
    void handleButtonPress(int8_t botao);
    void execute();
};
