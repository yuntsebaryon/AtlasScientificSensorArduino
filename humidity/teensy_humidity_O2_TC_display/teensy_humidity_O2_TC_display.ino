//This code was written to be easy to understand.
//Modify this code as you see fit.
//This code will output data to the Arduino serial monitor.
//Type commands into the Arduino serial monitor to control the Humidity sensor.
//This code was written in the Arduino 1.8.13 IDE
//An Arduino MEGA was used to test this code.
//This code was last tested 8/2020
//Author: Yun-Tse Tsai
//Date  : 2023.8.20
//Take the Arduino UNO sample code and adapt it for both the humidity and O2 sensors
//Adapt for Teensy

#include <Adafruit_MAX31856.h>

#define LOOPDELAY 300
#define DI  31
#define DO  32
#define CLK 33

// Use software SPI: CS, DI, DO, CLK
Adafruit_MAX31856 thermo1 = Adafruit_MAX31856(30, DI, DO, CLK);

// ILI9341 display, not using the touch screen functions
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

// Use the touch screen
#define TFT_CS 10
#define TFT_DC 9
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

int cy = tft.height() / 3;

// Import the serial library
#include <SoftwareSerial.h>

// Time library
#include <TimeLib.h>

#define TIME_HEADER  "T"   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message #include <TimeLib.h>


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


