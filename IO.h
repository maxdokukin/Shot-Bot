

class IO{

  bool updatedVal = true;
  
  public:void check(){

    checkEnc();
    
    if(updatedVal){

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(fillTime);
      
      updatedVal = false;
    }
    
    
  }

  private:void updateFillTime(int add){

    if(fillTime + add >= 0)
      fillTime += add; 
  }

  private:void checkEnc(){

      enc.tick();

      if (enc.isRightH()) {
        
        updateFillTime(50);
        updatedVal = true;
      }
        
      if (enc.isLeftH()){
        
        updateFillTime(-50);
        updatedVal = true;

      }

  }
};
