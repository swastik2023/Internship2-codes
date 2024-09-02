#include <TinyWireM.h>
#include <Tiny4kOLED.h>

#define INA226_ADDRESS 0x40 // Default I2C address of INA226
#define ALERT_PIN 1 // Pin 6 on ATtiny85 (PB1)
uint16_t voltageRaw=0;
uint16_t currentRaw=0;
float voltage=0;
float current=0;
void setup() {
  pinMode(ALERT_PIN, INPUT); // Set ALERT_PIN as input
  TinyWireM.begin(); // Initialize I2C communication as master

  // Initialize the OLED display
  oled.begin();
  oled.clear();
  oled.on();
  oled.setFont(FONT8X16);
  oled.setCursor(50,80);
  oled.println(F("EOM"));
  oled.setCursor(40,90);
  oled.println(F("V BIAS"));
  delay(2000);
  oled.clear();
  resetINA226();

}

void loop() {
   float current = getCurrent_mA(); // Get the current in mA
  
  // Check if current is below threshold
  if (current < CURRENT_THRESHOLD) {
    current = 0.0; // Reset current value to 0
  }
   voltageRaw = readINA226(0x02); // Read voltage from INA226
   currentRaw = readINA226(0x04); // Read current from INA226
  
  // Convert raw values to actual voltage and current
   voltage = voltageRaw * 1.25/1000; // Convert to millivolts (assuming default calibration)
  current = currentRaw*0.02252; // Convert to milliamps (assuming default calibration)

  // Display voltage and current on OLED with 2 decimal places
  oled.clear();
  oled.setCursor(0, 0);
  oled.print(F("V: "));
  oled.print(voltage, 3); // Print voltage with 2 decimal places
  oled.print(F("V"));
  
  oled.setCursor(0, 2);
  oled.print(F("I: "));
  oled.print(current, 4); // Print current with 2 decimal places
  oled.print(F("mA"));
  
  delay(1000); // Delay 1 second between readings
}

uint16_t readINA226(uint8_t reg) {
  TinyWireM.beginTransmission(INA226_ADDRESS);
  TinyWireM.write(reg);
  TinyWireM.endTransmission();
  
  TinyWireM.requestFrom(INA226_ADDRESS, 2);
  if (TinyWireM.available() == 2) {
    return TinyWireM.read() << 8 | TinyWireM.read();
  }
  return 0;
}
void resetINA226() {
  TinyWireM.beginTransmission(INA226_ADDRESS);
  TinyWireM.write(0x00); // Point to Configuration Register
  TinyWireM.write(0x80); // MSB: Set Reset bit (bit 15)
  TinyWireM.write(0x00); // LSB: Remaining bits are 0
  TinyWireM.endTransmission();
}
