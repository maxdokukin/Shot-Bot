#define TERMINATE_TIME 400
#define SHOT_DETECT_INTERVAL 5

class Shot{

  byte pin = 0;
  byte shotPlacedVal = 0;

  int shotPlasedVal = 0;
  
  long fillStartTime = 0;
  long terminationTimer = 0;
  long putGlassWait = 0;
  long lastDetectMeasure = 0;

  bool notFilling = true;
  
  public:enum {NO_GLASS, IN_PROCESS, READY, TERMINATION} stat = 0;
    
  public:Shot(int Tpin){

    pin = Tpin;
  }


  void check(){
    
    bool shotPlaced = detectShot();
    
    switch(stat){

      case NO_GLASS:
      
        if(shotPlaced){
          
          stat = IN_PROCESS;
        }
      break;      

      
      case IN_PROCESS:
        fillShot();
      break;



      case READY:
        
        if(!shotPlaced)
          stat = TERMINATION;

        
      break;



      case TERMINATION:

        if(terminationTimer == 0)
          terminationTimer = millis();
          
        if(shotPlaced) //glass was putted back during termination timer
          terminationTimer = millis();

        if(millis() - terminationTimer >= TERMINATE_TIME){
          
          stat = NO_GLASS;          
          terminationTimer = 0;
          
          buzz(10);
        }
        
      break;
    }
  }
  
  void fillShot(){

    if(notFilling){  //setup
      
      fillStartTime = millis();
      digitalWrite(2, 1);
      notFilling = false;
    }
        
    if(millis() - fillStartTime >= fillTime || digitalRead(pin)){ //termination

      digitalWrite(2, 0);

      stat = READY;
      notFilling = true;
    }
  }


  boolean detectShot(){

    if(millis() - lastDetectMeasure >= SHOT_DETECT_INTERVAL){

      lastDetectMeasure = millis();

      if(!digitalRead(pin) && shotPlacedVal < 40)
        shotPlacedVal ++;
        
      else if(digitalRead(pin) && shotPlacedVal > 0)
        shotPlacedVal --;
    }
    
    return shotPlacedVal >= 30;
  }

};
