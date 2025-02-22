#pragma once
#include <Arduino.h>
#include <LiquidCrystal.h>
#include "stack.h"
#include "GraphicsManager.h"

//Pw
#define MAX_PW_LENGTH 4
#define PW_0 0
#define PW_1 0
#define PW_2 0
#define PW_3 1

// Botoes
#define BT_NENHUM 0
#define BT_SELECT 1
#define BT_LEFT   2
#define BT_UP     3
#define BT_DOWN   4
#define BT_RIGHT  5

class StateManager {
  private:
    Node* currentNode;
    Stack menu_cursor_stack;//?
    int8_t cursor;//?
    //Node lista_estados[MAX_NUM_ESTADOS] //lista de estados ativos para executar
    GraphicsManager* p_ger_grafico; //pra notificar a flag pra limpar tela ou nao, ou pra chamar o clear, ou pra passar a lista pra printar

    ///desacoplar
    int16_t cont_counter;
    Stack pw; //C //se n for global precisaria alocar array?
    Cronometer cronometer;

    static StateManager* instance;


  public:
    StateManager();
    ~StateManager();
    static StateManager* getInstance();
    void botaoSolto(int8_t botao);
    void navigateBack();
    void navigateDown();
    void navigateUp();
    void selectNode();
    void executeButton();
    void execute();
};