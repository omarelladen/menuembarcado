//cartao sd
//cronometro e timer - manter printado enquanto mexe no menu (OK)
//sinal sonoro
//contador simples (OK)
//senha (OK - simples)
//logout (OK)
//turnoff (OK)
//info firmware/kernel = prog c++ arduino ide by Omar El Laden v0.0.0 (OK)
//ver outras melhorias de RAM
//monitorar sensores
//aproveitar o bt R (OK - toggle light)
//var byte a byte
//printar na 2a linha o filho[i+1] se existir (OK)
//colocar func pra calcular o tempo durante o menu de timer apos seu star, e func pra pausar e reiniciar diretamente
//logs em arq
//melhorar g_var da linguagem
//melhorar div de arq (como separar em cpps)
//melhorar class/struct (OK-indo)
//melhorar ger grafico, de eventos e de estados
//criar classe t pra ter metodo reset() e count() (q conta por si em vez de por fora) (OK)
//ver destrutoras, ptr em vez de global(?)
//stop funciona implicitamente com gambiarra, e tmb pausa qnd da um down soq add +1seg -> agr da +1 so qnd inicia ou reinicia, e basta ir pra baixo pra pausar (precisa melhorar pra ter qnd for pra baixo continuar e pausar so qnd dar select no pause)
//timer down
//colocar as funct de cronometro na classe (OK)
//so reprintar o numer o do count (mudar chamada do displayCurrentNode)
//tratar retorno do push e pop da stack
//separar melhor em funcoes e criar classes pras principais n ficarem soltas
//melhorar classes, como private/public
//VER BEM separacao das funcoes de cada menu e o WHILE ruim do cronometro; RTOS firmware; prints (ger graf) (OK-bem melhor)
//checar se nullptr (OK - pros children, Node parent e Node child apenas nao realiza a tarefa se for nullptr)
//colocar str usadas mais de uma vez em var
//ver uso de new pro Node, rever nome e classe
//comunicacao usb
//calculadora
//melhorar o ">"
//rever arrays
//tirar senha de stack talvez
//hash pra senha
//criptografia
//singleton gerenciadores
//tem q checar se eh null antes de dar delete? da problema
///add mais classes
//onde deletar instancia de gerenciador singleton


//#include <avr/pgmspace.h>
#include "basic_lcd_shield.h"
#include "cronometer.h"
#include "tree.h"
#include "EventManager.h"

static EventManager ger_eventos;
GraphicsManager* p_ger_grafico = GraphicsManager::getInstance();
StateManager* p_ger_estados = StateManager::getInstance();

///
void setup()
{
  // Luz
  DDRB|=(1<<DDB2); //pinMode(PIN_BACK_LIGHT, OUTPUT);
  PORTB|=(1<<PB2); //digitalWrite(PIN_BACK_LIGHT, HIGH);

  Serial.begin(9600);
}

///talvez precisa de mais func aqui ao desacoplar
void loop()
{
  // Gerenciamento Grafico
  //p_ger_grafico->displayCurrentNode();

  p_ger_estados->execute();

  // Gerenciamento de Eventos
  ger_eventos.execute();
  
}