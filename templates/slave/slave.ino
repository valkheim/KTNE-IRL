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
