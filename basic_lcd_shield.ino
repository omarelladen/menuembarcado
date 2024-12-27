//colocar sys info em arq de cartao sd
//cronometro e timer - manter printado enquanto mexe no menu (OK)
//sinal sonoro
//contador simples (OK)
//senha
//logout (OK)
//turnoff (OK)
//info firmware/kernel = prog c++ arduino ide by Omar El Laden v0.0.0 (OK)
//ver outras melhorias de RAM
//monitorar sensores
//aproveitar o bt R (OK - toggle light por agr, mas ele interrompe o cronometro)
//multithread (pro tempo?)
//var byte a byte
//printar na 2a linha o filho[i+1] se existir (OK)
//colocar func pra calcular o tempo durante o menu de timer apos seu star, e func pra pausar e reiniciar diretamente
//logs em arq
//melhorar g_var da linguagem (sem main)
//melhorar div de arq (como separar em cpps)
//melhorar class/struct
//melhorar ger grafico, de eventos e de estados
//criar classe t pra ter metodo reset() e count() (q conta por si em vez de por fora)
//ver destrutoras, ptr em vez de global(?)
//stop funciona implicitamente com gambiarra, e tmb pausa qnd da um down soq add +1seg -> agr da +1 so qnd inicia ou reinicia, e basta ir pra baixo pra pausar (precisa melhorar pra ter qnd for pra baixo continuar e pausar so qnd dar select no pause)
//timer (down)
//colocar as funct de cronometro na classe
//so reprintar o numer o do count (mudar chamada do displayCurrentNode)
//colocar volatile
//tratar retorno do push e pop da stack
//separar melhor em funcoes
//VER BEM separacao das funcoes de cada menu e o WHILE ruim do cronometro; RTOS firmware; prints (ger graf)
//checar se nullptr (OK pros children, Node parent e Node child - apenas nao realiza a tarefa se for nullptr)
//colocar str usadas mais de uma vez em var
//melhorar classes, como private/public
//ver uso de new pro Node, rever nome e classe 

//#include <Arduino.h>
//#include <LiquidCrystal.h>
//#include <stdint.h>
//#include <avr/pgmspace.h>
#include "basic_lcd_shield.h"
#include "cronometer.h"
#include "stack.h"
#include "tree.h"

// const char         str_root[] = "omar@arduino:\\n/$_"
// const char PROGMEM str_root[] = "omar@arduino:\\n/$_";
// String readProgmemString(const char* progmemStr) {
//  size_t len = strlen_P(progmemStr);// Calcula o tamanho da string em PROGMEM
//  char buffer[len + 1];// Cria um buffer temporario na SRAM para copiar a string
//  strcpy_P(buffer, progmemStr);// Copia a string da memoria Flash para o buffer na SRAM
//  return String(buffer);// Converte o buffer para um objeto String e retorna
// }

static LiquidCrystal lcd(pin_RS,pin_En,pin_D4,pin_D5,pin_D6,pin_D7);
static Node* g_currentNode = g_currentNode->initializeTree();
static int8_t g_estadoBotaoAnt=bt_NENHUM;
static bool cronometer_is_running=false;
static unsigned long g_bt_delay=0;
static Cronometer g_cronometer;
static Stack g_menu_cursor_stack;
static int16_t g_cont=0;
static int8_t cursor=0;
static bool lcd_is_clean=false;

void setup()
{
  lcd.begin(16, 2);

  // Luz
  DDRB|=(1<<DDB2);//pinMode(pin_back_light, OUTPUT);
  PORTB|=(1<<PB2);//digitalWrite(pin_back_light, HIGH);
}

void toggleLight()
{
  if (PORTB & (1 << PB2))// PB2 = pino digital 10 (pin_back_light)
    PORTB&=!(1<<PB2);//digitalWrite(pin_back_light, LOW);
  else
    PORTB|=(1<<PB2);//digitalWrite(pin_back_light, HIGH);
}

void navigateBack()
{
  if (g_currentNode->getParent() != nullptr)
  {
    lcd_is_clean=false;
    g_currentNode = g_currentNode->getParent();
    cursor = g_menu_cursor_stack.pop();
  } 
}

void navigateUp()
{
  if (cursor > 0)
    if (g_currentNode->getParent()->getChild(cursor-1) != nullptr)
    {
      lcd_is_clean=false;
      g_currentNode = g_currentNode->getParent()->getChild(--cursor);
    }
}

void navigateDown()
{
  if (cursor < g_currentNode->getParent()->getChildCount() - 1 and g_currentNode->getLabel() != F("omar@arduino:\\n/$_")) //nao pd ir para tras da raiz
    if (g_currentNode->getParent()->getChild(cursor+1) != nullptr)
    {
      lcd_is_clean=false;
      g_currentNode = g_currentNode->getParent()->getChild(++cursor);
    }
}

