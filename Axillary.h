long buzzTill = 0;
bool buzzing = false;

void checkBuzz(){

  if(buzzTill > millis() && !buzzing){
    
    tone(BUZZ_PIN, 1000);
    buzzing = true;
  }

  if(buzzTill <= millis() && buzzing){

    noTone(BUZZ_PIN);
    buzzing = false;
  }

  
}


void buzz(int t){

  buzzTill = millis() + t;
}


void updateMemory(){

  EEPROM.write(0, fillTime >> 8);
  EEPROM.write(1, fillTime & 0xFF);
  
}


int loadFromMemory(){
  
  return (EEPROM.read(0) << 8) + EEPROM.read(1);
}


void checkEnc(){

  enc.tick();

  if (enc.isRight()){
    
    fillTime += 100;
    buzz(50);

    Serial.println(fillTime);
  }
  
  if (enc.isLeft()){
    
    fillTime -= 100;
    buzz(50);

        Serial.println(fillTime);

  }

  if(enc.isClick()){
    
    updateMemory();
    buzz(500);

        Serial.println(fillTime);

  }

  if(enc.isDouble()){

    fillTime = 13500;
    updateMemory();
    buzz(1000);

        Serial.println(fillTime);

  }
}
