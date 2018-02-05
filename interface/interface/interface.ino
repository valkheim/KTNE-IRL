/****************************
 * interface
 * 
 * Charles Paulet
 * 
 * Simulating master sending
 * informations to central
 * computer to handle
 * scenography
 ***************************/
void setup() {
  Serial.begin(9600);
  Serial.println("tick");
}

void loop() {
  Serial.println("ping");
  delay(1500);
}
