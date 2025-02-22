#include "HardwareSerial.h"
#include "StateManager.h"

StateManager* StateManager::instance = nullptr;

StateManager::StateManager():
  cont_counter(0)
{
  currentNode = currentNode->initializeTree();

  p_ger_grafico = GraphicsManager::getInstance();
  //p_ger_grafico->setCurrentNode(currentNode);
}

StateManager::~StateManager()
{
  //delete p_ger_grafico; //onde deletar?
  p_ger_grafico = nullptr;
}

StateManager* StateManager::getInstance()
{
  if (instance == nullptr) // Se ainda não houver instância, cria uma nova
    instance = new StateManager();
  return instance;
}

void StateManager::navigateBack()
{
  //Volta pro no anterior
  if (currentNode->getParent() != nullptr)
  {
    p_ger_grafico->setLcdIsClean(false);
    currentNode = currentNode->getParent();
    currentNode->setCursor(currentNode->getMenuCursorStack().pop());
  } 
}

void StateManager::navigateUp()
{
  // Navegacao de teclado para senha
  if(currentNode->getParent()->getLabel() == String(F(">keyboard")))
  {
    int8_t num = currentNode->getLabel()[0] - '0';
    if (num > 0)
    {
      currentNode->setLabel(String(num-1));
      p_ger_grafico->setLcdIsClean(false);
    }
  }

  // Navegacao entre nos
  else if (currentNode->getCursor() > 0)
  {
    if (currentNode->getParent()->getChild(currentNode->getCursor()-1) != nullptr)
    {
      p_ger_grafico->setLcdIsClean(false);
      currentNode->setCursor(currentNode->getCursor()-1);
      currentNode = currentNode->getParent()->getChild(currentNode->getCursor());
    }
  }
}

void StateManager::navigateDown()
{
  // Navegacao de teclado para senha
  if(currentNode->getParent()->getLabel() == String(F(">keyboard")))
  {
    int8_t num = currentNode->getLabel()[0] - '0';
    if (num < 9)
    {
      currentNode->setLabel(String(num+1));
      p_ger_grafico->setLcdIsClean(false);
    }
  }

  // Navegacao entre nos
  else if (currentNode->getCursor() < currentNode->getParent()->getChildCount() - 1 and currentNode->getLabel() != F("omar@arduino:\\n/$_")) //nao pd ir para tras da raiz
    if (currentNode->getParent()->getChild(currentNode->getCursor()+1) != nullptr)
    {
      p_ger_grafico->setLcdIsClean(false);
      currentNode->setCursor(currentNode->getCursor()+1);
      currentNode = currentNode->getParent()->getChild(currentNode->getCursor());
    }
}

void StateManager::executeButton()
{
  // Funcionalidades (sem filhos)
  if (currentNode->getLabel() == String(F(">up")))
    cont_counter++;
  else if (currentNode->getLabel() == String(F(">down")))
    cont_counter--;
  else if (currentNode->getLabel() == String(F(">reset")) and currentNode->getParent()->getLabel() == String(F(">counter")))
    cont_counter=0;
  else if (currentNode->getLabel() == String(F(">start")))
    cronometer.setCronometerIsRunning(true);
  else if (currentNode->getLabel() == String(F(">pause")))
    cronometer.setCronometerIsRunning(false);
  else if (currentNode->getLabel() == String(F(">reset")) and currentNode->getParent()->getLabel() == String(F(">cronometer")))
    cronometer.reset();
  else if (currentNode->getLabel() == String(F(">logout")))
  {
    navigateBack();
    navigateBack();
  }
  else if (currentNode->getLabel() == String(F(">on")))
    PORTB|=(1<<PB2);//digitalWrite(PIN_BACK_LIGHT, HIGH);
  else if (currentNode->getLabel() == String(F(">off")))
    PORTB&=!(1<<PB2);//digitalWrite(PIN_BACK_LIGHT, LOW);
  else if (currentNode->getParent()->getLabel() == String(F(">keyboard")))
  {
    if(pw.getTop()+1 < MAX_PW_LENGTH)
    {
      int8_t num = currentNode->getLabel()[0] - '0';
      pw.push(num);
    }
  }
  else if (currentNode->getLabel() == String(F(">backspace")))
  {
    if(!pw.isEmpty())
    {
      pw.pop();
      p_ger_grafico->clearScreen();
    }
  }
}

void StateManager::selectNode()
{
  if (currentNode->getChildCount() > 0)
  {
    if (currentNode->getChild(0) != nullptr)
    {
      bool pass = false;

      // Verificacao de senha
      if (currentNode->getLabel() == String(F(">enter")))
      {
        
        if(pw.getTop()+1 == MAX_PW_LENGTH)
        {
          int8_t pw_3 = pw.pop();
          int8_t pw_2 = pw.pop();
          int8_t pw_1 = pw.pop();
          int8_t pw_0 = pw.pop();

          if(pw_0 == PW_0 and
             pw_1 == PW_1 and
             pw_2 == PW_2 and
             pw_3 == PW_3)
            pass = true;
        }
      }

      // Navegacao normal
      else
        pass = true;
      

      // Navega entre nos
      if(pass == true)
      {
        p_ger_grafico->setLcdIsClean(false);
        currentNode = currentNode->getChild(0);
        currentNode->getMenuCursorStack().push(currentNode->getCursor());
        currentNode->setCursor(0);
      }
    }      
  } 

  else
    executeButton();
}



void StateManager::botaoSolto(int8_t botao)
{
  if (botao == BT_DOWN)
    navigateDown();
  else if (botao == BT_UP)
    navigateUp();
  else if (botao == BT_SELECT)
    selectNode();
  else if (botao == BT_LEFT)
    navigateBack();
  else if (botao == BT_RIGHT)
    p_ger_grafico->toggleLight();
}

void StateManager::execute()
{
  cronometer.execute();
  p_ger_grafico->displayCurrentNode(currentNode, cont_counter, pw, cronometer);//desacoplar?


  //for i in lista_estados i->execute()
}