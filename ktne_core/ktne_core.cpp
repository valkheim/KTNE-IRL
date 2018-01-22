#include <Wire.h>
#include "ktne_core.h"

  /* Init default values */
  int as[] = {8, 9, 10, 11};
  const size_t sizeof_as = sizeof(as);
  uint16_t timeleft = 0xFFFF;
  uint16_t difficulty = 1;
  bool masterNeedsDefusingInformation = false;
  uint16_t defused = 0;
  uint16_t penality = 0;
  uint16_t command = 0;
  uint16_t parameter = 0;

void setupCore()
{
  /* Init pins */
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(SENSE_PIN, OUTPUT);
  digitalWrite(SENSE_PIN, LOW);
  /* Init communication */
  Wire.begin(getI2CAddr());
  Wire.onRequest(i2c_receive_request);
  Wire.onReceive(i2c_receive_data);
  Serial.begin(9600);
  Serial.println("Core setted up");
}

// Read module address from its reserved addresses pins
uint8_t getI2CAddr()
{
  uint8_t addr = 0;

  for (int i = 0 ; i < sizeof_as / sizeof(as[0]) ; ++i) {
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
