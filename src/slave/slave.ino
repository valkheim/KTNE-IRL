#include <Wire.h>

// Addressing pin slots
int as[] = {8, 9, 10, 11};
// command type received
byte command = 0;

byte addr = 42;

uint8_t getI2CAddr()
{
  uint8_t addr = 0;
  
  for (int i = 0 ; i < sizeof(as) / sizeof(as[0]) ; ++i) {
    pinMode(as[i], INPUT);
    if (digitalRead(as[i]) == HIGH)
      addr = addr | (digitalRead(as[i]) << i);
  }
  return addr;
}

void setup() {
  addr = getI2CAddr();
  Wire.begin(15);
  Serial.begin(9600);
  Wire.onRequest(i2c_receive_request);
  Wire.onReceive(i2c_receive_data);
  //Serial.println(addr);
}

void loop() {
  delay(100);
}

void i2c_receive_data(int count)
{
  byte input;
  byte rxCount = 0;

  Serial.println("Entering receive_data");
  Serial.println("Got count of bytes to receive: ");
  Serial.println(count);
  while (Wire.available() && rxCount < count) {
    input = (byte)Wire.read();
    if (rxCount == 0) {
      Serial.println("Got a command byte: ");
      Serial.println(input);
      command = input;
    }
    else {
      Serial.println("Got value: ");
      Serial.println(input);
    }      
    rxCount++;
  }
}

void i2c_receive_request()
{
  // Send addr + value
  // Can ask data
  Serial.println("Entering receive_request");
  switch(command)
  {
    case 1:
      Serial.println("Responding to command 1: 5");
      Wire.write(addr);
      Wire.write(5);
      break;
    case 2:
      Serial.println("Responding to command 2: 6");
      Wire.write(addr);
      Wire.write(6);
      break;
    default:
      Serial.println("Got a request without a known command.");
      Wire.write(addr);
      Wire.write(0);
      break;
  }
}
