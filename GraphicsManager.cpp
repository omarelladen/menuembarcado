#include "GraphicsManager.h"

GraphicsManager* GraphicsManager::instance = nullptr;

GraphicsManager::GraphicsManager():
  lcd(PIN_RS,PIN_EN,PIN_D4,PIN_D5,PIN_D6,PIN_D7),
  lcd_is_clean(false)
{
  //lcd = LiquidCrystal(PIN_RS,PIN_EN,PIN_D4,PIN_D5,PIN_D6,PIN_D7);
  lcd.begin(16, 2);
}
GraphicsManager::~GraphicsManager()
{
}

GraphicsManager* GraphicsManager::getInstance()
{
  if (instance == nullptr) // Se ainda não houver instância, cria uma nova
    instance = new GraphicsManager();
  return instance;
}

void GraphicsManager::setLcdIsClean(bool value)
{
  lcd_is_clean = value;
}

void GraphicsManager::toggleLight()
{
  if (PORTB & (1 << PB2))// PB2 = pino digital 10 (PIN_BACK_LIGHT)
    PORTB&=!(1<<PB2);//digitalWrite(PIN_BACK_LIGHT, LOW);
  else
    PORTB|=(1<<PB2);//digitalWrite(PIN_BACK_LIGHT, HIGH);
}

void GraphicsManager::clearScreen()
{
  lcd.clear();
  lcd_is_clean = true;
}

void GraphicsManager::displayCurrentNode(Node* currentNode, int16_t cont_counter, Stack pw, Cronometer cronometer)
{
  if(currentNode != nullptr)
  {
    if (lcd_is_clean == false)
    {
      lcd.clear();
      lcd_is_clean = true;
    }


    String text = currentNode->getLabel();
    
    int8_t loc_bar_n = -1;
    int8_t i=0;
    for (i=0; i<text.length(); i++)
      if (text[i] == '\\')
        if(text[i+1] == 'n' and i<text.length())
          loc_bar_n = i;

    if (loc_bar_n != -1) // se tem \n
    {
        String line1 = currentNode->getLabel().substring(0, loc_bar_n);// Parte antes do \n
        String line2 = currentNode->getLabel().substring(loc_bar_n+2, i);// Parte depois do \n
        lcd.setCursor(0, 0);
        lcd.print(line1);
        lcd.setCursor(0, 1);
        lcd.print(line2);
    }
    else // Se não tiver \n, exibe tudo na primeira linha
    {
        // Opcao atual
        lcd.setCursor(0, 0);
        lcd.print(currentNode->getLabel());

        // Proxima opcao, se existir
        if (currentNode->getParent()->getChildCount() > Node::getCursor()+1)
        {
          if (currentNode->getParent()->getChild(Node::getCursor()+1) != nullptr)
          {
            String label = currentNode->getParent()->getChild(Node::getCursor()+1)->getLabel();
            label.setCharAt(0, ' ');       
            lcd.setCursor(0, 1);
            lcd.print(label); 
          }
        }
    }

    // Prints de conteudos especificos de menus

    if (currentNode->getParent()->getLabel() == String(F(">cronometer")))// or currentNode->getLabel() == String(F(">cronometer")))
      cronometer.printCronometer(lcd);

    else if (currentNode->getParent()->getLabel() == String(F(">counter")))
    {
      uint8_t pos_cursor = 13;
      if (cont_counter >= 0)
        pos_cursor += (cont_counter < 100) + (cont_counter < 10);
      else
        pos_cursor += (cont_counter > -100) + (cont_counter > -10) - 1;
      lcd.setCursor(pos_cursor, 0);
      lcd.print(cont_counter);
    }
    else if (currentNode->getParent()->getLabel() == String(F(">keyboard")) or currentNode->getParent()->getLabel() == String(F(">login")))
    {
      for(int8_t i=0;i<pw.getTop()+1;i++)
      {
        lcd.setCursor(15-i, 0);
        lcd.print(F("*"));
      }
    }
  }
}

void GraphicsManager::displayScreen() const
{
   //displayCurrentNode(); //menu em si
   //lista_estados.display();  //prints de funcionalidades especificas
}

void GraphicsManager::execute()
{
  // if (lcd_is_clean == false)
  //   clearScreen();

  // displayScreen();
}