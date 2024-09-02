#include "max6675.h"

// Define pins for MAX6675
int thermoDO = 4;
int thermoCS = 5;
int thermoCLK = 6;



// Create MAX6675 object
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

// Define target temperature
const float TARGET_TEMP = 40.0;  // Target temperature in Celsius
const float TEMP_THRESHOLD = 1.0; // Allowable temperature deviation

void setup() {
  Serial.begin(9600);
  
  // Set H-bridge pins as outputs
  pinMode(3, OUTPUT);
  pinMode(7, OUTPUT);
  
  // Initial state
  digitalWrite(7, LOW);
  analogWrite(3, 255);
  
  // Wait for MAX6675 to initialize
  delay(500);
}

void loop() {
  // Read temperature from MAX6675
  double currentTemp = thermocouple.readCelsius();
  
  Serial.print(" ");
  Serial.println(currentTemp);
  
  // Control logic for Peltier module
  if (currentTemp>=TARGET_TEMP) {
    // COOLING mode
    digitalWrite(7, LOW);
    analogWrite(3,0);
  }
    // Adjust PWM value if needed
   else if (currentTemp <= TARGET_TEMP - TEMP_THRESHOLD) {
    //HEATING mode
    digitalWrite(7, LOW);
   analogWrite(3, 255);
   }
  
  // Small delay before next reading
  delay(1000);
}
