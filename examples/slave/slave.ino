#define BUTTON (4)

void loop()
{
  /* Begin module's logic */
  if (digitalRead(BUTTON) == HIGH)
    defuseModule();
  /* End module's logic */
  updateModuleStatus();
}

void setup()
{
  setupCore();
  pinMode(BUTTON, INPUT);
}
