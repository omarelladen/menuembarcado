#pragma once
#include <stdint.h>
#include "defines.h"

class Stack {
  private:
    int8_t stack[MAX_STACK_SIZE];
    int8_t top;

  public:
    Stack();
    ~Stack();
    int8_t getTop() const;
    bool isEmpty() const;
    bool isFull() const;
    bool push(int8_t value);
    int8_t pop();
};