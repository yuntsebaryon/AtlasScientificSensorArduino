//This code was written to be easy to understand.
//Modify this code as you see fit.
//This code will output data to the Arduino serial monitor.
//Type commands into the Arduino serial monitor to control the Humidity sensor.
//This code was written in the Arduino 1.8.13 IDE
//An Arduino MEGA was used to test this code.
//This code was last tested 8/2020

// Ethernet libraries
// #include <SPI.h>
// #include <Ethernet.h>

// #include <ArduinoRS485.h> // ArduinoModbus depends on the ArduinoRS485 library
// #include <ArduinoModbus.h>


String inputstring = "";                              //a string to hold incoming data from the PC
String sensorstring = "";                             //a string to hold the data from the Atlas Scientific product
boolean input_string_complete = false;                //have we received all the data from the PC
boolean sensor_string_complete = false;               //have we received all the data from the Atlas Scientific product

float HUM_float;                                      //float var used to hold the float value of the humidity.
float TMP_float;                                      //float var used to hold the float value of the temperatur.
float DEW_float;                                      //float var used to hold the float value of the dew point.


// Set up MAC address and IP
/*
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(172, 21, 103, 33);
IPAddress netmask(255, 255, 254, 0);
IPAddress gateway(172, 21, 102, 1);

EthernetServer ethServer(502);
ModbusTCPServer modbusTCPServer;
*/

void setup() {                                        //set up the hardware
  Serial.begin(9600);                                 //set baud rate for the hardware serial port_0 to 9600
  Serial3.begin(9600);                                //set baud rate for software serial port_3 to 9600
  inputstring.reserve(10);                            //set aside some bytes for receiving data from the PC
  sensorstring.reserve(30);                           //set aside some bytes for receiving data from Atlas Scientific product
  Serial3.print("O,T,1\r");                          //enable the temperature readout
  Serial3.print("O,Dew,1\r");                        //enable the dew point readout

/*
  Ethernet.begin(mac, ip);                            // Set up the ethernet
  Ethernet.setSubnetMask(netmask);
  Ethernet.setGatewayIP(gateway);

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());

  // start the server
  ethServer.begin();

  // start the Modbus TCP server
  if (!modbusTCPServer.begin()) {
    Serial.println("Failed to start Modbus TCP Server!");
    while (1);
  }

  // configure input registers at address 0x00

  modbusTCPServer.configureInputRegisters(0x00,6);
*/

}


void serialEvent() {                                  //if the hardware serial port_0 receives a char
  inputstring = Serial.readStringUntil(13);           //read the string until we see a <CR>
  input_string_complete = true;                       //set the flag used to tell if we have received a completed string from the PC
}

/*
void serialEvent3() {                                 //if the hardware serial port_3 receives a char
  sensorstring = Serial3.readStringUntil(13);         //read the string until we see a <CR>
  sensor_string_complete = true;                      //set the flag used to tell if we have received a completed string from the PC
}
*/

void loop() {                                         //here we go...

/*
  EthernetClient client = ethServer.available();
  
  if (client) {
    // a new client connected
    // Serial.println("new client");

    // let the Modbus TCP accept the connection 
    modbusTCPServer.accept(client);
    if (client.connected()) {
      // poll for Modbus TCP requests, while client connected
      modbusTCPServer.poll();
    }
  }
*/
  if (input_string_complete == true) {                //if a string from the PC has been received in its entirety
    Serial3.print(inputstring);                       //send that string to the Atlas Scientific product
    Serial3.print('\r');                              //add a <CR> to the end of the string
    inputstring = "";                                 //clear the string
    input_string_complete = false;                    //reset the flag used to tell if we have received a completed string from the PC
  }

  if (Serial3.available() > 0) {                     //if we see that the Atlas Scientific product has sent a character
    char inchar = (char)Serial3.read();              //get the char we just recived
    sensorstring += inchar;                           //add the char to the var called sensorstring
    if (inchar == '\r') {                             //if the incoming character is a <CR>
      sensor_string_complete = true;                  //set the flag
    }
  }

  if (sensor_string_complete == true) {               //if a string from the Atlas Scientific product has been received in its entirety
    if (isdigit(sensorstring[0]) == false) {          //if the first character in the string is a digit
      Serial.println(sensorstring);                   //send that string to the PC's serial monitor
    }
    else                                              //if the first character in the string is NOT a digit
    {
      print_Humidity_data();                          //then call this function
    }
    sensorstring = "";                                //clear the string
    sensor_string_complete = false;                   //reset the flag used to tell if we have received a completed string from the Atlas Scientific product
  }
/*
  uint8_t reg = 0;

  modbusTCPServer.inputRegisterWrite(reg++, (uint16_t) (HUM_float*100));
  modbusTCPServer.inputRegisterWrite(reg++, (uint16_t) (TMP_float*100));
  modbusTCPServer.inputRegisterWrite(reg++, (uint16_t) (DEW_float*100));
*/
}

void print_Humidity_data(void) {                      //this function will pars the string  

  char sensorstring_array[30];                        //we make a char array
  char *HUM;                                          //char pointer used in string parsing.
  char *TMP;                                          //char pointer used in string parsing.
  char *NUL;                                          //char pointer used in string parsing (the sensor outputs some text that we don't need).
  char *DEW;                                          //char pointer used in string parsing.

  
  sensorstring.toCharArray(sensorstring_array, 30);   //convert the string to a char array 
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
  HUM_float=atof(HUM);
  TMP_float=atof(TMP);
  DEW_float=atof(DEW);
  
}
