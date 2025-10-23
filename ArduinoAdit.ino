const int ledPins[] = {10, 11, 12, 13};
const int numLEDs = 4;

const int buttonNext = 2;
const int buttonConfirm = 3;
const int potPin = A0;

int currentIndex = 0;
int inputSequence[4];
int inputCount = 0;
bool accessGranted = false;

// Password rahasia: LED urutan 1 → 3 → 2 → 0
const int password[] = {1, 3, 2, 0};

void setup() {
  for (int i = 0; i < numLEDs; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }

  pinMode(buttonNext, INPUT_PULLUP);
  pinMode(buttonConfirm, INPUT_PULLUP);

  Serial.begin(9600);
  Serial.println("Sistem siap. Silakan masukkan urutan LED.");
}

void loop() {
  int delayTime = map(analogRead(potPin), 0, 1023, 100, 1000);

  if (accessGranted) {
    Serial.println("Akses berhasil! Menampilkan efek LED...");
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < numLEDs; j++) {
        digitalWrite(ledPins[j], HIGH);
      }
      delay(delayTime);
      for (int j = 0; j < numLEDs; j++) {
        digitalWrite(ledPins[j], LOW);
      }
      delay(delayTime);
    }
    accessGranted = false;
    inputCount = 0;
    currentIndex = 0;
    Serial.println("Sistem direset. Siap menerima input baru.");
    return;
  }

  if (digitalRead(buttonNext) == LOW) {
    currentIndex = (currentIndex + 1) % numLEDs;
    showCurrentLED();
    Serial.print("Tombol Next ditekan. LED aktif: ");
    Serial.println(currentIndex);
    delay(300);
  }

  if (digitalRead(buttonConfirm) == LOW) {
    inputSequence[inputCount] = currentIndex;
    Serial.print("Tombol Confirm ditekan. Input ke-");
    Serial.print(inputCount);
    Serial.print(": ");
    Serial.println(currentIndex);
    inputCount++;
    flashLED(currentIndex, delayTime);

    if (inputCount == 4) {
      Serial.println("4 input diterima. Memeriksa password...");
      if (checkPassword()) {
        Serial.println("Password benar. Akses diberikan.");
        accessGranted = true;
      } else {
        Serial.println("Password salah. Menampilkan efek error.");
        flashError(delayTime);
        inputCount = 0;
      }
    }
    delay(300);
  }
}

void showCurrentLED() {
  for (int i = 0; i < numLEDs; i++) {
    digitalWrite(ledPins[i], i == currentIndex ? HIGH : LOW);
  }
}

void flashLED(int index, int d) {
  digitalWrite(ledPins[index], HIGH);
  delay(d);
  digitalWrite(ledPins[index], LOW);
}

void flashError(int d) {
  for (int i = 0; i < numLEDs; i++) {
    digitalWrite(ledPins[i], HIGH);
  }
  delay(d);
  for (int i = 0; i < numLEDs; i++) {
    digitalWrite(ledPins[i], LOW);
  }
}

bool checkPassword() {
  for (int i = 0; i < 4; i++) {
    if (inputSequence[i] != password[i]) {
      Serial.print("Input salah pada indeks ");
      Serial.print(i);
      Serial.print(". Diharapkan: ");
      Serial.print(password[i]);
      Serial.print(", Diberikan: ");
      Serial.println(inputSequence[i]);
      return false;
    }
  }
  return true;
}



