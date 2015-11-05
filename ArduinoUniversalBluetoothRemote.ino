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
char myChar ;
void setup() {
  Serial.begin(9600);   
  Serial.println("AT");
  mySerial.begin(9600);

}
void loop() {
  while (mySerial.available()) {
    myChar = mySerial.read();
    Serial.print(myChar);
  }
 while (Serial.available()) {
    myChar = Serial.read();
    Serial.print(myChar); //echo
    mySerial.print(myChar);
  }
}