void setup() {                                        //set up the hardware
  Serial.begin(9600);                                 //set baud rate for the hardware serial port_0 to 9600
  Serial3.begin(9600);                                //set baud rate for software serial port_3 to 9600
  Serial4.begin(9600);                                //set baud rate for software serial port_4 to 9600

  inputstring_humidity.reserve(10);                   //set aside some bytes for receiving data from the PC
  sensorstring_humidity.reserve(30);                  //set aside some bytes for receiving data from Atlas Scientific product
  Serial3.print("O,T,1\r");                          //enable the temperature readout
  Serial3.print("O,Dew,1\r");                        //enable the dew point readout
  inputstring_O2.reserve(10);                         //set aside some bytes for receiving data from the PC
  sensorstring_O2.reserve(30);                        //set aside some bytes for receiving data from Atlas Scientific product


  // Thermocouple 1
  thermo1.begin();

  thermo1.setThermocoupleType(MAX31856_TCTYPE_T);
  delay(500);

  tft.begin();

  // read diagnostics (optional but can help debug problems)
  uint8_t x = tft.readcommand8(ILI9341_RDMODE);
  Serial.print("Display Power Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDMADCTL);
  Serial.print("MADCTL Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDPIXFMT);
  Serial.print("Pixel Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDIMGFMT);
  Serial.print("Image Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDSELFDIAG);
  Serial.print("Self Diagnostic: 0x"); Serial.println(x, HEX);

  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_YELLOW);
  tft.setRotation(3);
  tft.setTextSize(3);

  tft.setCursor(0, cy);
  tft.print("Thermocouple type: ");
  switch (thermo1.getThermocoupleType() ) {
    case MAX31856_TCTYPE_B: tft.println("B Type"); break;
    case MAX31856_TCTYPE_E: tft.println("E Type"); break;
    case MAX31856_TCTYPE_J: tft.println("J Type"); break;
    case MAX31856_TCTYPE_K: tft.println("K Type"); break;
    case MAX31856_TCTYPE_N: tft.println("N Type"); break;
    case MAX31856_TCTYPE_R: tft.println("R Type"); break;
    case MAX31856_TCTYPE_S: tft.println("S Type"); break;
    case MAX31856_TCTYPE_T: tft.println("T Type"); break;
    case MAX31856_VMODE_G8: tft.println("Voltage x8 Gain mode"); break;
    case MAX31856_VMODE_G32: tft.println("Voltage x8 Gain mode"); break;
    default: tft.println("Unknown"); break;
  }

  Serial.print("Thermocouple type: ");
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

  // setSyncProvider( requestSync);  //set function to call when sync required
  // Serial.println("Waiting for sync message");
  delay(500);

}


void serialEvent() {                                  //if the hardware serial port_0 receives a char
  inputstring_humidity = Serial.readStringUntil(13);           //read the string until we see a <CR>
  input_string_humidity_complete = true;                       //set the flag used to tell if we have received a completed string from the PC
}

void serialEvent3() {                                 //if the hardware serial port_3 receives a char
  sensorstring_humidity = Serial3.readStringUntil(13); //read the string until we see a <CR>
  sensor_string_humidity_complete = true;             //set the flag used to tell if we have received a completed string from the PC
}

void serialEvent4() {                                 //if the hardware serial port_4 receives a char
  sensorstring_O2 = Serial4.readStringUntil(13);      //read the string until we see a <CR>
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

  if (Serial3.available() > 0) {                     //if we see that the Atlas Scientific product has sent a character
    char inchar = (char)Serial3.read();              //get the char we just recived
    sensorstring_humidity += inchar;                 //add the char to the var called sensorstring
    if (inchar == '\r') {                            //if the incoming character is a <CR>
      sensor_string_humidity_complete = true;        //set the flag
    }
  }

  if (sensor_string_humidity_complete == true) {               //if a string from the Atlas Scientific product has been received in its entirety
    if (isdigit(sensorstring_humidity[0]) == false) {          //if the first character in the string is a digit
      Serial.println(sensorstring_humidity);                   //send that string to the PC's serial monitor
    }
    else                                              //if the first character in the string is NOT a digit
    {
      print_Humidity_data();                          //then call this function
    }
    sensorstring_humidity = "";                       //clear the string
    sensor_string_humidity_complete = false;          //reset the flag used to tell if we have received a completed string from the Atlas Scientific product
  }

  delay(5);

  // Reading the O2 sensor
  if (input_string_O2_complete == true) {             //if a string from the PC has been received in its entirety
    Serial4.print(inputstring_O2);                    //send that string to the Atlas Scientific product
    Serial4.print('\r');                              //add a <CR> to the end of the string
    inputstring_O2 = "";                              //clear the string
    input_string_O2_complete = false;                 //reset the flag used to tell if we have received a completed string from the PC
  }

  if (Serial4.available() > 0) {                      //if we see that the Atlas Scientific product has sent a character
    char inchar = (char)Serial4.read();               //get the char we just received
    sensorstring_O2 += inchar;                        //add the char to the var called sensorstring
    if (inchar == '\r') {                             //if the incoming character is a <CR>
      sensor_string_O2_complete = true;               //set the flag
    }
  }


  if (sensor_string_O2_complete == true) {               //if a string from the Atlas Scientific product has been received in its entirety
    // Serial.print("O2: ");
    // Serial.println(sensorstring_O2);                     //send that string to the PC's serial monitor

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

    sensorstring_O2 = "";                                //clear the string
    sensor_string_O2_complete = false;                   //reset the flag used to tell if we have received a completed string from the Atlas Scientific product
  }

  delay(5);

  // Reading thermocouple 1
  float TCTemp1 = thermo1.readThermocoupleTemperature();
  // Serial.print("TC1: ");
  // Serial.println(TCTemp1);
  
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

  delay(5);

/*
  if (Serial.available()) {
    processSyncMessage();
  }
*/
  // if (timeStatus()!= timeNotSet) {
    Serial.print(hour());
    Serial.print(" ");
    Serial.print(minute());
    Serial.print(" ");
    Serial.print(second());
    Serial.print(" ");
    Serial.print(HUM_float);
    Serial.print(" ");
    Serial.print(TMP_float);
    Serial.print(" ");
    Serial.print(DEW_float);
    Serial.print(" ");
    Serial.print(O2);
    Serial.print(" ");
    Serial.println(TCTemp1);
  // }

  delay(10);

  // Display the temperatures
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.print("Humidity: ");
  tft.print(HUM_float);
  tft.println(" %");
  tft.print("Air T: ");
  tft.print(TMP_float);
  tft.println(" C");
  tft.print("Dew T: ");
  tft.print(DEW_float);
  tft.println(" C");
  tft.print("O2: ");
  tft.print(O2);
  tft.println(" %");
  tft.print("TC: ");
  tft.print(TCTemp1);
  tft.println(" C");
  
  delay(500);

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

/*
  Serial.println();                                   //this just makes the output easier to read by adding an extra blank line. 
 
  Serial.print("HUM:");                               //we now print each value we parsed separately.
  Serial.println(HUM);                                //this is the humidity value.

  Serial.print("Air TMP:");                           //we now print each value we parsed separately.
  Serial.println(TMP);                                //this is the air temperatur value.

  Serial.print("DEW:");                               //we now print each value we parsed separately.
  Serial.println(DEW);                                //this is the dew point.
  Serial.println();                                   //this just makes the output easier to read by adding an extra blank line. 
*/
    
  //uncomment this section if you want to take the values and convert them into floating point number.
  HUM_float=atof(HUM);
  TMP_float=atof(TMP);
  DEW_float=atof(DEW);
  
}

void processSyncMessage() {
  unsigned long pctime;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013

  if(Serial.find(TIME_HEADER)) {
     pctime = Serial.parseInt();
     if( pctime >= DEFAULT_TIME) { // check the integer is a valid time (greater than Jan 1 2013)
       setTime(pctime); // Sync Arduino clock to the time received on the serial port
     }
  }
}

time_t requestSync()
{
  Serial.write(TIME_REQUEST);  
  return 0; // the time will be sent later in response to serial mesg
}
