class LED{

  byte id = 0;
  byte lastMode = 0;
  bool modeChanged = true;
  long refTime = 0;
  int fadeDel = 0;

  short colors[2][3];
  
  public:LED(byte n){

    id = n;
  }

  public:void check(byte shot_mode){

    if(lastMode != shot_mode){

      if(shot_mode != 4) // no mode between SERVO_SHAKE and SERVO_WAIT
        modeChanged = true;
        
      lastMode = shot_mode;
    }
    
    switch(shot_mode){

      case(0): //no glass
      fadeMode(0xff00ff, 0x00ffff, 2000); //purple to neon blue 
      break;
      
      case(1): //moving to glass
      fadeMode(0xff00ff, 0x330033, 300); //blinking purple
      break;

      case(2): //filling
      fadeMode(0x00ff00, 0xff00ff, 700); //green to purple
      break;      
      
      case(3): //shaking
      fadeMode(0x00ffff, 0x003333, 300); //blinking neon blue
      break;        
      
      case(4): //shaking
      fadeMode(0x00ffff, 0x003333, 300); //blinking neon blue
      break;      
      
      case(5): //ready
      fadeMode(0x00ff00, 0x001100, 1000); //blinking green 
      break;
            
      case(6): //safe time
      fadeMode(0xffff00, 0x333300, 300); //blinking yellow
      break;
    }
  }

  public:void fadeMode(long col1, long col2, int del){

    if(modeChanged){

      updateColors(col1, col2, colors);
      fadeDel = del;
      
      modeChanged = false;
      refTime = millis();
    }
    
    float mpl = map(millis() - refTime, 0, fadeDel, 0, 1000) / (float) 1000; //multiplier (0 - 1 range) with the reference to time

    if(millis() - refTime < fadeDel){

      
      leds[id].setRGB(colors[0][0] * (1 - mpl)  + colors[1][0] * mpl,
                     colors[0][1] * (1 - mpl)  + colors[1][1] * mpl,
                     colors[0][2] * (1 - mpl)  + colors[1][2] * mpl);
                     
    }
    else if(millis() - refTime < fadeDel + fadeDel){ // -1 because mpl range (1 - 2)
      
      leds[id].setRGB(colors[0][0] * (mpl - 1)  + colors[1][0] * (2 - mpl),
                     colors[0][1] * (mpl - 1)  + colors[1][1] * (2 - mpl),
                     colors[0][2] * (mpl - 1)  + colors[1][2] * (2 - mpl));
    }
    else
      refTime = millis();
  }
};
