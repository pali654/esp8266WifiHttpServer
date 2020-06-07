//various utility functions here
#include <Arduino.h>

void blinkLED (int delayMS, int count, const int pinLED);

void blinkLED (int delayMS, int count, const int pinLED) {
    pinMode(pinLED, OUTPUT);
    for (int i = 0; i < count; i++) {            
            digitalWrite(pinLED, LOW);
            delay(delayMS);
            digitalWrite(pinLED, HIGH);
    }
}