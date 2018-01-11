#include <Wire.h>

// Addresses pins
int as[] = {8, 9, 10, 11};

uint16_t timeleft = 0xFFFF;
uint16_t difficulty = 1;

// Command type sent by master
byte command = 0;
// Response for master
uint16_t value = 0;

// Read module address from its reserved addresses pins
uint8_t getI2CAddr()
{
  uint8_t addr = 0;

  for (int i = 0 ; i < sizeof(as) / sizeof(as[0]) ; ++i) {
    pinMode(as[i], INPUT);
    addr = addr | (digitalRead(as[i]) << i);
  }
  return addr;
}

void setup() {
  Wire.begin(getI2CAddr());
  Wire.onRequest(i2c_receive_request);
  Wire.onReceive(i2c_receive_data);
}

void loop() {

}

void i2c_receive_data(int count)
{
  byte input;
  byte rxCount = 0;

  value = 0;
  while (Wire.available() && rxCount <= count) {
    input = (byte)Wire.read();
    if (rxCount == 0) { // command byte
      command = input;
    }
    else {
      if (rxCount == 1) // first parameter byte
        value = input;
      else // second parameter byte
        value = value | (input << 8);
    }
    rxCount++;
  }
}

void answer(uint16_t response)
{
  Wire.write(response);
  Wire.write(response >> 8);  
}

void i2c_receive_request()
{
  switch(command)
  {
    case 0: // Update timeleft
      timeleft = value;
      answer(1);
      break;
    case 1: // Defused ?
      if (timeleft < 150)
        answer(1);
      else
        answer(0);
      break;
    case 2: // Difficulty
      difficulty = value;
      answer(1);
      break;
    case 3: // User made mistake ? penalty in seconds
      answer(10);
      break;
    default:
      break;
  }
}
