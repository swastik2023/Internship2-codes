#include <TinyWireM.h>
#include <Tiny4kOLED.h>

#define INA226_ADDRESS 0x40 // Default I2C address of INA226
#define ALERT_PIN 1 // Pin 6 on ATtiny85 (PB1)

// Calibration value calculated based on your specific setup
#define CALIBRATION_VALUE 168
int count = 0;
void setup() {
  pinMode(ALERT_PIN, INPUT); // Set ALERT_PIN as input
  TinyWireM.begin(); // Initialize I2C communication as master

  // Initialize the OLED display
  oled.begin();
  oled.clear();
  oled.on();
  oled.setFont(FONT8X16);
  oled.setCursor(50, 80);
  oled.println(F("EOM"));
  oled.setCursor(40, 90);
  oled.println(F("V BIAS"));
  delay(2000);
  oled.clear();

  configureINA226();
}

// Function to read current from INA226
float getCurrent_mA() {
  int16_t currentRaw = readINA226(0x04); // Read current from INA226
  // Convert raw value to actual current in milliamps (adjust based on your calibration)
  return currentRaw * 0.278461; // Adjust the multiplier based on your specific calibration
}

void loop() {
  // Read voltage and current from INA226
  uint16_t voltageRaw = readINA226(0x02); // Read voltage from INA226
  float voltage = voltageRaw * 1.25 / 1000.0; // Convert to volts (assuming default calibration)
  float current = getCurrent_mA(); // Get the current in mA
  
  // Display voltage and current on OLED with 3 decimal places
  
  oled.setCursor(0, 0);
  oled.print(F("V: "));
  oled.print(voltage, 3); // Print voltage with 3 decimal places
  oled.print(F(" V"));

  oled.setCursor(0, 2);
  oled.print(F("I: "));
  oled.print(current, 2); // Print current with 3 decimal places
  oled.print(F(" mA"));

  delay(1000); // Delay 1 second between readings

  count = count+1;
  if(count >= 10)
  {
    count = 0;
    oled.clear();
    
  }
}

void configureINA226() {

 // Set calibration register
  TinyWireM.beginTransmission(INA226_ADDRESS);
  TinyWireM.write(0x05); // Calibration register
  TinyWireM.write(CALIBRATION_VALUE >> 8); // Upper 8 bits
  TinyWireM.write(CALIBRATION_VALUE & 0xFF); // Lower 8 bits
  TinyWireM.endTransmission();
}

// Function to read from INA226
uint16_t readINA226(uint8_t reg) {
  TinyWireM.beginTransmission(INA226_ADDRESS);
  TinyWireM.write(reg);
  if (TinyWireM.endTransmission() != 0) {
    // Error handling if transmission fails
    return 0;
  }

  TinyWireM.requestFrom(INA226_ADDRESS, 2);
  if (TinyWireM.available() == 2) {
    return (TinyWireM.read() << 8) | TinyWireM.read();
  }
  return 0; // Return 0 if data is not available
}
