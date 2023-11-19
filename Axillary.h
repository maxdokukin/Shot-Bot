#include <ServoSmooth.h>
#include <SoftwareSerial.h>
#include "FastLED.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "GyverEncoder.h"

//////////////////////////////////////
//===========Parameters=============//
#define SHOT_NUMBER 3
//#define FILL_VOL 25//ml
#define SERVO_HOME_POS 0
#define STANDART_FILL_TIME 5700 
//////////////////////////////////////
//////////////////////////////////////
//==============Pins================//
#define PUMP_POWER 6
#define SERVO_POWER 12
#define SERVO_PIN 8
#define LED_PIN 7
#define BTN_PIN 8
#define DISP_DIO 11
#define DISP_CLK 12
//////////////////////////////////////
//////////////////////////////////////
//===========Global Vars============//
ServoSmooth servo;
CRGB leds[3];
Encoder enc(2, 3, 4);  
LiquidCrystal_I2C lcd(0x3F,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

int fillTime = 0;
boolean acutatorFree = true, servoSt = false, emergencyStop = false;

short fillQue[3] = {-1, -1, -1};
//////////////////////////////////////

void servoON();
void servoOFF();
void welcome(short del);

void setup() {

  pinMode(PUMP_POWER, 1);


  FastLED.addLeds<NEOPIXEL, LED_PIN > (leds, 3);
  FastLED.setBrightness(255);
  
  servoON();
  servo.write(0);
  
  welcome(800); //welcome animation with delay
  
  servo.setTargetDeg(0);
  servo.setSpeed(40);
  servo.setAccel(0.5);  
  servoOFF();
  servo.setAutoDetach(false);
  
  fillTime = 13000;//map(FILL_VOL, 0, 50, 0, STANDART_FILL_TIME);

  lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0, 0);

  enc.setType(TYPE2);

}

//////////////////////////////////////
//===========Que Functions==========//
void addToQue(byte id){

  int freeIndex = 0;
  for(; fillQue[freeIndex] != -1 && freeIndex < SHOT_NUMBER; freeIndex++);
  fillQue[freeIndex] = id;
}

void moveQue(){
  
  for(int i = 1; i < SHOT_NUMBER; i++)
    fillQue[i - 1] = fillQue[i];

  fillQue[SHOT_NUMBER - 1] = -1;  
}
//////////////////////////////////////
//////////////////////////////////////
//==========Servo Functions=========//
boolean servoAtPos(int goalPos){

  servo.tick();
  
  if(servo.getCurrentDeg() == goalPos - 1)
    return true;
    
  else{
    return false;
  }
}

void moveHome(){
  if(fillQue[0] == -1  && acutatorFree){ 
    if(servo.getCurrentDeg() != SERVO_HOME_POS){
      
      servo.setTargetDeg(SERVO_HOME_POS);
      servo.tick();
    }
    else if(servoSt)
      servoOFF();
  }
}

void servoON(){ 
  digitalWrite(SERVO_POWER, 1); 
  servo.attach(SERVO_PIN);
  servoSt = true;
}

void servoOFF(){ 
  digitalWrite(SERVO_POWER, 0); 
  servoSt = false;
}
//////////////////////////////////////
//////////////////////////////////////
//===========LED Functions==========//
short colCon(long hexValue, char color)
{    
    if(color == 'R')        
      return ((hexValue >> 16) & 0xFF);    
    if(color == 'G')
        return ((hexValue >> 8) & 0xFF);
    if(color == 'B')
      return ((hexValue) & 0xFF);
}
void updateColors(long col1, long col2, short colors[2][3]){
    
  colors[0][0] = colCon(col1, 'R');
  colors[0][1] = colCon(col1, 'G');
  colors[0][2] = colCon(col1, 'B');
  colors[1][0] = colCon(col2, 'R');
  colors[1][1] = colCon(col2, 'G');
  colors[1][2] = colCon(col2, 'B'); 
}
//////////////////////////////////////
//////////////////////////////////////
//===========Axl Functions==========//
void pumpPower(boolean t){

  digitalWrite(PUMP_POWER, t);
}

bool pr_btn_st = false;

void checkButton(){

  boolean butState = !digitalRead(BTN_PIN);

  if(pr_btn_st != butState){

    pr_btn_st = butState;
    
    if(butState){  //if button is pressed
        
      if(acutatorFree){   //shot is not filling
        
        pumpPower(true);  //turn on pump
      }
      else{               //shot is filling
        
        pumpPower(false);
        emergencyStop = true; //stop dat shit
      }
    }
    else if(digitalRead(PUMP_POWER) && !butState && acutatorFree){ //if button is not pressed
      
        pumpPower(false);
    }
  }
}

void welcome(short del){

  leds[0] = 0x00ff00;
  FastLED.show();
  delay(del / 4);
  
  leds[0] = 0x00ffff;
  leds[1] = 0x00ff00;
  FastLED.show();
  delay(del / 4);

  leds[0] = 0x00ff00;
  leds[1] = 0x00ffff;
  leds[2] = 0x00ff00;
  FastLED.show();
  delay(del / 4);

  leds[0] = 0x00ff00;
  leds[1] = 0x00ff00;
  leds[2] = 0x00ff00;
  FastLED.show();
  delay(del / 4);
}
