/****************************************
 * example slave
 *
 * Charles Paulet
 * Théophile Champion
 *
 * This is a slave example.
 * User need to press button to defuse
 * module (by calling defuseModule()
 * from ktne_core).
****************************************/

#include <ktne_core.h>

#define BUTTON (4)

void loop()
{
  if (digitalRead(BUTTON) == HIGH)
    defuseModule();
}

void setup()
{
  setupCore();
  pinMode(BUTTON, INPUT);
}
