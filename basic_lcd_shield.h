// Pin
#define PIN_D4 4
#define PIN_D5 5
#define PIN_D6 6
#define PIN_D7 7
#define PIN_RS 8
#define PIN_EN 9
#define PIN_BACK_LIGHT 10

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
void executeButton();