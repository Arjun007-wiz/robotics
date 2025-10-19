#include <EEPROM.h>

// 🎉 Continuous Holiday Light Show with 3 LEDs + Speed Memory
// LEDs on pins 10, 9, 8

const int leds[] = {10, 9, 8};
const int numLEDs = 3;

// Stores how many times each holiday has run
long repeatsChristmas = 0;
long repeatsDiwali = 0;
long repeatsHalloween = 0;

// Current holiday repeat target
long currentRepeats = 50;
long currentIteration = 0;

// Current active holiday
String currentHoliday = "Christmas";

// Speed control
int speedDelay = 100;     // default delay (ms)
int eepromAddress = 0;    // where we save speed in EEPROM

void setup() {
  for (int i = 0; i < numLEDs; i++) pinMode(leds[i], OUTPUT);
  Serial.begin(9600);

  // Load saved speed from EEPROM
  int savedSpeed = EEPROM.read(eepromAddress);
  if (savedSpeed == 1) {
    speedDelay = 50;
    Serial.println("⚡ Speed: FAST (loaded from memory)");
  } else if (savedSpeed == 2) {
    speedDelay = 200;
    Serial.println("🐢 Speed: SLOW (loaded from memory)");
  } else {
    speedDelay = 100;
    Serial.println("⚙️ Speed: NORMAL (default)");
  }

  Serial.println("🎄 Continuous Holiday Light Show starting...");
  Serial.println("Type: Christmas, Diwali, Halloween, fast, slow, or normal");
}

void loop() {
  // Check Serial input at every iteration
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    input.toLowerCase();
    handleInput(input);
  }

  // Run one iteration of the current holiday
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

  // Reset currentIteration if it exceeds currentRepeats
  if (currentIteration >= currentRepeats) {
    currentIteration = 0;
  }
}

// ---------------- Handle switching holidays or speed ----------------
void handleInput(String input) {
  if (input == "christmas") switchHoliday("Christmas", repeatsChristmas);
  else if (input == "diwali") switchHoliday("Diwali", repeatsDiwali);
  else if (input == "halloween") switchHoliday("Halloween", repeatsHalloween);
  else if (input == "fast") setSpeed(50, 1, "FAST ⚡");
  else if (input == "slow") setSpeed(200, 2, "SLOW 🐢");
  else if (input == "normal") setSpeed(100, 0, "NORMAL ⚙️");
  else Serial.println("❌ Unknown input. Type: Christmas, Diwali, Halloween, fast, slow, or normal");
}

void switchHoliday(String holidayName, long &holidayCounter) {
  Serial.print("🔄 Switching to "); Serial.println(holidayName);
  Serial.print("It had previously repeated: "); Serial.println(holidayCounter);
  currentHoliday = holidayName;
  currentRepeats = holidayCounter + 50;
  currentIteration = 0;
}

void setSpeed(int delayVal, int eepromVal, String label) {
  speedDelay = delayVal;
  EEPROM.write(eepromAddress, eepromVal);
  Serial.print("✅ Speed set to "); Serial.println(label);
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
