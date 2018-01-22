#include <ktne_core.h>

#define WIRES (3)

int wires[] = {4, 5, 6};

bool patterns[MAX_DIFFICULTY][3] = {{HIGH, LOW, HIGH},
                                        {LOW, HIGH, LOW},
                                        {LOW, LOW, LOW}};

void setupWires(void)
{
  for (int i = wires[0]; i < sizeof(wires) + wires[0]; ++i)
    pinMode(i, INPUT);
}

bool win(uint8_t difficulty)
{
  for (int i = wires[0]; i < sizeof(wires) + wires[0]; ++i)
  {
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
