#include <Wire.h>

// Addressing pin slots
int as[] = {8, 9, 10, 11};
// command type received
byte command = 0;
byte value = 0;

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

  while (Wire.available() && rxCount < count) {
    input = (byte)Wire.read();
    if (rxCount == 0) {
      command = input;
    }
    else {
      value = input;
    }      
    rxCount++;
  }
}

void i2c_receive_request()
{
  switch(command)
  {
    case 1:
      Wire.write(1);
      break;
    case 2:
      Wire.write(2);
      break;
    default:
      break;
  }
}
