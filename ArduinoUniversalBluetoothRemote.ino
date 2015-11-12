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

SoftwareSerial outSerial(rxPin, txPin); // RX / TX of the arduino
char inBuffer[100];
byte index = 0;

void setup() {
  Serial.begin(9600);   
  Serial.println("Arduino Bluetooth Universal Remote Server");
  outSerial.begin(9600);
  outSerial.println("AT");

}

void loop() {

  //Incoming BT data stream
  while (outSerial.available() >0) {
   char aChar = outSerial.read();
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

    
  //Manual outbound BT stream 
  while (Serial.available()) {
    char myChar = Serial.read();
    Serial.print(myChar); //echo
    outSerial.print(myChar);
  }
}

void processIncomingCommand() {
  Serial.print("Command recieved: ");
  int messageId = atoi(inBuffer);
  if (strstr(inBuffer, "ping") != 0) {
    Serial.println("Ping");
    
    sendReply(messageId,"ping");
    
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

//Send an outbound message over BLE with message ID
void sendReply(int messageId, char *reply) {
      outSerial.print( messageId);
      outSerial.print( "@");
      outSerial.print(reply);
}
