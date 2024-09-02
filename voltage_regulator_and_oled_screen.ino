#include <SPI.h> // library for I2C communication
#include <Wire.h> // library for I2C communication
#include <Adafruit_GFX.h> // library for grpahics (not needed)
#include <Adafruit_SSD1306.h> // library for OLED display
#include <math.h> // library for math stuff
#include <INA226_WE.h> // library for INA226 by wollewald

#define SCREEN_WIDTH 128 // setting OLED screen size
#define SCREEN_HEIGHT 64
#define OLED_RESET 4 
#define I2C_ADDRESS 0x40 // address for OLED board, you may need to change if using differnt OLED display
Adafruit_SSD1306 display(OLED_RESET);

  float shuntVoltage_mV = 0.000; // setting variables to simplify commands
  float busVoltage_V = 0.000;
  float current_A = 0.000;
  float loadVoltage_V = 0.000;
  float x;
  float y;
  float z;
INA226_WE ina226 = INA226_WE(0x40); // address for INA226, this may be different for you

void setup() {
  Wire.begin(); // initializing libraries
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  Serial.begin(9600);
  ina226.init();
  //ina226.setResistorRange(0.005,10.0); //sets resistor to 0.005Ω (calibration required) and max current to be 10A
  ina226.waitUntilConversionCompleted();
  ina226.setConversionTime(CONV_TIME_140); // sets conversion time, this is the fastest
  ina226.setAverage(AVERAGE_256); // sets average, the higher the more accurate, but it will be slow 
  pinMode(2,OUTPUT);
  }


void loop() {

  ina226.setCorrectionFactor(1); // you can use this to set resistor deviations, but I chose not to, so it's 1
  ina226.readAndClearFlags();
  shuntVoltage_mV = ina226.getShuntVoltage_mV(); // setting variables to make easier commands
  busVoltage_V = ina226.getBusVoltage_V();
  current_A = ina226.getCurrent_mA();
if (current_A <=0) { // this if else statment is used to prevent the display from showing negative current values
    x = 0.000;
  } 
  else{
    x = current_A;
  }
if (busVoltage_V >= 10){ //these if else statments are to align the cursors for the voltage and current if one of                                
    z = 18;              // them passes 10 as this would make the display look even
  }
  else {
    z = 30;
  }
  if ( current_A >= 10){
    y = 18;
  }
  else {
    y = 30;
  }
  display.clearDisplay(); 
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(z,0); // sets where the display starts writing
  display.print(busVoltage_V,3); // prints and sets loadvoltage to be measured to the third decimal
  display.setCursor(110,0);
  display.print("V      ");
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(y,18);
  display.print(x,3);
  display.setCursor(100,18);
  display.print("mA      ");
  display.display(); // command to display info
  Serial.println(shuntVoltage_mV,5); // use this line to check serial monitor to calibrate sensor 
  
  


}
