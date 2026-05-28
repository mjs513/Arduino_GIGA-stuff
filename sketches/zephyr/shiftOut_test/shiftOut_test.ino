void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  for (uint16_t i = 0; i <= 0xff; i++) {
    shiftOut(2, 3, LSBFIRST, i);
    shiftOut(4, 5, MSBFIRST, i);
  }
  delay(250);
}
