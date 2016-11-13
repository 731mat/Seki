//
// Created by mat on 10.11.16.
//

#include "bluetooth.h"
#include <Arduino.h>



Bluetooth::Bluetooth(){
    btRate = 9600;
    Serial1.begin(btRate);
    Serial.begin(19200);
    setName("pokus");
    //setPin(123);
}


void Bluetooth::setPin(int pin){
    boolean res = false;
    Serial.println();
    Serial.print(F("setting pin "));
    Serial.print(pin);
    Serial.println(F("..."));
    writeBT("AT+PIN"+String(pin));
    res = (btResult.startsWith("OKsetPIN"));
    if (res) Serial.println(F("=>success"));
    else Serial.println(F("=>error setting pin"));
}

void Bluetooth::setName(String name){
    writeBT("AT+NAME SEKI");
}

void Bluetooth::writeBT(String s){
    //Serial.print("send: "+s);
    Serial1.print(s);
}

char Bluetooth::readBT(){
    if (Serial1.available()){
        btData=Serial1.read();
        return(char(btData));
    }
    return 'M';
}

bool Bluetooth::isCon(){
    return Serial1;
}
