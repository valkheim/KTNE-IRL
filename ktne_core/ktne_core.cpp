#include <Wire.h>
#include "ktne_core.h"

/* Init default values */
uint16_t difficulty = 1;
uint16_t timeleft = 0xFFFF;
static int as[] = {8, 9, 10, 11};
static bool masterNeedsDefusingInformation = false;
static uint16_t penality = 0;
static uint16_t command = 0;
static uint16_t parameter = 0;

static uint16_t readUnit16()
{
  uint16_t res;

  res = (byte)Wire.read();
  res = res | ((byte)Wire.read() << 8);
  return res;
}

static void i2c_receive_data(int count)
{
  command   = readUnit16();
  parameter = readUnit16();
}

static void writeUint16(uint16_t i)
{
  Wire.write(i);
  Wire.write(i >> 8);
}

static void answer(uint16_t command, uint16_t parameter)
{
  writeUint16(command);
  writeUint16(parameter);
}

static bool needToSpeak()
{
  return (penality != 0 || masterNeedsDefusingInformation == true);
}

static void i2c_receive_request()
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

// Read module address from its reserved addresses pins
static uint8_t getI2CAddr()
{
  uint8_t addr = 0;

  for (int i = 0 ; i < sizeof(as) / sizeof(as[0]) ; ++i) {
    pinMode(as[i], INPUT);
    addr = addr | (digitalRead(as[i]) << i);
  }
  return addr;
}

void setupCore()
{
  /* Init pins */
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(SENSE_PIN, OUTPUT);
  digitalWrite(SENSE_PIN, LOW);
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, LOW);
  /* Init communication */
  Wire.begin(getI2CAddr());
  Wire.onRequest(i2c_receive_request);
  Wire.onReceive(i2c_receive_data);
  Serial.begin(9600);
  Serial.println("Core setted up");
}

void defuseModule()
{
  digitalWrite(SENSE_PIN, HIGH);
  masterNeedsDefusingInformation = true;
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, HIGH);
}
