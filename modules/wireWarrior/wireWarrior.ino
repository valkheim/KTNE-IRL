/****************************************
 * wireWarrior
 *
 * Charles Paulet
 * Nicolas-Emmanuel Robert
 *
 * User need to connect 3 wires correctly
 * to defuse the module. wire's pins are
 * defined in wires[]. The patterns that
 * must match are described in patterns[]
 * and each correspond to a difficulty.
 ***************************************/

#include <ktne_core.h>

#define WIRES (3)

int wires[] = {4, 5, 6};

bool patterns[MAX_DIFFICULTY][WIRES] = {{HIGH, LOW, HIGH},
                                        {LOW, HIGH, LOW},
                                        {LOW, LOW, LOW}};

void setupWires(void)
{
  for (int i = wires[0]; i < WIRES + wires[0]; ++i)
    pinMode(i, INPUT);
}

bool win(uint8_t difficulty)
{
  for (int i = wires[0]; i < WIRES + wires[0]; ++i)
  {
    Serial.println(patterns[difficulty - 1][i - 4]);
    if (digitalRead(i) != patterns[difficulty - 1][i - 4])
      return false;
  }
  return true;
}

void loop()
{
  if (win(difficulty))
    defuseModule();
}

void setup()
{
  setupCore();
  setupWires();
}
