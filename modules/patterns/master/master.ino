/****************************************
 * patterns/master
 *
 * Charles Paulet
 *
 * Master part of the module that controls
 * the slave. It interfaces the core and the
 * game mechanics with screen/SD.
 * 
 * This separation is induced by the lack of
 * pins on one board.
 * 
 ***************************************/

#include <ktne_core.h>

#define PIN_DIFFICULTY_0 (A0) /* tell slave to update its difficulty */
#define PIN_DIFFICULTY_1 (A1) /* tell slave to update its difficulty */
#define PIN_PENALITY (2) /* interrupt for communicating a user mistake */
#define PIN_DEFUSAL (3) /* interrupt for communicating the module defusal */
#define PENALITY (10)

void handlePenality(void)
{
  applyPenality(PENALITY * difficulty);
}

void setup()
{
  setupCore();
  pinMode(PIN_DIFFICULTY_0, OUTPUT);
  pinMode(PIN_DIFFICULTY_1, OUTPUT);
  pinMode(PIN_PENALITY, INPUT);
  pinMode(PIN_DEFUSAL, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_PENALITY), &handlePenality, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_DEFUSAL), &defuseModule, RISING);
}

void loop()
{
  if (difficulty == EASY)
  {
    analogWrite(PIN_DIFFICULTY_0, 0);
    analogWrite(PIN_DIFFICULTY_1, 0);
  }
  if (difficulty == MEDIUM)
  {
    analogWrite(PIN_DIFFICULTY_0, 255);
    analogWrite(PIN_DIFFICULTY_1, 0);
  }
  if (difficulty == HARD)
  {
    analogWrite(PIN_DIFFICULTY_0, 255);
    analogWrite(PIN_DIFFICULTY_1, 255);
  }
  delay(100);
}
