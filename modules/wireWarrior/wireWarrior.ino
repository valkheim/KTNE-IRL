#include <ktne_core.h>

#define BUTTON (4)

#define WIRES (3)

bool patterns[3][WIRES] = {{HIGH, LOW, HIGH},
                           {LOW, HIGH, LOW},
                           {LOW, LOW, LOW}};

void setupWires(void)
{
  for (int i = 4 ; i < 7 ; ++i)
    pinMode(i, INPUT);
}

bool win(uint8_t difficulty)
{
  for (int i = 4 ; i < 7 ; ++i)
  {
    if (digitalRead(i) != patterns[difficulty - 1][i - 4])
      return false;
  }
  return true;
}

void loop()
{
  if (digitalRead(BUTTON) == HIGH)
    if (win(difficulty) == true)
      defuseModule();
}

void setup()
{
  setupCore();
  setupWires();
  pinMode(BUTTON, INPUT);
}
