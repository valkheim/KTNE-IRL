/****************************************
 * The order of buttons
 *
 * Léo Paolo
 * Nicolas-Emmanuel Robert
 *
 * Activate the buttons in
 * the right order to defuse
 * the bomb.
****************************************/

#include <ktne_core.h>

#define NUMBER_OF_BUTTONS (4)

#define FIRST_BUTTON (3)
#define SECOND_BUTTON (6)
#define THIRD_BUTTON (12)
#define FORTH_BUTTON (13)
#define NONE (0)

uint8_t patterns[MAX_DIFFICULTY][NUMBER_OF_BUTTONS] = {{FIRST_BUTTON, SECOND_BUTTON, THIRD_BUTTON, FORTH_BUTTON},
                                                        {THIRD_BUTTON, FIRST_BUTTON, FORTH_BUTTON, SECOND_BUTTON},
                                                        {SECOND_BUTTON, THIRD_BUTTON, FORTH_BUTTON, FIRST_BUTTON}};

uint8_t user_pattern[NUMBER_OF_BUTTONS] = {NONE, NONE, NONE, NONE};

bool isDefused = true;

void setup()
{
  setupCore();
  pinMode(FIRST_BUTTON, INPUT);
  pinMode(SECOND_BUTTON, INPUT);
  pinMode(THIRD_BUTTON, INPUT);
  pinMode(FORTH_BUTTON, INPUT);
}

bool patternIsFull()
{
  if (user_pattern[NUMBER_OF_BUTTONS - 1] != NONE)
    return true;
   return false;
}

bool  comparePattern()
{
  for (uint8_t i = 0; i < NUMBER_OF_BUTTONS; ++i)
  {
    if (user_pattern[i] != patterns[difficulty - 1][i])
      return false;
  }
  return true;
}

void appendToPattern(uint8_t button)
{
  for (uint8_t i = 0; i < NUMBER_OF_BUTTONS; ++i)
  {
    if (user_pattern[i] == NONE)
    {
      user_pattern[i] = button;
      break ;
    }
  }
}

void resetUserPattern()
{
  for (uint8_t i = 0; i < NUMBER_OF_BUTTONS; ++i)
    user_pattern[i] = NONE;
}

bool  buttonAlreadyPressed(uint8_t b)
{
  for (uint8_t i = 0; i < NUMBER_OF_BUTTONS; ++i)
  {
    if (user_pattern[i] == b)
      return true;
  }
  return false;
}

void loop()
{
  if (isDefused && patternIsFull())
  {
    if (comparePattern())
    {
      isDefused = false;
      defuseModule();
    }
    else
    {
      resetUserPattern();
      applyPenality(10 * difficulty);
    }
  }
  if (digitalRead(FIRST_BUTTON) && !buttonAlreadyPressed(FIRST_BUTTON))
    appendToPattern(FIRST_BUTTON);
  if (digitalRead(SECOND_BUTTON) && !buttonAlreadyPressed(SECOND_BUTTON))
    appendToPattern(SECOND_BUTTON);
  if (digitalRead(THIRD_BUTTON) && !buttonAlreadyPressed(THIRD_BUTTON))
    appendToPattern(THIRD_BUTTON);
  if (digitalRead(FORTH_BUTTON) && !buttonAlreadyPressed(FORTH_BUTTON))
    appendToPattern(FORTH_BUTTON);
}
