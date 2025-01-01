//#include <avr/pgmspace.h>
#include "basic_lcd_shield.h"
#include "cronometer.h"
#include "stack.h"
#include "tree.h"

#include "EventManager.h"

// const char         str_root[] = "omar@arduino:\\n/$_"
// const char PROGMEM str_root[] = "omar@arduino:\\n/$_";
// String readProgmemString(const char* progmemStr) {
//  size_t len = strlen_P(progmemStr);// Calcula o tamanho da string em PROGMEM
//  char buffer[len + 1];// Cria um buffer temporario na SRAM para copiar a string
//  strcpy_P(buffer, progmemStr);// Copia a string da memoria Flash para o buffer na SRAM
//  return String(buffer);// Converte o buffer para um objeto String e retorna
// }


///add mais classes

///buscar ter menos var global
static LiquidCrystal lcd(PIN_RS,PIN_EN,PIN_D4,PIN_D5,PIN_D6,PIN_D7);///GGr
static Node* g_currentNode = g_currentNode->initializeTree();///GEs
static Cronometer g_cronometer;///C
static int16_t g_cont=0;///C
static bool g_lcd_is_clean=false;///GGr
static Stack g_pw;//C //se n for global precisaria alocar array?

static EventManager ger_eventos;

///
void setup()
{
  lcd.begin(16, 2);

  // Luz
  DDRB|=(1<<DDB2);//pinMode(PIN_BACK_LIGHT, OUTPUT);
  PORTB|=(1<<PB2);//digitalWrite(PIN_BACK_LIGHT, HIGH);

  //Serial.begin(9600);
}

//OK, mas desacoplar
void toggleLight()
{
  if (PORTB & (1 << PB2))// PB2 = pino digital 10 (PIN_BACK_LIGHT)
    PORTB&=!(1<<PB2);//digitalWrite(PIN_BACK_LIGHT, LOW);
  else
    PORTB|=(1<<PB2);//digitalWrite(PIN_BACK_LIGHT, HIGH);
}

///OK por enquanto
void navigateBack()
{
  //Volta pro no anterior
  if (g_currentNode->getParent() != nullptr)
  {
    g_lcd_is_clean=false;
    g_currentNode = g_currentNode->getParent();
    Node::setCursor(Node::getMenuCursorStack().pop());
  } 
}

//desacoplar
void navigateUp()
{
  // Navegacao de teclado para senha
  if(g_currentNode->getParent()->getLabel() == String(F(">keyboard")))
  {
    int8_t num = g_currentNode->getLabel()[0] - '0';
    if (num > 0)
    {
      g_currentNode->setLabel(String(num-1));
      g_lcd_is_clean=false;
    }
  }

  // Navegacao entre nos
  else if (Node::getCursor() > 0)
  {
    if (g_currentNode->getParent()->getChild(Node::getCursor()-1) != nullptr)
    {
      g_lcd_is_clean=false;
      Node::setCursor(Node::getCursor()-1);
      g_currentNode = g_currentNode->getParent()->getChild(Node::getCursor());
    }
  }
}

///desacoplar
void navigateDown()
{
  // Navegacao de teclado para senha
  if(g_currentNode->getParent()->getLabel() == String(F(">keyboard")))
  {
    int8_t num = g_currentNode->getLabel()[0] - '0';
    if (num < 9)
    {
      g_currentNode->setLabel(String(num+1));
      g_lcd_is_clean=false;
    }
  }

  // Navegacao entre nos
  else if (Node::getCursor() < g_currentNode->getParent()->getChildCount() - 1 and g_currentNode->getLabel() != F("omar@arduino:\\n/$_")) //nao pd ir para tras da raiz
    if (g_currentNode->getParent()->getChild(Node::getCursor()+1) != nullptr)
    {
      g_lcd_is_clean=false;
      Node::setCursor(Node::getCursor()+1);
      g_currentNode = g_currentNode->getParent()->getChild(Node::getCursor());
    }
}

void executeButton()
{
  // Funcionalidades (sem filhos)
  if (g_currentNode->getLabel() == String(F(">up")))
    g_cont++;
  else if (g_currentNode->getLabel() == String(F(">down")))
    g_cont--;
  else if (g_currentNode->getLabel() == String(F(">reset")) and g_currentNode->getParent()->getLabel() == String(F(">counter")))
    g_cont=0;
  else if (g_currentNode->getLabel() == String(F(">start")))
    g_cronometer.setCronometerIsRunning(true);
  else if (g_currentNode->getLabel() == String(F(">pause")))
    g_cronometer.setCronometerIsRunning(false);
  else if (g_currentNode->getLabel() == String(F(">reset")) and g_currentNode->getParent()->getLabel() == String(F(">cronometer")))
    g_cronometer.reset();
  else if (g_currentNode->getLabel() == String(F(">logout")))
  {
    navigateBack();
    navigateBack();
  }
  else if (g_currentNode->getLabel() == String(F(">on")))
    PORTB|=(1<<PB2);//digitalWrite(PIN_BACK_LIGHT, HIGH);
  else if (g_currentNode->getLabel() == String(F(">off")))
    PORTB&=!(1<<PB2);//digitalWrite(PIN_BACK_LIGHT, LOW);
  else if (g_currentNode->getParent()->getLabel() == String(F(">keyboard")))
  {
    if(g_pw.getTop()+1 < MAX_PW_LENGTH)
    {
      int8_t num = g_currentNode->getLabel()[0] - '0';
      g_pw.push(num);
    }
  }
  else if (g_currentNode->getLabel() == String(F(">backspace")))
  {
    if(!g_pw.isEmpty())
    {
      g_pw.pop();
      lcd.clear();
    }
  }
}

