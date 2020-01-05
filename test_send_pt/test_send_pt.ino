void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);
}

void sendVal(unsigned long low, unsigned long high) {
  unsigned long start = micros();
  while(micros() - start < low)
    delayMicroseconds(5);
  digitalWrite(8, HIGH);
  while(micros() - start < low + high)
    delayMicroseconds(5);
  digitalWrite(8, LOW);
}

void sendStart() {
  sendVal(8000, 800);
  Serial.write('s');
}

void sendBit(int b) {
  if (b == 0)
    sendVal(800, 200);
  else
    sendVal(1600, 200);
  Serial.write(b+'0');
}

void sendStr(char *s) {
  for (char *c = s; *c; ++c) {
    if (*c == 'S')
      sendStart();
    if (*c == '0')
      sendBit(0);
    if (*c == '1')
      sendBit(1);
  }
  Serial.write('\n');
}

void loop() {
  while(1) {
    // shock 43
    sendStr("S01111000111110100101001110010101101110000");
    // light
    sendStr("S01111100011110100101001110000000011100000");
  }
}
