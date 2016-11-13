//
// Created by mat on 10.11.16.
//
#ifndef SEKI_BLUETOOTH_H
#define SEKI_BLUETOOTH_H

#include <Arduino.h>


class Bluetooth {
    public:
        Bluetooth();
        void writeBT(String s);
        char readBT();
        void setName(String name);
        void setPin(int pin);
        bool isCon();
        
        byte btConfig;
        char btType;
        char btData;
        String btResult;
        long btRate;

};


#endif //SEKI_BLUETOOTH_H

