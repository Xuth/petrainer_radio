void setup() {
  // put your setup code here, to run once:

  pinMode(8, INPUT);
  pinMode(13, OUTPUT);
  Serial.begin(115200);
  
    

}


unsigned long waitForLevel(int want) {
  static unsigned long start = 0;
  while(1) {
    if (digitalRead(8) == want)
      break;
    delayMicroseconds(5);
  }
  unsigned long next = micros();
  unsigned long ret = next - start;
  start = next;
  return ret;
}


void loopx() {
  // put your main code here, to run repeatedly:
  int cur = LOW;
  int count = 0;
  while(1) {
    for (int i = 0; i < 16; ++i) {
      unsigned long d = waitForLevel(LOW);
      Serial.write('H');
      Serial.print(d/10);
      Serial.write(' ');
      d = waitForLevel(HIGH);
      Serial.write('L');
      Serial.print(d/10);
      Serial.write(' ');
      if (d > 7000) {
        i = 2000;
      }
    }
    Serial.write('\n');
  }
}


int getBit() {
  /* returns:
   *  -1 for unknown
   *  0 for 0
   *  1 for 1
   *  2 for start code
   */
  unsigned long l = waitForLevel(HIGH);
  unsigned long h = waitForLevel(LOW);

  if (l > 600 && l < 1000 && h > 150 && h < 250)
    return 0;
  if (l > 1400 && l < 1800 && h > 150 && h < 250)
    return 1;
  if (l > 6000 && h > 700 && h < 900)
    return 2;
  return -1;
}

int getNibble() {
  int ret = 0;
  for (int i = 0; i < 4; ++i) {
    ret *= 2;
    int v = getBit();
    if (v == -1)
      return 'X';
    if (v == 2)
      return 'S';
    if (v == 1)
      ret += 1;
  }
  if (ret < 10)
    return ret + '0';
  return (ret - 10) + 'a';
}

void loop() {
  int lastOk = 0;
  while(1) {
    int v = getBit();
    if (!lastOk) {
      if (v != 2)
        continue;
      lastOk = 1;  
    }

    if (v == -1) {
      //Serial.print("\nbad\n");
      lastOk = 0;
    }
    if (v == 0)
      Serial.write('0');
    if (v == 1)
      Serial.write('1');
    if (v == 2) {
      Serial.write('\n');
      Serial.write('S');
      
    }
  }
}

void loopz() {
  while(1) {
    while(getBit() != 2)
      continue;
    for (int i = 0; i < 5; ++i) {
      Serial.write(getNibble());
      Serial.write(getNibble());
      Serial.write(' ');
    }
    Serial.print(getBit());
    Serial.write('\n');
  }
}

