//This code was written to be easy to understand.
//Modify this code as you see fit.
//This code will output data to the Arduino serial monitor.
//Type commands into the Arduino serial monitor to control the EZO-o2 sensor.
//This code was written in the Arduino 1.8.12 IDE
//An Arduino MEGA was used to test this code.
//This code was last tested 6/2020



String inputstring = "";                              //a string to hold incoming data from the PC
String sensorstring = "";                             //a string to hold the data from the Atlas Scientific product
boolean input_string_complete = false;                //have we received all the data from the PC
boolean sensor_string_complete = false;               //have we received all the data from the Atlas Scientific product
float o2;                                             //used to hold a float number that is the o2


void setup() {                                        //set up the hardware
  Serial.begin(9600);                                 //set baud rate for the hardware serial port_0 to 9600
  Serial3.begin(9600);                                //set baud rate for software serial port_3 to 9600
  inputstring.reserve(10);                            //set aside some bytes for receiving data from the PC
  sensorstring.reserve(30);                           //set aside some bytes for receiving data from Atlas Scientific product
}

void serialEvent() {                                  //if the hardware serial port_0 receives a char
  inputstring = Serial.readStringUntil(13);           //read the string until we see a <CR>
  input_string_complete = true;                       //set the flag used to tell if we have received a completed string from the PC
}


void serialEvent3() {                                 //if the hardware serial port_3 receives a char
  sensorstring = Serial3.readStringUntil(13);         //read the string until we see a <CR>
  sensor_string_complete = true;                      //set the flag used to tell if we have received a completed string from the PC
}


void loop() {                                         //here we go...


  if (input_string_complete == true) {                //if a string from the PC has been received in its entirety
    Serial3.print(inputstring);                       //send that string to the Atlas Scientific product
    Serial3.print('\r');                              //add a <CR> to the end of the string
    inputstring = "";                                 //clear the string
    input_string_complete = false;                    //reset the flag used to tell if we have received a completed string from the PC
  }


  if (sensor_string_complete == true) {               //if a string from the Atlas Scientific product has been received in its entirety
    Serial.println(sensorstring);                     //send that string to the PC's serial monitor
	  
	   /*                                               //uncomment this section to see how to convert the reading from a string to a float
      o2 = sensorstring.toFloat();                    //convert the string to a integer number so it can be evaluated by the Arduino
      if (o2 >= 22) {                                 //if the o2 is greater than or equal to 22%
        Serial.println("high");                       //print "high" this is demonstrating that the Arduino is evaluating the o2 as a number and not as a string
      }
       if (o2 <= 20.95) {                             //if the o2 is less than or equal to 20.95%
        Serial.println("low");                        //print "low" this is demonstrating that the Arduino is evaluating the o2 as a number and not as a string
    }
    */
 
  }
  sensorstring = "";                                   //clear the string:
  sensor_string_complete = false;                      //reset the flag used to tell if we have received a completed string from the Atlas Scientific product
  }
