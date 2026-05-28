//#define TEST_MSBFIRST
void setup() {
  Serial.begin(115200);
  pinMode(2, INPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);

  pinMode(LED_BUILTIN, OUTPUT);

  attachInterrupt(3, &shiftOut_int, RISING);
}

uint8_t shiftOut_val;
void shiftOut_int() {
#ifdef TEST_MSBFIRST
  digitalWrite(4, (shiftOut_val & 0x80)? HIGH : LOW);
  shiftOut_val <<= 1;
#else  
  digitalWrite(4, (shiftOut_val & 0x01)? HIGH : LOW);
  shiftOut_val >>= 1;
#endif
}


void loop() {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  for (uint16_t i = 0; i <= 0xff; i++) {
    shiftOut_val = i;
#ifdef TEST_MSBFIRST
    uint8_t val = shiftIn(2, 3, MSBFIRST);
#else
    uint8_t val = shiftIn(2, 3, LSBFIRST);
#endif
    if (val != i) {
      printk("%x != %x\n", i, val);
      delay(250);
    }

  }
  delay(250);
}
