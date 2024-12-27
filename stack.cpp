#include "stack.h"

Stack::Stack():
top(-1)// Pilha comeca vazia
{
}

bool Stack::isEmpty() const {return top == -1;}

bool Stack::isFull() const {return (top == MAX_STACK_SIZE - 1);}

bool Stack::push(int8_t value)
{
  if (isFull())
    return false;
  stack[++top] = value;
  return true;
}

int8_t Stack::pop()
{
  if (isEmpty())
    return -1;
  return stack[top--];
}

// Funcão para ver o elemento no topo (peek)
// int peek() {
//   if (isEmpty()) {
//     Serial.println("Erro: Pilha vazia!");
//     return -1;
//   }
//   return stack[top];
// }
// Funcão para imprimir os elementos da pilha
// void printStack() {
//   if (isEmpty()) {
//     Serial.println("Pilha vazia.");
//     return;
//   }
//   Serial.print("Pilha: ");
//   for (int i = 0; i <= top; i++) {
//     Serial.print(stack[i]);
//     if (i < top) Serial.print(", ");
//   }
//   Serial.println();
// }