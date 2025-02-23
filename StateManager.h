#pragma once
#include <Arduino.h>
#include <LiquidCrystal.h>
#include "stack.h"
#include "GraphicsManager.h"
#include "defines.h"

class StateManager {
  private:
    Node* currentNode;
   
    static StateManager* instance;
    
    GraphicsManager* p_ger_grafico; //pra notificar a flag pra limpar tela ou nao, ou pra chamar o clear, ou pra passar a lista pra printar

    //Node lista_estados[MAX_NUM_ESTADOS] //lista de estados ativos para executar
    
    ///desacoplar
    int16_t cont_counter;
    Stack pw; //C //se n for global precisaria alocar array?
    Cronometer cronometer;

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