#include <Wire.h>

// Output : Module
# define PIN_RED_LED (10)
# define PIN_GREEN_LED (11)
# define PIN_YELLOW_LED (12)

// Output : Difficulty
# define PIN_EASY (5)
# define PIN_MEDIUM (6)
# define PIN_HARD (8)

// Input
# define PIN_SENSE (7)
# define PIN_BUTTON_DIFF (2)

// Command
# define CMD_TIME (0)
# define CMD_DIFFICULTY (1)
# define CMD_NEED_TO_SPEAK (2)
# define CMD_INFO (3)

# define CMD_DEFUSED (4)
# define CMD_PENALITY (5)
# define CMD_END (6)

// Answer
# define ANSWER_YES (1)
# define ANSWER_NO (0)
# define ANSWER_OK (1)

// Difficulty
# define MAX_DIFFICULTY (3)
# define HARD (3)
# define MEDIUM (2)
# define EASY (1)

bool addresses[127] = {false};

uint16_t timeleft = 300;    // 5 minutes
uint16_t difficulty = HARD;

bool defused = false;       // Is the bomb defused ?

// Message's structure
struct BusMessage
{
  uint16_t data[2];
};

void printDeviceFound(uint8_t address)
{
  Serial.print("I2C device found at address 0x");
  if (address < 16)
    Serial.print("0");
  Serial.println(address, HEX);
}

void printUnknownError(uint8_t address)
{
  Serial.print("Unknown error at address 0x");
  if (address < 16)
    Serial.print("0");
  Serial.println(address, HEX);
}

void printNumberOfDeviceFound(byte n)
{
  if (n == 0)
    Serial.println("No I2C devices found\n");
  else
  {
    Serial.print("Found ");
    Serial.print(n);
    Serial.println(" device(s).");
  }
}

void printHeader(int command, uint8_t addr, byte res)
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

void writeUint16(uint16_t i)
{
  Wire.write(i);
  Wire.write(i >> 8);
}

BusMessage request(uint8_t addr, uint16_t command, uint16_t parameter)
{
  Wire.beginTransmission(addr);
  writeUint16(command);
  writeUint16(parameter);
  Wire.endTransmission();
  delay(1);
  return receiveFrom(addr);
}

BusMessage requestTime(uint8_t addr)
{
  return request(addr, CMD_TIME, timeleft);
}

BusMessage requestDifficulty(uint8_t addr)
{
  return request(addr, CMD_DIFFICULTY, difficulty);
}

BusMessage requestNeedToSpeak(uint8_t addr)
{
  return request(addr, CMD_NEED_TO_SPEAK, 0);
}

BusMessage requestInfo(uint8_t addr)
{
  return request(addr, CMD_INFO, 0);
}

void registerSlave(uint8_t address)
{
  addresses[address] = true;
  requestDifficulty(address);
  requestTime(address);
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
      printDeviceFound(address);
      n++;
      registerSlave(address);
    }
    else if (error == 4)
      printUnknownError(address);
  }
  printNumberOfDeviceFound(n);
}

BusMessage receiveFrom(uint8_t addr)
{
  BusMessage res = { 0 };
  byte tmp = 0;
  int i = 0;

  Wire.requestFrom((int)addr, 4);
  while(Wire.available())
  {
    tmp = (byte)Wire.read();
    res.data[i / 2] = res.data[i / 2] | (tmp << 8 * (i % 2));
    i++;
  }
  return res;
}

bool someoneNeedToSpeak()
{
  return digitalRead(PIN_SENSE);
}

void decreaseTimeLeft(uint16_t nb_sec)
{
  if (timeleft < nb_sec)
    timeleft = 0;
  else
    timeleft = timeleft - nb_sec;
}

void handleNeedToSpeakCommand(uint8_t addr, BusMessage answer)
{
  BusMessage res = { 0 };

  if (answer.data[0] != ANSWER_YES)
    return;
  do
  {
    Serial.println("addr :");
    Serial.println(addr);
    res = requestInfo(addr);
    switch (res.data[0])
    {
      case CMD_DEFUSED:
        Serial.println("CMD_DEFUSED");
        addresses[addr] = false;
        break;
      case CMD_PENALITY:
        Serial.println("CMD_PENALITY");
        decreaseTimeLeft(res.data[1]);
        break;
      default:
        break;
    }
  } while (res.data[0] != CMD_END);
}

void pingEveryone()
{
  for (uint8_t i = 0 ; i < 127 ; ++i)
  {
    if (addresses[i] == true)
    {
      requestTime(i);
      if (someoneNeedToSpeak() == true)
        handleNeedToSpeakCommand(i, requestNeedToSpeak(i));
    }
  }
}

bool areAllDefused()
{
  for (uint8_t i = 0 ; i < 127 ; ++i)
  {
    if (addresses[i] == true)
      return false;
  }
  return true;
}

void updateModuleStatus()
{
  if (defused == false)
  {
    digitalWrite(PIN_RED_LED, HIGH);
    digitalWrite(PIN_GREEN_LED, LOW);
  }
  else
  {
    digitalWrite(PIN_RED_LED, LOW);
    digitalWrite(PIN_GREEN_LED, HIGH);
  }
}

void increaseDiff()
{
  difficulty += 1;
  if (difficulty > HARD)
    difficulty = EASY;
}

void updateDifficulty()
{
  increaseDiff();
  if (difficulty == EASY)
  {
    digitalWrite(PIN_EASY, HIGH);
    digitalWrite(PIN_MEDIUM, LOW);
    digitalWrite(PIN_HARD, LOW);
  }
  else if (difficulty == MEDIUM)
  {
    digitalWrite(PIN_EASY, LOW);
    digitalWrite(PIN_MEDIUM, HIGH);
    digitalWrite(PIN_HARD, LOW);
  }
  else
  {
    digitalWrite(PIN_EASY, LOW);
    digitalWrite(PIN_MEDIUM, LOW);
    digitalWrite(PIN_HARD, HIGH);
  }
}

void HandlePlay()
{
  if (defused == false)
  {
    pingEveryone();
    decreaseTimeLeft(1);
    defused = areAllDefused();
  }
  updateModuleStatus();
}

void HandleBombeExplosion()
{
  digitalWrite(PIN_YELLOW_LED, HIGH);
}

void loop()
{
  if (timeleft == 0)
    HandleBombeExplosion();
  else
    HandlePlay();
  delay(1000);
}

void setup()
{
  updateDifficulty();
  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON_DIFF), updateDifficulty, RISING);
  pinMode(PIN_EASY , OUTPUT);
  pinMode(PIN_MEDIUM, OUTPUT);
  pinMode(PIN_HARD, OUTPUT);
  pinMode(PIN_SENSE, INPUT);
  pinMode(PIN_BUTTON_DIFF, INPUT);
  pinMode(PIN_RED_LED, OUTPUT);
  pinMode(PIN_GREEN_LED, OUTPUT);
  pinMode(PIN_YELLOW_LED, OUTPUT);
  Wire.begin();
  Serial.begin(9600);
  while (!Serial);
  scan();
}
