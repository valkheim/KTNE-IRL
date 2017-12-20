#include <Wire.h>

int as[] = {8, 9, 10, 11};
int asCount = 4;

byte getI2CAddr()
{
  byte addr = 0;
  
  for (int i = 0; i < asCount; i++) {
    pinMode(as[i], INPUT);
    if (digitalRead(as[i]) == HIGH)
    {/*
      Serial.print("i : ");
      Serial.println(i);
      Serial.print("1 << i : ");
      Serial.println(1 << i);*/
      addr = addr | (1 << i);
      /*Serial.print("addr : ");
      Serial.println(addr);*/
    }
  }
  return addr;
}


void setup() {
  byte a = getI2CAddr();
  
  Wire.begin(a);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
  Serial.print("final addr : ");
  Serial.println(a);
}

void loop() {
  delay(100);
}

void receiveEvent(int howMany)
{
  while (Wire.available() > 1) {
    char c = Wire.read();
    Serial.print(c);
  }
  int x = Wire.read();
  Serial.println(x);
}
