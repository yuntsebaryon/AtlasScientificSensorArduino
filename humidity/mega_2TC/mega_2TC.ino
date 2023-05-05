//This code was written to be easy to understand.
//Modify this code as you see fit.
//This code will output data to the Arduino serial monitor.
//Type commands into the Arduino serial monitor to control the Humidity sensor.
//This code was written in the Arduino 1.8.13 IDE
//An Arduino MEGA was used to test this code.
//This code was last tested 8/2020

// Update: Yun-Tse
// Date  : 2022.8.24
// Combined the humidity and O2 sensors, with humidity using serial port 3 and O2 using port 2.
// Currently the string from PC will only transfer to the humidity sensor.
// Date  : 2022.8.25
// Tested and it worked
// Date  : 2023.4.2
// Added the two thermocouple breakouts

#include <Adafruit_MAX31856.h>

// Use software SPI: CS, DI, DO, CLK
Adafruit_MAX31856 thermo1 = Adafruit_MAX31856(46, 48, 50, 52);
Adafruit_MAX31856 thermo2 = Adafruit_MAX31856(47, 49, 51, 53);


void setup() {                                        //set up the hardware

  Serial.begin(9600);                                 //set baud rate for the hardware serial port_0 to 9600

  thermo1.begin();

  thermo1.setThermocoupleType(MAX31856_TCTYPE_K);

  Serial.print("Thermocouple 1 type: ");
  switch (thermo1.getThermocoupleType() ) {
    case MAX31856_TCTYPE_B: Serial.println("B Type"); break;
    case MAX31856_TCTYPE_E: Serial.println("E Type"); break;
    case MAX31856_TCTYPE_J: Serial.println("J Type"); break;
    case MAX31856_TCTYPE_K: Serial.println("K Type"); break;
    case MAX31856_TCTYPE_N: Serial.println("N Type"); break;
    case MAX31856_TCTYPE_R: Serial.println("R Type"); break;
    case MAX31856_TCTYPE_S: Serial.println("S Type"); break;
    case MAX31856_TCTYPE_T: Serial.println("T Type"); break;
    case MAX31856_VMODE_G8: Serial.println("Voltage x8 Gain mode"); break;
    case MAX31856_VMODE_G32: Serial.println("Voltage x8 Gain mode"); break;
    default: Serial.println("Unknown"); break;
  }

  delay(500);

  thermo2.begin();

  thermo2.setThermocoupleType(MAX31856_TCTYPE_K);

  Serial.print("Thermocouple 2 type: ");
  switch (thermo2.getThermocoupleType() ) {
    case MAX31856_TCTYPE_B: Serial.println("B Type"); break;
    case MAX31856_TCTYPE_E: Serial.println("E Type"); break;
    case MAX31856_TCTYPE_J: Serial.println("J Type"); break;
    case MAX31856_TCTYPE_K: Serial.println("K Type"); break;
    case MAX31856_TCTYPE_N: Serial.println("N Type"); break;
    case MAX31856_TCTYPE_R: Serial.println("R Type"); break;
    case MAX31856_TCTYPE_S: Serial.println("S Type"); break;
    case MAX31856_TCTYPE_T: Serial.println("T Type"); break;
    case MAX31856_VMODE_G8: Serial.println("Voltage x8 Gain mode"); break;
    case MAX31856_VMODE_G32: Serial.println("Voltage x8 Gain mode"); break;
    default: Serial.println("Unknown"); break;
  }

  delay(500);
}



void loop() {                                         //here we go...

  float TCTemp1 = thermo1.readThermocoupleTemperature();
  float TCTemp2 = thermo2.readThermocoupleTemperature();
  Serial.print("TC1: ");
  Serial.println(TCTemp1);
  Serial.print("TC2: ");
  Serial.println(TCTemp2);

  // Check and print any faults
  uint8_t fault1 = thermo1.readFault();
  if (fault1) {
    if (fault1 & MAX31856_FAULT_CJRANGE) Serial.println("Cold Junction1 Range Fault");
    if (fault1 & MAX31856_FAULT_TCRANGE) Serial.println("Thermocouple1 Range Fault");
    if (fault1 & MAX31856_FAULT_CJHIGH)  Serial.println("Cold Junction1 High Fault");
    if (fault1 & MAX31856_FAULT_CJLOW)   Serial.println("Cold Junction1 Low Fault");
    if (fault1 & MAX31856_FAULT_TCHIGH)  Serial.println("Thermocouple1 High Fault");
    if (fault1 & MAX31856_FAULT_TCLOW)   Serial.println("Thermocouple1 Low Fault");
    if (fault1 & MAX31856_FAULT_OVUV)    Serial.println("Over/Under Voltage1 Fault");
    if (fault1 & MAX31856_FAULT_OPEN)    Serial.println("Thermocouple1 Open Fault");
  }
  delay(1000);

  // Check and print any faults
  uint8_t fault2 = thermo2.readFault();
  if (fault2) {
    if (fault2 & MAX31856_FAULT_CJRANGE) Serial.println("Cold Junction2 Range Fault");
    if (fault2 & MAX31856_FAULT_TCRANGE) Serial.println("Thermocouple2 Range Fault");
    if (fault2 & MAX31856_FAULT_CJHIGH)  Serial.println("Cold Junction2 High Fault");
    if (fault2 & MAX31856_FAULT_CJLOW)   Serial.println("Cold Junction2 Low Fault");
    if (fault2 & MAX31856_FAULT_TCHIGH)  Serial.println("Thermocouple2 High Fault");
    if (fault2 & MAX31856_FAULT_TCLOW)   Serial.println("Thermocouple2 Low Fault");
    if (fault2 & MAX31856_FAULT_OVUV)    Serial.println("Over/Under Voltage2 Fault");
    if (fault2 & MAX31856_FAULT_OPEN)    Serial.println("Thermocouple2 Open Fault");
  }
  delay(1000);


}
