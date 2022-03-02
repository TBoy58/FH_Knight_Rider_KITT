/**
 * Knight Rider (actually K.I.T.T.) 8xLED scanner bar
 * 
 * Petr Stehlik in December 2015
 *
 * petr@pstehlik.cz
 *
 * licensed under the GPLv3
 * 
 * Adapted by TBoy58 - 2022
 */

#include <TimerOne.h>

#define PWM_PINS    8
#define START_PIN   2
#define NO_OF_MODES 6

#define FAST_WRITE  true
#define LED_TO_PLUS false

volatile bool isDoOnceLEDOn; 

volatile bool leds[PWM_PINS];
volatile byte pwm_regs[PWM_PINS];
volatile byte kittMode;
volatile byte kittIndex;
volatile byte kittSpeed;

void setup()
{
    for(byte i = START_PIN; i < START_PIN+PWM_PINS; i++) {
        pinMode(i, OUTPUT);
    }
    LEDoff();

    setKittMode(6, 25);

    Timer1.initialize(64);  // 15625 Hz => 8bit PWM 61 Hz refresh rate
    Timer1.attachInterrupt(myIrq);
    
    delay(2000);

    setKittMode(2, 30);
    
}

void LEDoff()
{
    for(byte i = START_PIN; i < START_PIN+PWM_PINS; i++) {
        digitalWrite(i, LED_TO_PLUS ? HIGH : LOW);
    }
}

void loop()
{   for(byte m = 0; m < NO_OF_MODES; m++) {
        delay(10000);
        setKittMode(m, 30);
    }
}

void setKittMode(byte mode, byte speed)
{
    if (mode != kittMode) {
        if(mode == 6) isDoOnceLEDOn = 1;
        kittMode = -1;
        kittIndex = 0;
        LEDoff();
        kittMode = mode;
    }
    kittSpeed = speed;
}

void nextKittStep()
{
    static bool dirRight;

    switch(kittMode) {
        case 0: leds[kittIndex] = LOW;
                if (++kittIndex >= PWM_PINS) kittIndex = 0;
                leds[kittIndex] = HIGH;
                break;

        case 1: leds[kittIndex] = LOW;
                if (--kittIndex >= PWM_PINS) kittIndex = PWM_PINS-1;
                leds[kittIndex] = HIGH;
                break;

        case 2: leds[kittIndex] = LOW;
                if (dirRight && ++kittIndex >= PWM_PINS) { kittIndex = PWM_PINS-2; dirRight = false; }
                else if (!dirRight && --kittIndex >= PWM_PINS) { kittIndex = 1; dirRight = true; }
                leds[kittIndex] = HIGH;
                break;

        case 3: leds[kittIndex] = leds[PWM_PINS-1 - kittIndex] = LOW;
                if (++kittIndex >= PWM_PINS/2) kittIndex = 0;
                leds[kittIndex] = leds[PWM_PINS-1 - kittIndex] = HIGH;
                break;

        case 4: leds[kittIndex] = leds[PWM_PINS-1 - kittIndex] = LOW;
                if (--kittIndex >= PWM_PINS) kittIndex = PWM_PINS/2-1;
                leds[kittIndex] = leds[PWM_PINS-1 - kittIndex] = HIGH;
                break;

        case 5: leds[kittIndex] = leds[PWM_PINS-1 - kittIndex] = LOW;
                if (dirRight && ++kittIndex >= PWM_PINS/2) { kittIndex = PWM_PINS/2-2; dirRight = false; }
                else if (!dirRight && --kittIndex >= PWM_PINS) { kittIndex = 1; dirRight = true; }
                leds[kittIndex] = leds[PWM_PINS-1 - kittIndex] = HIGH;
                break;

        case 6: for(byte i = 0; i < PWM_PINS; i++) {
                            leds[i] = LOW;                      
                      }                              
                if(isDoOnceLEDOn){
                      for(byte i = 0; i < PWM_PINS; i++) {
                            leds[i] = HIGH;
                      }
                      isDoOnceLEDOn = 0;
                 }
    }
}

void myIrq(void)
{
    static byte counter = 0;
    if (counter % 64 == 0)
        autoScan();
    if (!counter++)
        fadeOutEffect();
    softPWM();
}

void autoScan(void)
{
    static byte shadow;
    if (!shadow) {
        shadow = kittSpeed;       
    }
    if (--shadow == 0)
        nextKittStep();
}

void softPWM(void)
{
    static byte counter = 0;
    static byte shadows[PWM_PINS];

    if (!counter++) {
        for(byte i = 0; i < PWM_PINS; i++) {
            shadows[i] = pwm_regs[i];
        }
        counter++;
    }

#if FAST_WRITE
    byte state = 0;
#endif
    for(byte i = 0; i < PWM_PINS; i++) {
        bool b = false;
        if (shadows[i]) {
            shadows[i]--;
            b = true;
        }
#if FAST_WRITE
        state = state << 1 | b;
#else
        digitalWrite(START_PIN + i, LED_TO_PLUS ? !b : b);
#endif
    }
#if FAST_WRITE
# if LED_TO_PLUS
    PORTD = (PORTD & 0x03) | ~(state << 2);
    PORTB = (PORTB & ~0x03) | ~(state >> 6);
# else
    PORTD = (PORTD & 0x03) | (state << 2);
    PORTB = (PORTB & ~0x03) | (state >> 6);
# endif
#endif
}

void fadeOutEffect(void)
{
    for(byte i = 0; i < PWM_PINS; i++) {
        if (leds[i] && pwm_regs[i] != 255) {
            unsigned x = pwm_regs[i] + 64;  // quickly light up (in 4 steps)
            if (x > 255) x = 255;
            pwm_regs[i] = x;
        }
        else if (!leds[i] && pwm_regs[i]) {
            pwm_regs[i] = pwm_regs[i] * 15 / 16; // very slowly fade out (in 54 steps)
        }
    }   
}
