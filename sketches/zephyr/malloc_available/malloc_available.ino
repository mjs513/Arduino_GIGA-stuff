void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial && millis() < 5000) {}
  delay(250);
  Serial.println("Check max malloc");
  Serial.print("Enter max KB to start: ");
  pinMode(LED_BUILTIN, OUTPUT);

  //printk("KB: %u Buffer: %p\n", buffer);
}

void loop() {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  if (Serial.available()) {
    int kb_max = Serial.parseInt();
    int kb;
    void *buffer;
    Serial.println(kb_max);
    for (kb = kb_max; kb > 0; kb--) {
      buffer = malloc(kb * 1024);
      if (buffer) break;
    }
    Serial.print(kb);
    Serial.print(": ");
    Serial.print((uint32_t)buffer, HEX);
    if (buffer) {
      Serial.print(" - ");
      Serial.print((uint32_t)buffer + kb * 1024, HEX);
    }
    Serial.print(" Stack var: ");
    Serial.println((uint32_t)&buffer, HEX);
    free(buffer);    
    Serial.print("Enter max KB to start: ");
    while (Serial.read() != -1) {}
  }
  delay(500);
}
