#include "cronometer.h"

Cronometer::Cronometer():
s(0), m(0), h(0)
{
}

uint8_t Cronometer::getS() const {return s;}

uint8_t Cronometer::getM() const {return m;}

uint8_t Cronometer::getH() const {return h;}

void Cronometer::reset()
{
  s=m=h=0;
}

void Cronometer::sec_count()
{
  s++;
  if(s == 60)
  {
    s = 0;
    m++;
  }
  if(m == 60)
  {
    m = 0;
    h++;
  }
}

void Cronometer::printCronometer(LiquidCrystal lcd) const
{
  if(h >= 10)
      lcd.setCursor(8, 0);
      else
      {
        lcd.setCursor(8, 0);
        lcd.print(0);

        lcd.setCursor(9, 0);
      }
      lcd.print(h);

      if(m >= 10)
        lcd.setCursor(11, 0);
      else
      {
        lcd.setCursor(11, 0);
        lcd.print(0);

        lcd.setCursor(12, 0);
      }
      lcd.print(m);

      if(s >= 10)
        lcd.setCursor(14, 0);
      else
      {
        lcd.setCursor(14, 0);
        lcd.print(0);

        lcd.setCursor(15, 0);
      }
      lcd.print(s);

      lcd.setCursor(10, 0);
      lcd.print(F(":"));
      lcd.setCursor(13, 0);
      lcd.print(F(":"));
}

void Cronometer::updateCronometer()
{
  static unsigned long previousMillis = millis(); //static so eh criada uma vez; nao eh destruida ao sair da func; escopo local
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 1000)
  {
    previousMillis = currentMillis;
    sec_count();
  }
}