/**
 * Example of Class C ABP device
 * Authors: 
 *        Ivan Moreno
 *        Eduardo Contreras
 *        Miguel Romani
 *        Soldered.com
 *  Aug 2021
 * 
 * This code is beerware; if you see me (or any other collaborator 
 * member) at the local, and you've found our code helpful, 
 * please buy us a round!
 * Distributed as-is; no warranty is given.
 * 
 * Modified by soldered.com
 */

#include <lorawan.h>

// ABP Credentials - find yours at https://www.thethingsnetwork.org/ console
// Add your device manually with LoRaWAN 1.0, your area frequency and click on advanced to select class and activation type
const char *devAddr = "00000000";
const char *nwkSKey = "00000000000000000000000000000000";
const char *appSKey = "00000000000000000000000000000000";

const unsigned long interval = 20000; // 10 s interval to send message
unsigned long previousMillis = 0;     // will store last time message sent
unsigned int counter = 0;             // message counter

char myStr[50];
char outStr[255];
byte recvStatus = 0;

// Soldered ESP32 LoRa pins
const sRFM_pins RFM_pins = {
    .CS = 5,
    .RST = 4,
    .DIO0 = 27,
    .DIO1 = 32,
    .DIO2 = 33,
    .DIO5 = -1,
};

void setup()
{
  // Setup loraid access
  Serial.begin(115200);
  delay(5000);
  Serial.println("Start..");
  if (!lora.init())
  {
    Serial.println("RFM95 not detected");
    delay(5000);
    return;
  }

  // Set LoRaWAN Class change CLASS_A or CLASS_C
  lora.setDeviceClass(CLASS_C);

  // Set Data Rate
  lora.setDataRate(SF7BW125);

  // set channel to random
  lora.setChannel(MULTI);

  // Put ABP Key and DevAddress here
  lora.setNwkSKey(nwkSKey);
  lora.setAppSKey(appSKey);
  lora.setDevAddr(devAddr);
}

void loop()
{

  // Check interval overflow
  if (millis() - previousMillis > interval)
  {
    previousMillis = millis();

    sprintf(myStr, "Counter-%d", counter);

    Serial.print("Sending: ");
    Serial.println(myStr);

    lora.sendUplink(myStr, strlen(myStr), 0, 1);
    counter++;
  }

  recvStatus = lora.readData(outStr);
  if (recvStatus)
  {
    Serial.print("====>> ");
    Serial.println(outStr);
  }

  // Check Lora RX
  lora.update();
}