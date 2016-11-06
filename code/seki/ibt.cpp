//
// Created by mat on 6.11.16.
//

#include "ibt.h"
#include <Arduino.h>

Ibt::Ibt(int pinMotorEnableR, int pinMotorEnableL, int pinMotorPWM, int pinMotorSense) {
    this->pinMotorEnableR = pinMotorEnableR;
    this->pinMotorEnableL = pinMotorEnableL;
    this->pinMotorPWM = pinMotorPWM;
    this->pinMotorSense = pinMotorSense;
}

void Ibt::setData(bool smer, int value) {
    this->smer = smer;
    if(value >= 0 && value <= 255)
        this->value = value;
    else
        this->value = 0;
}

void Ibt::aplikovat() {
    if(this->smer == true){
        digitalWrite(this->pinMotorEnableL, LOW);
        digitalWrite(this->pinMotorEnableR, HIGH);
        analogWrite(this->pinMotorPWM, this->value);
    } else{
        digitalWrite(this->pinMotorEnableL, HIGH);
        digitalWrite(this->pinMotorEnableR, LOW);
        analogWrite(this->pinMotorPWM, this->value);
    }
    Serial.println("MOTOR SET");
}

void Ibt::setStop() {
    digitalWrite(this->pinMotorEnableL, LOW);
    digitalWrite(this->pinMotorEnableR, LOW);
    analogWrite(this->pinMotorPWM, 0);
}

