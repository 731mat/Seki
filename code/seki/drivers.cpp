//
// Created by mat on 6.11.16.
//

#include "drivers.h"
#include <Arduino.h>

unsigned int readHCSR04(int triggerPin, int echoPin){
    unsigned int uS;
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);
    uS = pulseIn(echoPin, HIGH, MAX_ECHO_TIME + 1000);
    if (uS > MAX_ECHO_TIME) uS = 5000;
    else if (uS < MIN_ECHO_TIME) uS = NO_ECHO;
    return uS;
    //float distance = pulseIn(ECHOPIN, HIGH);
    //distance= distance*0.017315f;
    //Serial.print(distance);
    //Serial.print("cm\n");
}

void motorMower(int pin,int value){
    if(value >= 0 && value <= 255)
        analogWrite(pin, value);
    else
        analogWrite(pin, 0);
}