///desacoplar
void selectNode()
{
  if (g_currentNode->getChildCount() > 0)
  {
    if (g_currentNode->getChild(0) != nullptr)
    {
      bool pass = false;

      // Verificacao de senha
      if (g_currentNode->getLabel() == String(F(">enter")))
      {
        
        if(g_pw.getTop()+1 == MAX_PW_LENGTH)
        {
          int8_t pw_3 = g_pw.pop();
          int8_t pw_2 = g_pw.pop();
          int8_t pw_1 = g_pw.pop();
          int8_t pw_0 = g_pw.pop();

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
        g_lcd_is_clean=false;
        g_currentNode = g_currentNode->getChild(0);
        Node::getMenuCursorStack().push(Node::getCursor());
        Node::setCursor(0);
      }
    }      
  } 

  else
    executeButton();
}

///desacoplar
void displayCurrentNode()
{
  if (g_lcd_is_clean == false)
  {
    lcd.clear();
    g_lcd_is_clean = true;
  }

  String text = g_currentNode->getLabel();
  
  int8_t loc_bar_n = -1;
  int8_t i=0;
  for (i=0; i<text.length(); i++)
    if (text[i] == '\\')
      if(text[i+1] == 'n' and i<text.length())
        loc_bar_n = i;

  if (loc_bar_n != -1) // se tem \n
  {
      String line1 = g_currentNode->getLabel().substring(0, loc_bar_n);// Parte antes do \n
      String line2 = g_currentNode->getLabel().substring(loc_bar_n+2, i);// Parte depois do \n
      lcd.setCursor(0, 0);
      lcd.print(line1);
      lcd.setCursor(0, 1);
      lcd.print(line2);
  }
  else // Se não tiver \n, exibe tudo na primeira linha
  {
      // Opcao atual
      lcd.setCursor(0, 0);
      lcd.print(g_currentNode->getLabel());

      // Proxima opcao, se existir
      if (g_currentNode->getParent()->getChildCount() > Node::getCursor()+1)
      {
        if (g_currentNode->getParent()->getChild(Node::getCursor()+1) != nullptr)
        {
          String label = g_currentNode->getParent()->getChild(Node::getCursor()+1)->getLabel();
          label.setCharAt(0, ' ');       
          lcd.setCursor(0, 1);
          lcd.print(label); 
        }
      }
  }

  // Prints de conteudos especificos de menus

  if (g_currentNode->getParent()->getLabel() == String(F(">cronometer")))// or g_currentNode->getLabel() == String(F(">cronometer")))
    g_cronometer.printCronometer(lcd);

  else if (g_currentNode->getParent()->getLabel() == String(F(">counter")))
  {
    uint8_t pos_cursor = 13;
    if (g_cont >= 0)
      pos_cursor += (g_cont < 100) + (g_cont < 10);
    else
      pos_cursor += (g_cont > -100) + (g_cont > -10) - 1;
    lcd.setCursor(pos_cursor, 0);
    lcd.print(g_cont);
  }
  else if (g_currentNode->getParent()->getLabel() == String(F(">keyboard")) or g_currentNode->getParent()->getLabel() == String(F(">login")))
  {
    for(int8_t i=0;i<g_pw.getTop()+1;i++)
    {
      lcd.setCursor(15-i, 0);
      lcd.print(F("*"));
    }
  }
}


// void botaoApertado(int8_t botao)
// {
//   //Quando um botão for apertado

// }

///OK
void botaoSolto(int8_t botao)
{
  //Quando um botão for solto

  if (botao == BT_DOWN)
    navigateDown();
  else if (botao == BT_UP)
    navigateUp();
  else if (botao == BT_SELECT)
    selectNode();
  else if (botao == BT_LEFT)
    navigateBack();
  else if (botao == BT_RIGHT)
    toggleLight();
}

///talvez precisa de mais func aqui ao desacoplar
void loop()
{
  // Gerenciamento Grafico
  displayCurrentNode();

  // Gerenciamento de Eventos
  // uint8_t bt_pressed = checkButtonPress();
  // handleButtonPress(bt_pressed); // notifica Gerenciador de Estados
  ger_eventos.execute();

  //Estado executado de forma acoplada
  g_cronometer.execute();
}