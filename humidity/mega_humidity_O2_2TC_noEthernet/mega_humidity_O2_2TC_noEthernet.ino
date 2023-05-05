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

// Ethernet libraries
#include <SPI.h>
#include <Ethernet.h>

#include <ArduinoRS485.h> // ArduinoModbus depends on the ArduinoRS485 library
#include <ArduinoModbus.h>

#include <Adafruit_MAX31856.h>

// Humidity sensor
String inputstring_humidity = "";                              //a string to hold incoming data from the PC
String sensorstring_humidity = "";                             //a string to hold the data from the Atlas Scientific product
boolean input_string_humidity_complete = false;                //have we received all the data from the PC
boolean sensor_string_humidity_complete = false;               //have we received all the data from the Atlas Scientific product

float HUM_float;                                      //float var used to hold the float value of the humidity.
float TMP_float;                                      //float var used to hold the float value of the temperatur.
float DEW_float;                                      //float var used to hold the float value of the dew point.

// O2 sensor
String inputstring_O2 = "";                              //a string to hold incoming data from the PC
String sensorstring_O2 = "";                             //a string to hold the data from the Atlas Scientific product
boolean input_string_O2_complete = false;                //have we received all the data from the PC
boolean sensor_string_O2_complete = false;               //have we received all the data from the Atlas Scientific product

float O2;

// Set up MAC address and IP
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(172, 21, 103, 33);
IPAddress netmask(255, 255, 254, 0);
IPAddress gateway(172, 21, 102, 1);

EthernetServer ethServer(502);
ModbusTCPServer modbusTCPServer;

// Use software SPI: CS, DI, DO, CLK
Adafruit_MAX31856 thermo1 = Adafruit_MAX31856(46, 48, 50, 52);
Adafruit_MAX31856 thermo2 = Adafruit_MAX31856(47, 49, 51, 53);


