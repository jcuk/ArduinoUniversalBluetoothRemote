//HM-10 bluetooth setup.
// Pin connections from BT moule to Arduino:
// VCC --> 5v
// GND --> 0v
// Rx --> Pin 11
// Tx --> Pin 10

#include "remoteCore.h"
#include <IRremote.h>
#include <IRremoteInt.h>

#include <SoftwareSerial.h>
#define rxPin 10
#define txPin 11

SoftwareSerial mySerial(rxPin, txPin); // RX / TX of the arduino
char inBuffer[100];
byte index = 0;

void setup() {
  Serial.begin(9600);   
  Serial.println("Arduino Bluetooth Universal Remote Server");
  mySerial.begin(9600);
  mySerial.println("AT");

}

void loop() {

  //Incoming BT data stream
  while (mySerial.available() >0) {
   char aChar = mySerial.read();
     if(aChar == ':')
     {
        processIncomingCommand();
        
        index = 0;
        inBuffer[index] = NULL;
     }
     else
     {
        inBuffer[index] = aChar;
        index++;
        inBuffer[index] = '\0'; // Keep the string NULL terminated
     }
  }

    
  //Outbound BT stream 
  while (Serial.available()) {
    char myChar = Serial.read();
    Serial.print(myChar); //echo
    mySerial.print(myChar);
  }
}

void processIncomingCommand() {
  Serial.println("Command recieved:");
  if (strstr(inBuffer, "ping") != 0) {
    Serial.println("Ping");
    
  } else if (strstr(inBuffer, "record") != 0) {
    Serial.println("Record");
    
  } else if (strstr(inBuffer, "getcode") != 0) {
    Serial.println("Get Code");
    
  } else if (strstr(inBuffer, "sendcode") != 0) {
    Serial.println("Send Code");
    
  } else {
    Serial.println("Unknown command");
  }
}
