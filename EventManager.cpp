#include "EventManager.h"

EventManager::EventManager():
  estadoBotaoAnt(BT_NENHUM),
  bt_delay(0)
{
  p_ger_estados = StateManager::getInstance();
}

EventManager::~EventManager()
{
  //delete p_ger_estados; //onde deletar?
  p_ger_estados = nullptr;
}

uint8_t EventManager::checkButtonPress()
{
  int16_t val_botoes = analogRead(PIN_BOTOES);

  int8_t botao = -1;
  if ((val_botoes < SEL_THRESHOLD) and (val_botoes >= LEFT_THRESHOLD))
    botao = (BT_SELECT);
  else if ((val_botoes < LEFT_THRESHOLD) and (val_botoes >= UP_THRESHOLD))
    botao = (BT_LEFT);
  else if ((val_botoes < UP_THRESHOLD) and (val_botoes >= DOWN_THRESHOLD))
    botao = (BT_DOWN);
  else if ((val_botoes < DOWN_THRESHOLD) and (val_botoes >= RIGHT_THRESHOLD))
    botao = (BT_UP);
  else if  (val_botoes < RIGHT_THRESHOLD)
    botao = (BT_RIGHT);
  else 
    botao = (BT_NENHUM);

  return botao;
}

void EventManager::handleButtonPress(int8_t botao)
{

  //Quando o botao for apertado ou solto
  if ((millis() - bt_delay) > DEBOUNCE_TIME)
  {
    // Apertado
    // if ((botao != BT_NENHUM) and (estadoBotaoAnt == BT_NENHUM) )
    // {
    //   p_ger_estados->botaoApertado(botao); 
    //   bt_delay = millis();
    // }

    // Solto
    if ((botao == BT_NENHUM) and (estadoBotaoAnt != BT_NENHUM) )
    {
      p_ger_estados->botaoSolto(estadoBotaoAnt); 
      bt_delay = millis();
    }
  }
  estadoBotaoAnt = botao;
}

void EventManager::execute()
{
  uint8_t bt_pressed = checkButtonPress();
  handleButtonPress(bt_pressed);
}

