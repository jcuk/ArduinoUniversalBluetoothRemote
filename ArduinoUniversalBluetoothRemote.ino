/*
    ArduinoUniversalBluetoothRemote.ino
    
    Bluetooth Arduino Universal IR Remote controller
    Copyright (C)  2015 Jason Crane
    jasoncraneuk (at) hotmail.com

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
    
Circuit - Pin connections from HM-10 BT moule to Arduino:
* VCC --> 5v
* GND --> 0v
* Rx --> Pin 11
* Tx --> Pin 10
*/

#include "remoteCore.h"
#include <IRremote.h>
#include <IRremoteInt.h>

#include <SoftwareSerial.h>
#define rxPin 10
#define txPin 11

SoftwareSerial outSerial(rxPin, txPin); // RX / TX of the arduino
char inBuffer[100];
byte index = 0;

//Wrapper for IRRemote services
RemoteCore remoteCore;

String version="1.0.1SNAPSHOT";

void setup() {
  Serial.begin(9600);   
  Serial.println("Arduino Bluetooth Universal Remote Server");
  outSerial.begin(9600);
  outSerial.println("AT");
  
  remoteCore.begin();

}

void loop() {

  //Incoming BT data stream
  while (outSerial.available() >0) {
   char aChar = outSerial.read();
     if(aChar == ':') //All commands terminated by :
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
  
  //Check for incoming IR traffic
  remoteCore.readIRCode();
}

void processIncomingCommand() {
  Serial.print("Command recieved: ");
  int messageId = atoi(inBuffer);
  if (strstr(inBuffer, "ping") != 0) {
    Serial.println("Ping");
    
    sendReply(messageId,"OK");
    
  } else if (strstr(inBuffer, "record") != 0) {
    Serial.println("Record");
    remoteCore.resetLastCode(); 
    sendReply(messageId,"OK");
    
  } else if (strstr(inBuffer, "getcode") != 0) {
    Serial.println("Get Code");
    decode_results results = remoteCore.lastDecodedCode();
    
     outSerial.print( messageId);
     outSerial.print( "@");

     outSerial.print(results.decode_type);
     outSerial.print("/");
     outSerial.print(results.value, HEX);
     outSerial.print("/");
     outSerial.print(results.address, HEX);
     outSerial.print("/");
     outSerial.println(results.bits);    
   
  } else if (strstr(inBuffer, "sendcode") != 0) {
    Serial.println("Send Code");
//TODO: send code
Serial.println(inBuffer);
    
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
