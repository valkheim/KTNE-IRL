#include <Wire.h>

bool addresses[127] = {false};

// Addresses slots
int as[] = {8, 9, 10, 11};
int asCount = 4;

void initAddresses()
{
  for (int i = 0; i < asCount; i++) {
    pinMode(as[i], OUTPUT);
    digitalWrite(as[i], HIGH);
  }
}

void scan()
{
  byte error, address;
  byte n;

  Serial.println("i2c scanner...");

  n = 0;
  for (address = 0; address < 127; ++address)
  {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    
    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println(" !");
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

void pingEveryone()
{
  static byte x = 0;
  for (byte i = 0 ; i < 127 ; ++i)
  {
    if (addresses[i] == true)
    {
      Serial.print("discuss with ");
      Serial.println(i);
      Wire.beginTransmission(i);
      Wire.write("x is ");
      Wire.write(x);
      Serial.println(x);
      Wire.endTransmission();
      x++;
    }
  }
}

void loop()
{
  pingEveryone();
  delay(500);
}

void setup()
{
  initAddresses();
  Wire.begin();
  Serial.begin(9600);
  while (!Serial); // wait for serial monitor
  scan();
}

