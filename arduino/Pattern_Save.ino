#include <EEPROM.h>

// 🎉 Holiday Light Show with 3 LEDs + Saved Speed Pattern
const int leds[] = {10, 9, 8};
const int numLEDs = 3;

long repeatsChristmas = 0;
long repeatsDiwali = 0;
long repeatsHalloween = 0;

long currentRepeats = 50;
long currentIteration = 0;

String currentHoliday = "Christmas";

// ---- Speed Pattern Memory ----
int speedPattern[50];       // store up to 50 speed changes
int speedPatternLength = 0; // how many entries stored
int currentSpeedIndex = 0;  // which one we’re on now
int speedDelay = 100;       // current delay in ms
int eepromStart = 10;       // starting EEPROM address for pattern storage

void setup() {
  for (int i = 0; i < numLEDs; i++) pinMode(leds[i], OUTPUT);
  Serial.begin(9600);

  // Load saved pattern from EEPROM
  speedPatternLength = EEPROM.read(0);
  if (speedPatternLength > 0 && speedPatternLength <= 50) {
    for (int i = 0; i < speedPatternLength; i++) {
      speedPattern[i] = EEPROM.read(eepromStart + i);
    }
    Serial.println("📂 Loaded speed pattern from memory:");
    for (int i = 0; i < speedPatternLength; i++) {
      Serial.print(i+1); Serial.print(": ");
      printSpeedName(speedPattern[i]);
      Serial.println();
    }
  } else {
    Serial.println("⚙️ No pattern found. Defaulting to NORMAL speed.");
    speedDelay = 100;
  }

  Serial.println("🎄 Holiday Light Show Ready!");
  Serial.println("Type: Christmas, Diwali, Halloween");
  Serial.println("Or type a sequence like: slow, fast, slow, normal");
  Serial.println("It will learn and remember your pattern!");
}

void loop() {
  // Read user input
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    input.toLowerCase();
    handleInput(input);
  }

  // Run one iteration of the current holiday
  updateSpeed(); // Adjust delay from pattern automatically

  if (currentHoliday == "Christmas") {
    christmasPattern();
    repeatsChristmas++;
  } else if (currentHoliday == "Diwali") {
    diwaliPattern();
    repeatsDiwali++;
  } else if (currentHoliday == "Halloween") {
    halloweenPattern();
    repeatsHalloween++;
  }

  currentIteration++;
  if (currentIteration >= currentRepeats) currentIteration = 0;
}

// ---------------- Handle Input ----------------
void handleInput(String input) {
  if (input == "christmas") switchHoliday("Christmas", repeatsChristmas);
  else if (input == "diwali") switchHoliday("Diwali", repeatsDiwali);
  else if (input == "halloween") switchHoliday("Halloween", repeatsHalloween);
  else if (input.indexOf("fast") != -1 || input.indexOf("slow") != -1 || input.indexOf("normal") != -1) {
    savePattern(input);
  }
  else Serial.println("❌ Unknown input. Type holiday or speed pattern.");
}

void switchHoliday(String name, long &counter) {
  Serial.print("🔄 Switching to "); Serial.println(name);
  Serial.print("It had previously repeated: "); Serial.println(counter);
  currentHoliday = name;
  currentRepeats = counter + 50;
  currentIteration = 0;
}

// ---------------- Save and Load Speed Pattern ----------------
void savePattern(String input) {
  // Split pattern by commas
  input += ",";
  speedPatternLength = 0;
  int startIdx = 0;
  for (int i = 0; i < input.length(); i++) {
    if (input.charAt(i) == ',') {
      String part = input.substring(startIdx, i);
      part.trim();
      int val = speedFromText(part);
      if (val != -1 && speedPatternLength < 50) {
        speedPattern[speedPatternLength++] = val;
      }
      startIdx = i + 1;
    }
  }

  // Save to EEPROM
  EEPROM.write(0, speedPatternLength);
  for (int i = 0; i < speedPatternLength; i++) {
    EEPROM.write(eepromStart + i, speedPattern[i]);
  }

  Serial.println("✅ Saved new speed pattern to memory!");
}

void updateSpeed() {
  if (speedPatternLength == 0) return; // no pattern yet
  int val = speedPattern[currentSpeedIndex];
  if (val == 1) speedDelay = 50;
  else if (val == 2) speedDelay = 200;
  else speedDelay = 100;

  currentSpeedIndex++;
  if (currentSpeedIndex >= speedPatternLength) currentSpeedIndex = 0;
}

int speedFromText(String txt) {
  if (txt == "fast") return 1;
  if (txt == "slow") return 2;
  if (txt == "normal") return 0;
  return -1;
}

void printSpeedName(int val) {
  if (val == 1) Serial.print("FAST ⚡");
  else if (val == 2) Serial.print("SLOW 🐢");
  else Serial.print("NORMAL ⚙️");
}

// ---------------- Holiday Patterns ----------------
void christmasPattern() {
  for (int i = 0; i < numLEDs; i++) {
    digitalWrite(leds[i], HIGH); delay(speedDelay); digitalWrite(leds[i], LOW);
  }
  for (int i = numLEDs - 1; i >= 0; i--) {
    digitalWrite(leds[i], HIGH); delay(speedDelay); digitalWrite(leds[i], LOW);
  }
}

void diwaliPattern() {
  for (int i = 0; i < numLEDs; i++) digitalWrite(leds[i], random(0,2));
  delay(speedDelay);
}

void halloweenPattern() {
  digitalWrite(leds[random(0,numLEDs)], HIGH);
  delay(random(speedDelay/2, speedDelay+50));
  digitalWrite(leds[random(0,numLEDs)], LOW);
  delay(random(speedDelay/2, speedDelay+50));
}
