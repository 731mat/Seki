//
// Created by mat on 6.11.16.
//

#include "ibt.h"
#include <Arduino.h>

Ibt::Ibt(int pinMotorLeftEnableR, int pinMotorEnableL, int pinMotorPWMR, int pinMotorPWML, int pinMotorSense) {
    this->pinMotorEnableR = pinMotorEnableR;
    this->pinMotorEnableL = pinMotorEnableL;
    this->pinMotorPWML = pinMotorPWML;
    this->pinMotorPWMR = pinMotorPWMR;
    this->pinMotorSense = pinMotorSense;
}

void Ibt::setData(bool smer, int value) {
    if(smer == this->smer && value == this->value)
        return 0;

    this->smer = smer;
    if(value >= 0 && value <= 255)
        this->value = value;
    else
        this->value = 0;
    aplikovat();
}

void Ibt::aplikovat() {
    if(this->smer == true){
        digitalWrite(this->pinMotorEnableL, LOW);
        digitalWrite(this->pinMotorEnableR, HIGH);
        analogWrite(this->pinMotorPWMR, this->value);
    } else{
        digitalWrite(this->pinMotorEnableL, HIGH);
        digitalWrite(this->pinMotorEnableR, LOW);
        analogWrite(this->pinMotorPWML, this->value);
    }
}

void Ibt::setStop() {
    digitalWrite(this->pinMotorEnableL, LOW);
    digitalWrite(this->pinMotorEnableR, LOW);
    analogWrite(this->pinMotorPWMR, 0);
    analogWrite(this->pinMotorPWML, 0);
}


bool Ibt::getSmer() { return this->smer; }
int Ibt::getValue() { return this->value; }

