#pragma once
#include <Arduino.h>
#include <LiquidCrystal.h>

class Cronometer {
  private:
    uint8_t s;
    uint8_t m;
    uint8_t h;

  public:
    Cronometer();
    uint8_t getS() const;
    uint8_t getM() const;
    uint8_t getH() const;
    void reset();
    void sec_count();
    void printCronometer(LiquidCrystal lcd) const;
    void updateCronometer();
};