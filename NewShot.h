#include "Axillary.h"
#include "LED.h"

#define SHAKE_ANG 2
#define TERMINATE_TIME 600
#define SHAKE_TIME 500
#define WAIT_AFTER_SHAKE 300
#define SHOT_DETECT_INTERVAL 5 //shot detection time is *50

class Shot{

  int pos = 0;
  byte pin = 0;
  byte id = -1;
  byte shotPlacedVal = 0;
  
  long fillStartTime = 0;
  long terminationTimer = 0;
  long ledTime = 0;
  long shakeStartTime = 0;
  long lastDetectMeasure = 0;

  public:bool active = false;
  bool runServo = false;
  bool shakeDirection = false;
  bool shotDetecting = false;
  bool shotPlaced = false;
  
  public:enum {NO_GLASS, EMPTY, IN_PROCESS, SHAKE, SHAKE_WAIT, READY, TERMINATION} stat;
    
  public:Shot(byte Tpin, int Tid, int Tpos){

    id = Tid;
    pin = Tpin;
    pos = Tpos;
  }


  void check(){

    shotPlaced = detectShot();
    
    if(runServo)
      servo.tick();
      
    switch(stat){

      case NO_GLASS:
        if(shotPlaced){
          
          stat = EMPTY;
          addToQue(id);
        }
      break;
      
      case EMPTY:
        if(active){

          if(acutatorFree)
            acutatorFree = false;

          if(!servoSt) //turn on servo if turned off
            servoON();

          if(!runServo){ //set servo goal pos once 
            
            servo.setTargetDeg(pos);
            runServo = true;
          }

          if(servoAtPos(pos))  //if servo reached pos, fill shot
            fillShot();

          if(!shotPlaced){
            stat = READY;
          }
        }
      break;
      
      case IN_PROCESS:
        fillShot(); //:)
      break;

      case SHAKE:
        
        if(shakeStartTime == 0){  //setup
          
          shakeStartTime = millis();
          servo.setTargetDeg(pos - SHAKE_ANG);
        }

        if(servoAtPos(pos - SHAKE_ANG) || servoAtPos(pos + SHAKE_ANG)){ //shake direction switching
          
          shakeDirection = !shakeDirection;

          servo.setTargetDeg(shakeDirection ? pos + SHAKE_ANG : pos - SHAKE_ANG);
        }

        if(millis() - shakeStartTime >= SHAKE_TIME){   //termination

          servo.setTargetDeg(pos);

          shakeStartTime = millis();

          stat = SHAKE_WAIT;
        }
      break;

      case SHAKE_WAIT:
        if(analogRead(pin) > 512 || millis() - shakeStartTime >= WAIT_AFTER_SHAKE){

          shakeStartTime = 0;
          stat = READY;
        }

      break;

      case READY:
        if(active){ //deactivate local
          
          active = false;
          acutatorFree = true;
        }

        if(!shotPlaced){
          
          stat = TERMINATION;
          terminationTimer = millis();
        }
          
      break;

      case TERMINATION:
        
        if(shotPlaced) //glass was putted back during termination timer
          terminationTimer = millis();

        if(millis() - terminationTimer >= TERMINATE_TIME){
          
          stat = NO_GLASS;
          runServo = false;
          
          shakeDirection = 0;
        }
      break;
    }
  }
  
  void fillShot(){

    if(stat == EMPTY){  //setup
      
      fillStartTime = millis();
      pumpPower(true);

      stat = IN_PROCESS;
    }
        
    if(millis() - fillStartTime >= fillTime || !shotPlaced){ //termination

      pumpPower(false);  

      stat = SHAKE;
    }
  }

  boolean detectShot(){

    if(millis() - lastDetectMeasure >= SHOT_DETECT_INTERVAL){

      lastDetectMeasure = millis();

      if(analogRead(pin) < 127 && shotPlacedVal < 60)
        shotPlacedVal ++;
      else if(analogRead(pin) > 127 && shotPlacedVal > 0)
        shotPlacedVal --;
    }
    
    return shotPlacedVal >= 50;
  }
};
