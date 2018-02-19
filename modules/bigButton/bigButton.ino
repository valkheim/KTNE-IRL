/****************************************
 * Simon slave
 *
 * Théophile Champion
 *
 * This is the simon slave.
 * User have to repeat the correct sequence to defuse
 * the module (by calling defuseModule()
 * from ktne_core).
****************************************/

#include <ktne_core.h>

#define PIN_BUTTON (2)

void loop()
{
  //delay(100);
}

void checkUserInput()
{
  if (difficulty == EASY && timeleft % 2 == 0)
  {
    defuseModule();
    return;
  }
  if (difficulty == MEDIUM && timeleft % 3 == 0)
  {
    defuseModule();
    return;
  }
  if (difficulty == HARD && timeleft % 7 == 0)
  {
    defuseModule();
    return;
  }
  applyPenality(20);
}

void setup()
{
  setupCore();
  pinMode(PIN_BUTTON, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON), checkUserInput, RISING);
}

