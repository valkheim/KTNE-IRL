#include <Wire.h>

int as[] = {8, 9, 10, 11}; // Pins used to read the module's address

uint16_t timeleft = 0xFFFF;
uint16_t difficulty = 1;


bool masterNeedsDefusingInformation = false;
uint16_t defused = 0;
uint16_t penality = 0;

uint16_t command = 0;   // Last command sent by master
uint16_t parameter = 0; // Last command's parameter sent by master

// Output
#define RED_LED (2)
#define GREEN_LED (3)
#define SENSE_PIN (7)

// Input
#define BUTTON (4)

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

uint16_t readUnit16()
{
  uint16_t res;
  
  res = (byte)Wire.read();
  res = res | ((byte)Wire.read() << 8);
  return res;
}

void i2c_receive_data(int count)
{
  command   = readUnit16();
  parameter = readUnit16();
}

void writeUint16(uint16_t i)
{
  Wire.write(i);
  Wire.write(i >> 8);  
}

void answer(uint16_t command, uint16_t parameter)
{
  writeUint16(command);
  writeUint16(parameter);
}

bool needToSpeak()
{
  return (penality != 0 || masterNeedsDefusingInformation == true);
}

void i2c_receive_request()
{
  switch(command)
  {
    case CMD_TIME:
      timeleft = parameter;
      answer(ANSWER_OK, 0);
      break;
    case CMD_DIFFICULTY:
      difficulty = parameter;
      answer(ANSWER_OK, 0);
      break;
    case CMD_NEED_TO_SPEAK:
      if (needToSpeak())
        answer(ANSWER_YES, 0);
      else
        answer(ANSWER_NO, 0);
      break;
    case CMD_INFO:
      if (penality != 0) {
        answer(CMD_PENALITY, penality);
        penality = 0;
        if (needToSpeak() == false)
          digitalWrite(SENSE_PIN, LOW);
      } else if (masterNeedsDefusingInformation == true) {
        answer(CMD_DEFUSED, 0);
        masterNeedsDefusingInformation = false;
        if (needToSpeak() == false)
          digitalWrite(SENSE_PIN, LOW);
      } else {
        answer(CMD_END, 0);
      }
      break;
    default:
      break;
  }
}

void updateModuleStatus()
{
  if (defused == false)
  {
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
  }
  else
  {
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
  }
}

void defuseModule()
{
  digitalWrite(SENSE_PIN, HIGH);
  masterNeedsDefusingInformation = true;
  defused = 1;
}

void loop()
{
  if (digitalRead(BUTTON) == HIGH)
    defuseModule();
  updateModuleStatus();
}

void setup()
{
  pinMode(BUTTON, INPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(SENSE_PIN, OUTPUT);
  digitalWrite(SENSE_PIN, LOW);
  Wire.begin(getI2CAddr());
  Wire.onRequest(i2c_receive_request);
  Wire.onReceive(i2c_receive_data);
  Serial.begin(9600);
}
