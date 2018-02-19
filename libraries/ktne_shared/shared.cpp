#include "74HC595.h"

void shiftOut(int myDataPin, int myClockPin, byte myDataOut)
{
  int i = 0;
  int pinState;

  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);
  for (i = 7; i >= 0; i--)
  {
    digitalWrite(myClockPin, 0);
    if (myDataOut & (1<<i))
      pinState = 1;
    else
      pinState = 0;
    digitalWrite(myDataPin, pinState);
    digitalWrite(myClockPin, 1);
    digitalWrite(myDataPin, 0);
  }
  digitalWrite(myClockPin, 0);
}
