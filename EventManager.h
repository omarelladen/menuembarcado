#pragma once
#include <Arduino.h>
#include <LiquidCrystal.h>
#include "StateManager.h"

void botaoSolto(int8_t botao);/////////////////////////////

// Pin
#define PIN_BOTOES A0


// Tensoes dos botoes
#define SEL_THRESHOLD   800
#define LEFT_THRESHOLD  600
#define UP_THRESHOLD    400
#define DOWN_THRESHOLD  200
#define RIGHT_THRESHOLD  60

#define DEBOUNCE_TIME 50

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
