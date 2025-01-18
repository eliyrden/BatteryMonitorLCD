#include <Wire.h>
#include <LiquidCrystal_I2C.h> // Library for I2C LCD


LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the LCD address to 0x27 for a 16x2 display

const int batteryPin = A0;          // Pin connected to the voltage divider
const float referenceVoltage = 5.0; // Arduino reference voltage (5V for Uno)
const float voltageDividerRatio = 2.0; // Ratio of the voltage divider (e.g., 10k/10k)
float batteryVoltage = 0.0;         // Variable to store battery voltage
int batteryPercentage = 0;          // Variable to store battery percentage

unsigned long previousMillis = 0;   // For timing the alternation
const unsigned long interval = 6000; // Switch every 6 seconds (8000 ms)
bool showVoltage = true;            // Toggle between voltage and progress bar

void setup() {
  lcd.init();       // Initialize the LCD
  lcd.backlight();  // Turn on the backlight


  // Display "Battery Voltage Monitor System" for initialization
  lcd.setCursor(0, 0);
  lcd.print("Battery Voltage");
  lcd.setCursor(0, 1);
  lcd.print("Monitor System");
  delay(5000);  // Show initialization message for 2 seconds


  // Clear the display after initialization
  lcd.clear();
}

void loop() {
  // Read battery voltage
  int rawValue = analogRead(batteryPin);
  batteryVoltage = (rawValue / 1023.0) * referenceVoltage * voltageDividerRatio;


  // Calculate battery percentage (assuming 3.0V = 0% and 4.2V = 100%)
  batteryPercentage = map(batteryVoltage * 100, 300, 420, 0, 100);
  if (batteryPercentage > 100) batteryPercentage = 100;
  if (batteryPercentage < 0) batteryPercentage = 0;


  // Alternate display every interval
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    showVoltage = !showVoltage; // Toggle between voltage and progress bar
  }


  // Display voltage or progress bar
  if (showVoltage) {
    displayVoltage();
    delay(2000); // Delay for 2 seconds before updating again
  } else {
    displayProgressBar();
    delay(2000); // Delay for 2 seconds before updating again
  }
}

void displayVoltage() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Voltage:");
  lcd.setCursor(0, 1);
  lcd.print(batteryVoltage, 2); // Display voltage with 2 decimal places
  lcd.print("V");
}


void displayProgressBar() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Battery Level:");


  // Create a progress bar
  int numBars = map(batteryPercentage, 0, 100, 0, 10); // Map percentage to 10 blocks / 10 spaces
  lcd.setCursor(0, 1);
  for (int i = 0; i < numBars; i++) {
    lcd.print((char)255);
  }
  for (int i = numBars; i < 10; i++) {
    lcd.print(" ");
  }


  lcd.setCursor(11, 1);
  lcd.print(batteryPercentage);
  lcd.print("%");
}

