//
// Created by mat on 6.11.16.
//

#include "ibt.h"
#include <Arduino.h>

Ibt::Ibt(int pinMotorEnable, int pinMotorPWMR, int pinMotorPWML, int pinMotorSense) {
    this->pinMotorEnable = pinMotorEnable;
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
    if(this->value == 0){
        analogWrite(this->pinMotorPWMR, 0);
        analogWrite(this->pinMotorPWML, 0);
        digitalWrite(this->pinMotorEnable, LOW);
        return;
    }
    digitalWrite(this->pinMotorEnable, HIGH);
    if(this->smer == true){
        analogWrite(this->pinMotorPWML, 0);
        analogWrite(this->pinMotorPWMR, this->value);
    } else{
        analogWrite(this->pinMotorPWMR, 0);
        analogWrite(this->pinMotorPWML, this->value);
    }
}

void Ibt::setStop() {
    this->value = 0;
    digitalWrite(this->pinMotorEnable, LOW);
    analogWrite(this->pinMotorPWMR, 0);
    analogWrite(this->pinMotorPWML, 0);
}


bool Ibt::getSmer() { return this->smer; }
int Ibt::getValue() { return this->value; }

