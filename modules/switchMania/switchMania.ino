/****************************************
 * Switch Mania
 *
 * Léo Paol
 * Nicolas-Emmanuel Robert
 *
 * Find the right pattern to
 * defuse the module and prevent
 * the explosion of the bomb.
 ***************************************/

#include <ktne_core.h>

#define NUMBER_OF_SWITCHS (4)

#define BUTTON (2)
#define FIRST_SWITCH (3)
#define SECOND_SWITCH (6)
#define THIRD_SWITCH (12)
#define FOURTH_SWITCH (13)

bool patterns[MAX_DIFFICULTY][NUMBER_OF_SWITCHS] = {{HIGH, LOW, HIGH, HIGH},
                                                     {LOW, HIGH, HIGH, LOW},
                                                     {LOW, LOW, HIGH, HIGH}};

void  patternValidation()
{
  if (digitalRead(FIRST_SWITCH) == patterns[difficulty - 1][0] &&
      digitalRead(SECOND_SWITCH) == patterns[difficulty - 1][1] &&
      digitalRead(THIRD_SWITCH) == patterns[difficulty - 1][2] &&
      digitalRead(FOURTH_SWITCH) == patterns[difficulty - 1][3])
      {
        defuseModule();
      }
   else
   {
    applyPenality(20 + difficulty * 5);
   }
}

void  setupPins()
{
  pinMode(BUTTON, INPUT);
  pinMode(FIRST_SWITCH, INPUT);
  pinMode(SECOND_SWITCH, INPUT);
  pinMode(THIRD_SWITCH, INPUT);
  pinMode(FOURTH_SWITCH, INPUT);
  attachInterrupt(digitalPinToInterrupt(BUTTON), patternValidation, RISING);
}

void loop()
{
  delay(100);
}

void setup()
{
  setupCore();
  setupPins();
}