void setup() {                                        //set up the hardware

  Serial.begin(9600);                                 //set baud rate for the hardware serial port_0 to 9600
  Serial3.begin(9600);                                //set baud rate for software serial port_3 to 9600
  Serial2.begin(9600);                                //set baud rate for software serial port_2 to 9600
  inputstring_humidity.reserve(10);                   //set aside some bytes for receiving data from the PC
  sensorstring_humidity.reserve(30);                  //set aside some bytes for receiving data from Atlas Scientific product
  inputstring_O2.reserve(10);                         //set aside some bytes for receiving data from the PC
  sensorstring_O2.reserve(30);                        //set aside some bytes for receiving data from Atlas Scientific product


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


void serialEvent() {                                  //if the hardware serial port_0 receives a char
  inputstring_humidity = Serial.readStringUntil(13);  //read the string until we see a <CR>
  input_string_humidity_complete = true;              //set the flag used to tell if we have received a completed string from the PC
}


void serialEvent3() {                                 //if the hardware serial port_3 receives a char
  sensorstring_humidity = Serial3.readStringUntil(13); //read the string until we see a <CR>
  sensor_string_humidity_complete = true;             //set the flag used to tell if we have received a completed string from the PC
}

void serialEvent2() {                                 //if the hardware serial port_2 receives a char
  sensorstring_O2 = Serial2.readStringUntil(13);      //read the string until we see a <CR>
  sensor_string_O2_complete = true;                   //set the flag used to tell if we have received a completed string from the PC
}

void loop() {                                         //here we go...

  // Reading the humidity sensor...
  if (input_string_humidity_complete == true) {       //if a string from the PC has been received in its entirety
    Serial3.print(inputstring_humidity);              //send that string to the Atlas Scientific product
    Serial3.print('\r');                              //add a <CR> to the end of the string
    inputstring_humidity = "";                        //clear the string
    input_string_humidity_complete = false;           //reset the flag used to tell if we have received a completed string from the PC
  }

  if (sensor_string_humidity_complete == true) {      //if a string from the Atlas Scientific product has been received in its entirety
    if (isdigit(sensorstring_humidity[0]) == false) { //if the first character in the string is a digit
      Serial.println(sensorstring_humidity);          //send that string to the PC's serial monitor
    }
    else                                              //if the first character in the string is NOT a digit
    {
      print_Humidity_data();                          //then call this function
    }
    sensorstring_humidity = "";                       //clear the string
    sensor_string_humidity_complete = false;          //reset the flag used to tell if we have received a completed string from the Atlas Scientific product
  }

  // Reading the O2 sensor...
  if (input_string_O2_complete == true) {             //if a string from the PC has been received in its entirety
    Serial2.print(inputstring_O2);                    //send that string to the Atlas Scientific product
    Serial2.print('\r');                              //add a <CR> to the end of the string
    inputstring_O2 = "";                              //clear the string
    input_string_O2_complete = false;                 //reset the flag used to tell if we have received a completed string from the PC
  }

  if (sensor_string_O2_complete == true) {            //if a string from the Atlas Scientific product has been received in its entirety
    Serial.print("O2: ");
    Serial.println(sensorstring_O2);                  //send that string to the PC's serial monitor

    if (isdigit(sensorstring_O2[0])) {                   //if the first character in the string is a digit
                                                      //uncomment this section to see how to convert the reading from a string to a float
      O2 = sensorstring_O2.toFloat();                   //convert the string to a integer number so it can be evaluated by the Arduino
     /*
      if (o2 >= 22) {                                 //if the o2 is greater than or equal to 22%
        Serial.println("high");                       //print "high" this is demonstrating that the Arduino is evaluating the o2 as a number and not as a string
      }
       if (o2 <= 20.95) {                             //if the o2 is less than or equal to 20.95%
        Serial.println("low");                        //print "low" this is demonstrating that the Arduino is evaluating the o2 as a number and not as a string
      }
      */
    }

    sensorstring_O2 = "";                              //clear the string:
    sensor_string_O2_complete = false;                 //reset the flag used to tell if we have received a completed string from the Atlas Scientific product
  }

  float TCTemp1 = thermo1.readThermocoupleTemperature();
  float TCTemp2 = thermo2.readThermocoupleTemperature();
  Serial.print("TC1: ");
  Serial.println(TCTemp1);
  Serial.print("TC2: ");
  Serial.println(TCTemp2);

  // Check and print any faults
  uint8_t fault1 = thermo1.readFault();
  if (fault1) {
    if (fault1 & MAX31856_FAULT_CJRANGE) Serial.println("Cold Junction 1 Range Fault");
    if (fault1 & MAX31856_FAULT_TCRANGE) Serial.println("Thermocouple 1 Range Fault");
    if (fault1 & MAX31856_FAULT_CJHIGH)  Serial.println("Cold Junction 1 High Fault");
    if (fault1 & MAX31856_FAULT_CJLOW)   Serial.println("Cold Junction 1 Low Fault");
    if (fault1 & MAX31856_FAULT_TCHIGH)  Serial.println("Thermocouple 1 High Fault");
    if (fault1 & MAX31856_FAULT_TCLOW)   Serial.println("Thermocouple 1 Low Fault");
    if (fault1 & MAX31856_FAULT_OVUV)    Serial.println("Over/Under Voltage 1 Fault");
    if (fault1 & MAX31856_FAULT_OPEN)    Serial.println("Thermocouple 1 Open Fault");
  }
  delay(1000);

  // Check and print any faults
  uint8_t fault2 = thermo2.readFault();
  if (fault2) {
    if (fault2 & MAX31856_FAULT_CJRANGE) Serial.println("Cold Junction 2 Range Fault");
    if (fault2 & MAX31856_FAULT_TCRANGE) Serial.println("Thermocouple 2 Range Fault");
    if (fault2 & MAX31856_FAULT_CJHIGH)  Serial.println("Cold Junction 2 High Fault");
    if (fault2 & MAX31856_FAULT_CJLOW)   Serial.println("Cold Junction 2 Low Fault");
    if (fault2 & MAX31856_FAULT_TCHIGH)  Serial.println("Thermocouple 2 High Fault");
    if (fault2 & MAX31856_FAULT_TCLOW)   Serial.println("Thermocouple 2 Low Fault");
    if (fault2 & MAX31856_FAULT_OVUV)    Serial.println("Over/Under Voltage 2 Fault");
    if (fault2 & MAX31856_FAULT_OPEN)    Serial.println("Thermocouple 2 Open Fault");
  }
  delay(1000);

}

void print_Humidity_data(void) {                      //this function will pars the string

  char sensorstring_array[30];                        //we make a char array
  char *HUM;                                          //char pointer used in string parsing.
  char *TMP;                                          //char pointer used in string parsing.
  char *NUL;                                          //char pointer used in string parsing (the sensor outputs some text that we don't need).
  char *DEW;                                          //char pointer used in string parsing.


  sensorstring_humidity.toCharArray(sensorstring_array, 30);   //convert the string to a char array
  HUM = strtok(sensorstring_array, ",");              //let's pars the array at each comma
  TMP = strtok(NULL, ",");                            //let's pars the array at each comma
  NUL = strtok(NULL, ",");                            //let's pars the array at each comma (the sensor outputs the word "DEW" in the string, we dont need it)
  DEW = strtok(NULL, ",");                            //let's pars the array at each comma

  Serial.println();                                   //this just makes the output easier to read by adding an extra blank line.

  Serial.print("HUM:");                               //we now print each value we parsed separately.
  Serial.println(HUM);                                //this is the humidity value.

  Serial.print("Air TMP:");                           //we now print each value we parsed separately.
  Serial.println(TMP);                                //this is the air temperatur value.

  Serial.print("DEW:");                               //we now print each value we parsed separately.
  Serial.println(DEW);                                //this is the dew point.
  Serial.println();                                   //this just makes the output easier to read by adding an extra blank line.

  //uncomment this section if you want to take the values and convert them into floating point number.
  HUM_float = atof(HUM);
  TMP_float = atof(TMP);
  DEW_float = atof(DEW);

}
