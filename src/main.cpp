#include <Arduino.h>
#include <SoftwareSerial.h>
#include "protocol.h"

#define PIN_BRX 11
#define PIN_BTX 10

SoftwareSerial BTserial(PIN_BRX, PIN_BTX);
const long baudRateAT = 38400; 
const long baudRate = 9600; 
char c = ' ';
boolean NL = true;

DataEncoder encoder;
char message[64];
uint8_t encodedMessage[64];
unsigned long lastMessage = 0;

uint8_t receivedBytes[64];
uint8_t receivedIndex = 0;

void printByteArray(const uint8_t* arr, size_t length, const char* prefix) {
  Serial.print(prefix);
  for (size_t i = 0; i < length; i++) {
    Serial.print("0x");
    if (arr[i] < 0x10) Serial.print("0");
    Serial.print(arr[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}
 
void setup() 
{
  Serial.begin(baudRate);
  Serial.print("Sketch:   ");   Serial.println(__FILE__);
  Serial.print("Uploaded: ");   Serial.println(__DATE__);
  Serial.println(" ");

  BTserial.begin(baudRate);  
  Serial.print("BTserial started at "); Serial.println(baudRate);
  Serial.println(" ");
}
 
void loop()
{
  if(millis() > lastMessage + 5000)
  {
    sprintf(message, "Hello! Your random number: %d", rand() % 1000 + 1); // [1;1000]
    size_t encodedLen = encoder.encodeString(message, encodedMessage);

    Serial.println("Sending encoded message:");
    printByteArray(encodedMessage, encodedLen, "Sent bytes: ");

    BTserial.write(encodedMessage, encodedLen);
    lastMessage = millis();
  }
 
  while (BTserial.available())
  {
    uint8_t incomingByte = BTserial.read();
    
    if (receivedIndex < sizeof(receivedBytes)) {
      receivedBytes[receivedIndex++] = incomingByte;
    }
    
    // Debug print for each byte
    Serial.print("Received byte: 0x");
    if (incomingByte < 0x10) Serial.print("0");
    Serial.print(incomingByte, HEX);
    Serial.print(" (dec: ");
    Serial.print(incomingByte);
    Serial.println(")");
  }
  
  if (receivedIndex > 0) {
    Serial.println("Complete received byte sequence:");
    printByteArray(receivedBytes, receivedIndex, "Received bytes: ");
    
    BTserial.write(receivedBytes, receivedIndex);
    
    receivedIndex = 0;
  }

  if (Serial.available())
  {
    uint8_t serialByte = Serial.read();
    BTserial.write(serialByte);
    
    Serial.print("Sent from Serial: 0x");
    if (serialByte < 0x10) Serial.print("0");
    Serial.println(serialByte, HEX);
  }
}