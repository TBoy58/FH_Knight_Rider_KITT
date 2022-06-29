/*
*        Title: KITT_MONITORS
*
*        Author: TBoy58
*
*        Description: Animations for Fanhome Knight Rider KITT console monitors
*                     
*        Dependencies: kitt.h - holds frames for kitt_rotate
*
*        Platform: ESP32
*                      
*        Format: Arduino
*
*        Board: Dev Kit WROOM
*
*        Modified: 29Jun22
*
************************************************************************************
*/  
  
  #include "kitt.h"
  
  #include <SPI.h>
  #include <TFT_eSPI.h> // Hardware-specific library
  TFT_eSPI tft = TFT_eSPI();  
  
  void kitt_rotate();
  void kitt_id();
  
  
  void setup() {
    Serial.begin(115200);
    tft.init();
    tft.setRotation(1);  
    tft.setSwapBytes(true);
    tft.fillScreen(TFT_BLACK);
  }
  
  void loop() 
  {
    kitt_rotate();
    kitt_id();  
  }

/*=============================================================
 *    KITT_ROTATE - DISPLAYS ROTATING KITT CAR IN LEFT MONITOR
 *=============================================================    
 */
  void kitt_rotate()
  {
    static byte rot_frame    = 7;
    static byte hold_set     = 5;
    static byte hold_frame   = 14;
    static byte frame_index  = 0;

    static bool isHold      = true;

    static unsigned long frame_millis = millis();

    // Pass guard every 100 milliseconds
    if(!(millis() - frame_millis >= 100)) return;

    if(!rot_frame)
    { // 4 - Display 8th frame and trigger hold frame (back to 1)
      ++frame_index;
      tft.pushImage(15, 30,animation_width  , animation_height, rotate[frame_index]);
      rot_frame = 7;
      isHold = true;
      ++frame_index;
      frame_millis = millis();
      return;      
    }

    if(!isHold)
    { // 3 - Display the first 7 of 8 frames of each rotation
      ++frame_index;
      tft.pushImage(15, 30,animation_width  , animation_height, rotate[frame_index]);
      --rot_frame;
      frame_millis = millis();
      return;
    }

    // 1 - Repeat every 9th frame 14 times including first and last of 37 frames
    tft.pushImage(15, 30,animation_width  , animation_height, rotate[frame_index]);
    --hold_frame;

    if(!hold_frame)
    { // 2 - Trigger each car rotation. Trigger frame reset after last hold frame
      hold_frame = 14;
      isHold = false;
      --hold_set;
    }

    if(!hold_set)
    { // 5 - All frames done, reset frame count
      frame_index = 0;
      hold_set= 5;
      isHold = true;
    }

    frame_millis = millis();                        
  }
/*=============================================================
 *          END KITT_ROTATE
 *=============================================================    
 */

/*=============================================================
 *          KITT_ID - KITT IDENTIFICATION IN RIGHT MONITOR
 *=============================================================    
 */
  void kitt_id()
  {
    static String id[51] = {"K","KN","KNI","KNIG","KNIGH","KNIGHT",
                            "I","IN","IND","INDU","INDUS","INDUST","INDUSTR",
                            "INDUSTRI","INDUSTRIE","INDUSTRIES",
                            "2","20","200","2000",
                            "S","SE","SER","SERI","SERIA","SERIAL","SERIAL ",
                            "SERIAL N","SERIAL NU","SERIAL NUM","SERIAL NUMB",
                            "SERIAL NUMBE","SERIAL NUMBER","SERIAL NUMBER:",
                            "A","AL","ALP","ALPH","ALPHA","ALPHA ","ALPHA D",
                            "ALPHA DE","ALPHA DEL","ALPHA DELT","ALPHA DELTA",
                            "2","22","227","2275","22752","227529"};

    static byte           x = 180;
    static byte           y = 16;
    static byte start_delay = 0; // max - 100
    static byte    id_delay = 200;
    static byte     idIndex = 0; // max - 51

    static unsigned long id_millis = millis();

    // Pass guard every 100 milliseconds
    if(!(millis() - id_millis >= 100)) return;

    // Delay id display by 10 seconds
    if(!(start_delay == 100)) {++start_delay; id_millis = millis(); return;}

    if(!id_delay)
    { // 3 - Clear the id and reset for next display
      tft.fillRect(150,15,90,90,TFT_BLACK);
      id_delay = 200;
      start_delay = 0;
      idIndex = 0;
      x = 180;
      y = 16;
      id_millis = millis();
      return;
    }

    // 2 - Once id fully displayed, hold it for 20 seconds before reset
    if(!(idIndex < 51)) {--id_delay; id_millis = millis(); return;}

    // 1 - Display id
    tft.drawString(id[idIndex],x,y);
    switch (++idIndex)
    {
        case 6: 
                x = 166;
                y = 28;
                break;

        case 16: 
        
                x = 186;
                y = 40;                
                break;
                
        case 20: 
                
                x = 156;
                y = 52;               
                break;
                
        case 34: 
        
                x = 162;
                y = 64;                
                break;
                
        case 45: 
        
                x = 180;
                y = 76;                
                break;             
        default:
                break;
    }
    id_millis = millis();
  }
/*=============================================================
 *          END KITT_ID
 *=============================================================    
 */

  
