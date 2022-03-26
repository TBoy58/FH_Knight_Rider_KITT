#include <Arduino.h>
#include <Wire.h>

#define SDA_PIN 21
#define SCL_PIN 22

#define I2C_SCANNER_ADDRESS 9

void startI2CClient()
{
    Wire.begin(SDA_PIN, SCL_PIN);
}
 

void sendScannerCode(int m)
{
    byte c;
    c=m;
    Wire.beginTransmission(I2C_SCANNER_ADDRESS);   // slave device #9
    Wire.write(c);
    Wire.endTransmission();

}
