#include <Wire.h>

bool addresses[127] = {false};

// Addresses pins
int as[] = {8, 9, 10, 11};

uint16_t timeleft = 300; // 5 minutes
uint16_t difficulty = 1;

// End of game
bool eog = false;

void initAddresses()
{
  for (int i = 0; i < sizeof(as) / sizeof(as[0]); i++) {
    pinMode(as[i], OUTPUT);
    digitalWrite(as[i], HIGH);
  }
}

void scan()
{
  byte n, error;
  uint8_t address;

  n = 0;
  for (address = 0 ; address < 127 ; ++address)
  {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
      n++;
      addresses[address] = true;
    }
    else if (error == 4)
    {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (n == 0)
    Serial.println("No I2C devices found\n");
  else
  {
    Serial.print("Found ");
    Serial.print(n);
    Serial.println(" device(s).");
  }
}

byte transmit(int addr, byte command, uint16_t value)
{
  byte res = 0;

  Wire.beginTransmission(addr);
  Wire.write(command);
  Wire.write(value);
  Wire.write(value >> 8);
  Wire.endTransmission();
  delay(1);
  Wire.requestFrom(addr, 1);
  while(Wire.available())
  {
    res = (byte)Wire.read();
  }
  return res;
}

void pingEveryone()
{
  for (uint8_t i = 0 ; i < 127 ; ++i)
  {
    if (addresses[i] == true)
    {
      analyse(0, i, transmit(i, 0, timeleft)); // update timeleft
      analyse(1, i, transmit(i, 1, 0)); // defused ?
      analyse(2, i, transmit(i, 2, difficulty)); // difficulty
      analyse(3, i, transmit(i, 3, 0)); // user made mistake ?
    }
  }
}

void printHeader(int command, int addr, byte res)
{
  Serial.print("=== command : ");
  Serial.print(command);
  Serial.print(" | addr : ");
  Serial.print(addr);
  Serial.print(" | response : ");
  Serial.print(res);
  Serial.print(" | timeleft : ");
  Serial.print(timeleft);    
  Serial.println(" ==="); 
}

void analyse(int command, int addr, byte res)
{
  printHeader(command, addr, res);
  switch (command)
  {
    case 0: // update timeleft
      break;
    case 1: // defused ?
      if (res == 1)
      {
        Serial.print("defused : ");
        Serial.println(addr);
        addresses[addr] = false;
      }
      break;
    case 2: // difficulty
      break;
    case 3: // user made mistake ?
      Serial.print("penality : ");
      Serial.println(res);
      timeleft = timeleft - res;
      break;
    default:
      break;
  }
}

bool isEnd()
{
  if (timeleft <= 0)
  {
    return true;
  }
  for (uint8_t i = 0 ; i < 127 ; ++i)
  {
    if (addresses[i] == true)
    {
      return false;
    }
  }
  return true;
}

void loop()
{
  if (!eog) {
    pingEveryone();
    timeleft--;
  }
  else
    Serial.println("Bomb has been defused");
  eog = isEnd();
  delay(1000);
}

void setup()
{
  initAddresses();
  Wire.begin();
  Serial.begin(9600);
  while (!Serial);
  scan();
}

