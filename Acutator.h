#include "NewShot.h"

class Acutator{

  Shot shots[SHOT_NUMBER] = {Shot(2, 0, 35), Shot(1, 1, 87), Shot(0, 2, 147)}; //shots declaration
  LED led_obj[SHOT_NUMBER] = {LED(0), LED(1), LED(2)};

  public:void checkShots(){

    for(int i = 0; i < SHOT_NUMBER; i++) //check shots
      shots[i].check();

    for(int i = 0; i < SHOT_NUMBER; i++)
      led_obj[i].check(shots[i].stat); //check led and give em shot current state
      
    if(fillQue[0] != -1 && acutatorFree){  //there are shots in que
      
      shots[fillQue[0]].active = true;     //make first shot in a que active
       
      moveQue();
    }

    moveHome(); //check and move home if possible

    if(servoSt) //if servo on run servo tick
      servo.tick();

    FastLED.show();
  }

  public:void setLedToRed(){
    
    for(int i = 0; i < SHOT_NUMBER; i++)
      leds[i].setRGB(255, 0, 0);

    FastLED.show();
  }  
};
