/**
 * Knight 2000 Control Server
 * 
 * Part of the Fanhome Knight Rider K.I.T.T. build
 *       
 * Created by TBoy58 - Mar2022
 */

#include <Arduino.h>

#include <CommsBLE.h>
#include <commsI2C.h>

#define MODE_ONE          0
#define MODE_TWO          1
#define MODE_THREE        2
#define MODE_FOUR         3
#define MODE_FIVE         4
#define MODE_SIX          5
#define SCANNER_ON        6
#define SCANNER_OFF       7
#define SPEED_DOWN        8
#define SPEED_UP          9
#define RESET            10

int MODE [6] = {MODE_ONE, 
                MODE_TWO, 
                MODE_THREE, 
                MODE_FOUR, 
                MODE_FIVE, 
                MODE_SIX };
 
bool isScannerOn = false;

int modeIndex = 0;

void setup() {

  startBLEServer();

  startI2CClient();
  
}

void loop() {

  
  switch (kittInstr)
  {
    case 'S': // SCANNER ON/OFF
            if(isScannerOn)
            {
              sendScannerCode(SCANNER_OFF);
              kittInstr = 'Z';
              isScannerOn = false;
              break;
            }

            sendScannerCode(SCANNER_ON);
            delay(1500);
            sendScannerCode(MODE_THREE);
            kittInstr = 'Z';
            isScannerOn = true;
            break;

    case 'Q': // CHANGE SCANNER MODE           
            sendScannerCode(MODE[modeIndex]);  
            if(++modeIndex == 6) modeIndex = 0;
            kittInstr = 'Z';
            break;

    case 'D': // SPEED DOWN
            sendScannerCode(SPEED_DOWN);
            kittInstr = 'Z';
            break;

    case 'U': // SPEED UP
            sendScannerCode(SPEED_UP);
            kittInstr = 'Z';
            break;

    case 'T': // RESET
            sendScannerCode(RESET);
            kittInstr = 'Z';
            break;
  }   
  
}