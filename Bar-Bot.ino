#include "GyverEncoder.h"
#include <EEPROM.h>

#define PUMP_POWER 2
#define BUZZ_PIN 9
#define SENS_PIN 3

Encoder enc(6, 5, 4);  

int fillTime = 0;

#include "Axillary.h"
#include "NewShot.h"
Shot *shot;


void setup() {

  pinMode(PUMP_POWER,OUTPUT);
  pinMode(SENS_PIN, INPUT_PULLUP);
  pinMode(BUZZ_PIN, OUTPUT);

  Serial.begin(9600); 
  
  shot = new Shot(SENS_PIN);

  fillTime = loadFromMemory();

  Serial.println(fillTime);

  enc.setType(TYPE2);
  
  buzz(100);
}



void loop() {
  
  checkEnc();

  checkBuzz();
  
  shot->check(); 

  delay(4);
}
