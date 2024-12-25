#pragma once
#include <Arduino.h>

#define MAX_STACK_SIZE 7

class Stack {
  private:
    int8_t stack[MAX_STACK_SIZE];
    int8_t top;

  public:
    Stack();
    bool isEmpty();
    bool isFull();
    bool push(int8_t value);
    int8_t pop();
};
