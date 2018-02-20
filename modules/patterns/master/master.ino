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

#define PIN_DIFFICULTY (12) /* tell slave to update its difficulty */
#define PIN_PENALITY (2) /* interrupt for communicating a user mistake */
#define PIN_DEFUSAL (3) /* interrupt for communicating the module defusal */
#define PENALITY (10)

uint16_t slave_difficulty = difficulty;

void handlePenality(void)
{
  switch (difficulty) {
    case EASY:
      applyPenality(PENALITY);
      break;
    case MEDIUM:
      applyPenality(PENALITY * 2);
      break;
    case HARD:
      applyPenality(PENALITY * 3);
      break;
  }
}

void setup()
{
  setupCore();
  pinMode(PIN_DIFFICULTY, OUTPUT);
  pinMode(PIN_PENALITY, INPUT);
  pinMode(PIN_DEFUSAL, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_PENALITY), &handlePenality, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_DEFUSAL), &defuseModule, RISING);
}

void loop()
{
  if (slave_difficulty != difficulty)
  {
    digitalWrite(PIN_DIFFICULTY, HIGH);
    delay(100);
    digitalWrite(PIN_DIFFICULTY, LOW);
    if (slave_difficulty++ == MAX_DIFFICULTY)
      slave_difficulty = EASY;
  }
}