void selectNode()
{
  // Navegacao
  if (g_currentNode->getChildCount() > 0)
  {
    if (g_currentNode->getChild(0) != nullptr)
    {
      lcd_is_clean=false;
      g_currentNode = g_currentNode->getChild(0);
      g_menu_cursor_stack.push(cursor);
      cursor = 0;
    }      
  } 

  // Funcionalidades (sem filhos)
  else if (g_currentNode->getLabel() == String(F(">up")))
    g_cont++;
  else if (g_currentNode->getLabel() == String(F(">down")))
    g_cont--;
  else if (g_currentNode->getLabel() == String(F(">reset")) and g_currentNode->getParent()->getLabel() == String(F(">counter")))
    g_cont=0;
  else if (g_currentNode->getLabel() == String(F(">start")))
    cronometer_is_running = true;
  else if (g_currentNode->getLabel() == String(F(">pause")))
    cronometer_is_running = false;
  else if (g_currentNode->getLabel() == String(F(">reset")) and g_currentNode->getParent()->getLabel() == String(F(">cronometer")))
  {
    g_cronometer.reset();
    cronometer_is_running = false;
  }
  else if (g_currentNode->getLabel() == String(F(">logout")))
    g_currentNode = g_currentNode->getParent()->getParent();
  else if (g_currentNode->getLabel() == String(F(">on")))
    PORTB|=(1<<PB2);//digitalWrite(pin_back_light, HIGH);
  else if (g_currentNode->getLabel() == String(F(">off")))
    PORTB&=!(1<<PB2);//digitalWrite(pin_back_light, LOW);
}

void displayCurrentNode()
{
  if (lcd_is_clean == false)
    lcd.clear();

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
      if (g_currentNode->getParent()->getChildCount() > cursor+1)
        if (g_currentNode->getParent()->getChild(cursor+1) != nullptr)
        {
          String label = g_currentNode->getParent()->getChild(cursor+1)->getLabel();
          label.setCharAt(0, ' ');       
          lcd.setCursor(0, 1);
          lcd.print(label); 
        }
  }

  // Prints de conteudos especificos de menus

  if (g_currentNode->getParent()->getLabel() == String(F(">cronometer")))// or g_currentNode->getLabel() == String(F(">cronometer")))
    g_cronometer.printCronometer(lcd);

  if (g_currentNode->getParent()->getLabel() == String(F(">counter")))
  {
    uint8_t pos_cursor = 13;
    if (g_cont >= 0)
      pos_cursor += (g_cont < 100) + (g_cont < 10);
    else
      pos_cursor += (g_cont > -100) + (g_cont > -10) - 1;
    lcd.setCursor(pos_cursor, 0);
    lcd.print(g_cont);
  }
}

void handleButtonPress(int8_t botao)
{
  lcd_is_clean=true;

  //Quando o botao for apertado ou solto
  if ((millis() - g_bt_delay) > tempo_debounce)
  {
    // Apertado
    // if ((botao != bt_NENHUM) and (g_estadoBotaoAnt == bt_NENHUM) )
    // {
    //   botaoApertado(botao); 
    //   g_bt_delay = millis();
    // }

    // Solto
    if ((botao == bt_NENHUM) and (g_estadoBotaoAnt != bt_NENHUM) )
    {
      botaoSolto(g_estadoBotaoAnt); 
      g_bt_delay = millis();
    }
  }
  g_estadoBotaoAnt = botao;
}

// void botaoApertado(int8_t botao)
// {
//   //Quando um botão for apertado

// }

void botaoSolto(int8_t botao)
{
  //Quando um botão for solto

  if (botao == bt_DOWN)
    navigateDown();
  else if (botao == bt_UP)
    navigateUp();
  else if (botao == bt_SELECT)
    selectNode();
  else if (botao == bt_LEFT)
    navigateBack();
  else if (botao == bt_RIGHT)
    toggleLight();
}

uint8_t checkButtonPress()
{
  int16_t val_botoes = analogRead(pin_botoes);

  int8_t botao = -1;
  if ((val_botoes < sel_threshold) and (val_botoes >= left_threshold))
    botao = (bt_SELECT);
  else if ((val_botoes < left_threshold) and (val_botoes >= up_threshold))
    botao = (bt_LEFT);
  else if ((val_botoes < up_threshold) and (val_botoes >= down_threshold))
    botao = (bt_DOWN);
  else if ((val_botoes < down_threshold) and (val_botoes >= right_threshold))
    botao = (bt_UP);
  else if  (val_botoes < right_threshold)
    botao = (bt_RIGHT);
  else 
    botao = (bt_NENHUM);

  return botao;
}

void loop()
{


  handleButtonPress(checkButtonPress());

  displayCurrentNode();

  if (cronometer_is_running)
    g_cronometer.updateCronometer();
    
}