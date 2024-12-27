// Pin
#define pin_D4 4
#define pin_D5 5
#define pin_D6 6
#define pin_D7 7
#define pin_RS 8
#define pin_En 9
#define pin_back_light 10
#define pin_botoes A0

// Botoes
#define bt_NENHUM 0
#define bt_SELECT 1
#define bt_LEFT   2
#define bt_UP     3
#define bt_DOWN   4
#define bt_RIGHT  5

#define sel_threshold  800
#define left_threshold 600
#define up_threshold   400
#define down_threshold 200
#define right_threshold 60

#define tempo_debounce 50

void handleButtonPress(int8_t botao);
// void botaoApertado(int8_t botao);
void botaoSolto(int8_t botao);
uint8_t checkButtonPress();
void displayCurrentNode();
void startCronometer();
void navigateBack();
void navigateDown();
void toggleLight();
void navigateUp();