// 🎉 Continuous Holiday Light Show with 3 LEDs (corrected counter)
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

void setup() {
  for (int i = 0; i < numLEDs; i++) pinMode(leds[i], OUTPUT);
  Serial.begin(9600);
  Serial.println("🎄 Continuous Holiday Light Show starting...");
  Serial.println("Type: Christmas, Diwali, Halloween to switch holidays");
}

void loop() {
  // Check Serial input at every iteration
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    input.toLowerCase();
    handleHolidaySwitch(input);
  }

  // Run one iteration of the current holiday
  if (currentHoliday == "Christmas") {
    christmasPattern();
    repeatsChristmas++;       // update total repeats continuously
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

// ---------------- Handle switching holidays ----------------
void handleHolidaySwitch(String holiday) {
  if (holiday == "christmas") switchHoliday("Christmas", repeatsChristmas);
  else if (holiday == "diwali") switchHoliday("Diwali", repeatsDiwali);
  else if (holiday == "halloween") switchHoliday("Halloween", repeatsHalloween);
  else Serial.println("❌ Unknown holiday. Type: Christmas, Diwali, or Halloween");
}

void switchHoliday(String holidayName, long &holidayCounter) {
  Serial.print("🔄 Switching to "); Serial.println(holidayName);
  Serial.print("It had previously repeated: "); Serial.println(holidayCounter);
  currentHoliday = holidayName;
  currentRepeats = holidayCounter + 50;
  currentIteration = 0; // reset iteration count for this new session
}

// ---------------- Holiday Patterns ----------------
void christmasPattern() {
  for (int i = 0; i < numLEDs; i++) {
    digitalWrite(leds[i], HIGH); delay(100); digitalWrite(leds[i], LOW);
  }
  for (int i = numLEDs - 1; i >= 0; i--) {
    digitalWrite(leds[i], HIGH); delay(100); digitalWrite(leds[i], LOW);
  }
}

void diwaliPattern() {
  for (int i = 0; i < numLEDs; i++) digitalWrite(leds[i], random(0,2));
  delay(100);
}

void halloweenPattern() {
  digitalWrite(leds[random(0,numLEDs)], HIGH);
  delay(random(50,150));
  digitalWrite(leds[random(0,numLEDs)], LOW);
  delay(random(50,150));
}
