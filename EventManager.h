#pragma once
#include <Arduino.h>
#include <LiquidCrystal.h>

void botaoSolto(int8_t botao);/////////////////////////////

// Pin
#define PIN_BOTOES A0

// Botoes
#define BT_NENHUM 0
#define BT_SELECT 1
#define BT_LEFT   2
#define BT_UP     3
#define BT_DOWN   4
#define BT_RIGHT  5

// Tensoes dos botoes
#define SEL_THRESHOLD  800
#define LEFT_THRESHOLD 600
#define UP_THRESHOLD   400
#define DOWN_THRESHOLD 200
#define RIGHT_THRESHOLD 60

#define DEBOUNCE_TIME 50

class EventManager {
  private:
    int8_t estadoBotaoAnt;
    unsigned long bt_delay;
    //p_ger_estados //pra notificar o bt pressionado

  public:
    EventManager();
    ~EventManager();
    uint8_t checkButtonPress();
    void handleButtonPress(int8_t botao);
    void execute();
};