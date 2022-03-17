  
  #include <Wire.h>
  
  #include <DigitalPin.h>  
  
  #define MODE_ONE          0
  #define MODE_TWO          1
  #define MODE_THREE        2
  #define MODE_FOUR         3
  #define MODE_FIVE         4
  #define MODE_SIX          5
  #define SCANNER_ON        6
  #define SCANNER_OFF       7
  
  #define I2C_SCANNER_ADDRESS 9
  
  int MODE [8] = {SCANNER_ON, 
                    MODE_ONE, 
                    MODE_TWO, 
                    MODE_THREE, 
                    MODE_FOUR, 
                    MODE_FIVE, 
                    MODE_SIX, 
                    SCANNER_OFF};
  
  int modeIndex = 0;
  
  DigitalPin MODE_SW(16);    // A2
    
  void setup() {
  
     MODE_SW. beginPULLUP();  
  
     Wire.begin();  
  }
  
  void loop() {
  
  if (! MODE_SW.readState())
    {
        delay(1000);

        if(modeIndex == 0)
        {
          sendScannerMode(MODE[modeIndex]);
          delay(2000);
          sendScannerMode(MODE[3]);
          ++modeIndex;
        }
        else
        {
        
          sendScannerMode(MODE[modeIndex]);
  
          if(++modeIndex == 8) modeIndex = 0;
        }
    }
  
  }
  
  void sendScannerMode(int h) {
    byte c;
      c=h;
      Wire.beginTransmission(I2C_SCANNER_ADDRESS);   // slave device #9
      Wire.write(c);
      Wire.endTransmission();
    }
  
  
  
  
  
  
  
  
  
  
  
  
