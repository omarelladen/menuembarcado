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
//tratar retorno do push da stack
//separar melhor em funcoes
//VER BEM separacao das funcoes de cada menu e o WHILE ruim do cronometro; RTOS firmware; prints (ger graf)

//#include <Arduino.h>
//#include <LiquidCrystal.h>
//#include <stdint.h>
//#include <avr/pgmspace.h>
#include "basic_lcd_shield.h"
#include "cronometer.h"
#include "stack.h"
#include "tree.h"

// const char str_root[] = "omar:/$_";
// const char PROGMEM str_root[] = "omar@arduino:/$_" //readProgmemString
// String readProgmemString(const char* progmemStr) {
//     size_t len = strlen_P(progmemStr);// Calcula o tamanho da string em PROGMEM
//     char buffer[len + 1];// Cria um buffer temporario na SRAM para copiar a string
//     strcpy_P(buffer, progmemStr);// Copia a string da memoria Flash para o buffer na SRAM
//     return String(buffer);// Converte o buffer para um objeto String e retorna
// }
// const char descBotao0[] PROGMEM = "";// const char descBotao1[] PROGMEM = "S";// const char descBotao2[] PROGMEM = "L";// const char descBotao3[] PROGMEM = "D";// const char descBotao4[] PROGMEM = "U";// const char descBotao5[] PROGMEM = "R";
// const char* const descBotao[6] PROGMEM = {descBotao0, descBotao1, descBotao2, descBotao3, descBotao4, descBotao5};
//const String descBotao[6] = {"", "S", "L", "D", "U", "R"};

LiquidCrystal lcd(pin_RS, pin_En, pin_D4, pin_D5, pin_D6, pin_D7);
Node* g_currentNode;
Stack g_menu_cursor_stack;
static int8_t cursor = 0;
static int16_t g_cont = 0;
static Cronometer g_cronometer;
static int8_t g_estadoBotaoAnt = bt_NENHUM;
static unsigned long g_bt_delay = 0;
bool isCronometerRunning = false;

void setup()
{
  lcd.begin(16, 2);

  // Luz
  DDRB|=(1<<DDB2); //pinMode(pin_back_light, OUTPUT);
  PORTB|=(1<<PB2);//digitalWrite(pin_back_light, HIGH);

  //Serial.begin(9600);

  g_currentNode = initializeTree();
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
  if (g_currentNode->parent != nullptr)
  {
    g_currentNode = g_currentNode->parent;
    cursor = g_menu_cursor_stack.pop();
  }
}

void navigateUp()
{
  if (cursor > 0)
    g_currentNode = g_currentNode->parent->children[--cursor];
}

void navigateDown()
{
  if (cursor < g_currentNode->parent->childCount - 1 and g_currentNode->label != F("omar@arduino:\\n/$_")) //nao pd ir para tras da raiz
    g_currentNode = g_currentNode->parent->children[++cursor];
}


void selectNode()
{
  // Navegacao
  if (g_currentNode->childCount > 0)
  {
    g_currentNode = g_currentNode->children[0];
    g_menu_cursor_stack.push(cursor);
    cursor = 0;
  }
  

  // Funcionalidades (sem filhos)
  else if (g_currentNode->label == String(F(">up")))
    g_cont++;
  else if (g_currentNode->label == String(F(">down")))
    g_cont--;
  else if (g_currentNode->label == String(F(">reset")) and g_currentNode->parent->label == String(F(">counter")))
    g_cont=0;
  else if (g_currentNode->label == String(F(">start")))
    isCronometerRunning = true;
  else if (g_currentNode->label == String(F(">pause")))
    isCronometerRunning = false;
  else if (g_currentNode->label == String(F(">reset")) and g_currentNode->parent->label == String(F(">cronometer")))
  {
    g_cronometer.reset();
    isCronometerRunning = false;
  }
  else if (g_currentNode->label == String(F(">logout")))
    g_currentNode = g_currentNode->parent->parent;
  else if (g_currentNode->label == String(F(">on")))
    PORTB|=(1<<PB2);//digitalWrite(pin_back_light, HIGH);
  else if (g_currentNode->label == String(F(">off")))
    PORTB&=!(1<<PB2);//digitalWrite(pin_back_light, LOW);

}

void displayCurrentNode()
{
  String text = g_currentNode->label;
  
  int8_t loc_barn = -1;
  int8_t i=0;
  for (i=0; i<text.length(); i++)
    if (text[i] == '\\')
      if(text[i+1] == 'n' and i<text.length())
        loc_barn = i;

  if (loc_barn != -1) // se na tem \n
  {
      String line1 = g_currentNode->label.substring(0, loc_barn);// Parte antes do \n
      String line2 = g_currentNode->label.substring(loc_barn+2, i);// Parte depois do \n

      lcd.setCursor(0, 0);
      lcd.print(line1);

      lcd.setCursor(0, 1);
      lcd.print(line2);
  }
  else // Se não houver \n, exibe tudo na primeira linha
  {
      // Opcao atual
      lcd.setCursor(0, 0);
      lcd.print(g_currentNode->label);

      // Proxima opcao, se existir
      if (g_currentNode->parent->childCount > cursor+1)
      {
        String label = g_currentNode->parent->children[cursor+1]->label;
        label.setCharAt(0, ' ');
        
        lcd.setCursor(0, 1);
        lcd.print(label); 
      }
  }

  if (g_currentNode->parent->label == String(F(">cronometer")))// or g_currentNode->label == String(F(">cronometer")))
    g_cronometer.printCronometer(lcd);

  if (g_currentNode->parent->label == String(F(">counter")))
  {
    uint8_t pos_cursor = 13;
    if (g_cont >= 0) pos_cursor += (g_cont < 100) + (g_cont < 10);
    else pos_cursor += (g_cont > -100) + (g_cont > -10) - 1;
    lcd.setCursor(pos_cursor, 0);
    lcd.print(g_cont);
  }
}

void handleButtonPress(int8_t botao)
{
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

  lcd.clear();

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
  displayCurrentNode(); //desnecessario o tempo todo mas desacoplado (melhorar)
  handleButtonPress(checkButtonPress());

  if (isCronometerRunning)
    g_cronometer.updateCronometer();
}
