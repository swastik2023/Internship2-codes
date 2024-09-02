#include <max6675.h>
#include <QuickPID.h>

// Define pins for MAX6675
int thermoDO = 4;
int thermoCS = 5;
int thermoCLK = 6;

// Define pins for H-bridge
int directionPin = 7; // Controls the direction (cooling or heating)
int pwmPin = 3;       // Controls the PWM signal to the Peltier module

// Create MAX6675 object
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

// Define target temperature
float TARGET_TEMP = 35.0;  // Target temperature in Celsius

// PID parameters
float Kp = 0.785, Ki = 1.39, Kd = 0.099;
float currentTemp;
float output;

// Create QuickPID object
QuickPID myPID(&currentTemp, &output, &TARGET_TEMP, Kp, Ki, Kd, QuickPID::Action::direct);

void setup() {
  Serial.begin(9600);
  
  // Set H-bridge pins as outputs
  pinMode(directionPin, OUTPUT);
  pinMode(pwmPin, OUTPUT);
  
  // Initial state
  digitalWrite(directionPin, LOW);
  analogWrite(pwmPin, 0);
  
  // Initialize the QuickPID controller
  myPID.SetTunings(Kp, Ki, Kd);
  myPID.SetOutputLimits(0, 255);  // Limit PID output to match PWM range
  myPID.SetMode(QuickPID::Control::automatic);
  
  // Wait for MAX6675 to initialize
  delay(500);
}

void loop() {
  // Read temperature from MAX6675
  currentTemp = thermocouple.readCelsius();
  Serial.println(currentTemp);
  
  // Compute PID output
  myPID.Compute();
  
  // Control logic for Peltier module
  if (output >= 0) {
    digitalWrite(directionPin, LOW);   
    analogWrite(pwmPin, output);       // Set PWM signal
  } else {
    digitalWrite(directionPin, LOW);  
    analogWrite(pwmPin, -output);      // Set PWM signal (make output positive)
  }
  
  // Small delay before next reading
  delay(1000);
}
