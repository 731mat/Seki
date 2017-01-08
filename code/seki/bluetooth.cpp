//
// Created by mat on 10.11.16.
//

#include "bluetooth.h"
#include <Arduino.h>


Bluetooth::Bluetooth(){
    btRate = 9600;
    Serial1.begin(btRate);
    Serial.begin(115200);
    //setName("pokus");
    //setPin(123);
    valueDrive = false;
}


void Bluetooth::setPin(int pin){
    boolean res = false;
    Serial.println();
    Serial.print(F("setting pin "));
    Serial.print(pin);
    Serial.println(F("..."));
    writeBT("AT+PIN"+String(pin));
}

void Bluetooth::setName(String name){
    writeBT("AT+NAME SEKI");
}

void Bluetooth::writeBT(String s){
    //Serial.print("send: "+s);
    Serial1.print(s);
}

void Bluetooth::readBT(){
    int buffer = 0;
    if (Serial1.available() > 0) {
        delay(10);
        while (Serial1.available() > 0) { // kdyby náhodou před 'o' == 111 byly nějake nezmane znaky
            buffer = Serial1.read();
            
            switch(buffer){  
                      
              case 111:/// po identifikacnim zanku '0' jsou dva byty jeden rychlost a druhy smer
                killTime = millis();
                buffer = Serial1.read();      // přečte
                if(buffer > -1 && buffer < 101) // oveři zda je to v rozmeti o 0-100
                    this->rychlost = buffer;
                
                buffer = Serial1.read();  // přečte
                if(buffer > -1 && buffer < 181) // overi rozmezi  0 - 100
                  this->uhel = buffer;
                break;

                
              case 101:
                Serial.println("---BLUETOOTH---SET---DRIVE---");
                buffer = Serial1.read();      // přečte
                if(buffer > 0 && buffer < 21) // oveři zda je to v rozmeti o 0-100
                  valueDrive = !valueDrive;
                break;    
            }
        }
    }
}
int Bluetooth::getRychlost()const {
    //if (millis() <= killTime + 700)
        return this->rychlost;
    return 0;
}

int Bluetooth::getUhel() const {
    return this->uhel;
}
bool Bluetooth::getValueDrive() const{
  return this->valueDrive;
  }

void Bluetooth::setValueDrive(bool valueDrive){
    this->valueDrive = valueDrive;
}

bool Bluetooth::isCon(){
    return Serial1;
}
