// Pin
#define PIN_D4 4
#define PIN_D5 5
#define PIN_D6 6
#define PIN_D7 7
#define PIN_RS 8
#define PIN_EN 9
#define PIN_BACK_LIGHT 10
#define PIN_BOTOES A0

// Botoes
#define BT_NENHUM 0
#define BT_SELECT 1
#define BT_LEFT   2
#define BT_UP     3
#define BT_DOWN   4
#define BT_RIGHT  5

// Tensoes dos botoes do shield
#define sel_THRESHOLD  800
#define left_THRESHOLD 600
#define up_THRESHOLD   400
#define down_THRESHOLD 200
#define right_THRESHOLD 60

#define DEBOUNCE_TIME 50

//Pw
#define MAX_PW_LENGTH 4
#define PW_0 0
#define PW_1 0
#define PW_2 0
#define PW_3 1


void handleButtonPress(int8_t botao);
// void botaoApertado(int8_t botao);
void botaoSolto(int8_t botao);
uint8_t checkButtonPress();

void displayCurrentNode();

void navigateBack();
void navigateDown();
void navigateUp();
void selectNode();

void toggleLight();