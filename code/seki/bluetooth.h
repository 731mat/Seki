//
// Created by mat on 10.11.16.
//
#ifndef SEKI_BLUETOOTH_H
#define SEKI_BLUETOOTH_H

#include <Arduino.h>

/**
 * @file   bluetooth.h
 * @Author Hloušek Matěj (matej.hlousek@email.cz)
 * @date   Listopad,2016
 *
 * Bluetooth class. Třída přistupuje k bluetooth modulu
 */
class Bluetooth {
    public:
        /**
         * A constructor
         */
        Bluetooth();
        /**
        * Metoda která zapisuje hodnotu v parametru na seriovou linku připojenou k bloutooth modulu
        * @param s type string  string se vypíše na seriovou linku
        */
        void writeBT(String s);
        /**
        * Metoda čte seriovou linku
        */
        void readBT();
        /**
        * Metoda nastaví jméno modulu HC-06
        * @param name type String nazev bluetooth modulu
        */
        void setName(String name);
        /**
        * Metoda nastaví pin modulu HC-06
        * @param pin type int heslo pro bluetooth modul
        */
        void setPin(int pin);
        /**
        * Metoda vrací zda je serial avaible
        * @return bool je serial aveible
        */
        bool isCon();
        getRychlost() const;
        getUhel() const;
private:
        /**
        *  proměnná pro čtení z seriové liky
        */
        char btData;
        /**
        *  hodnota bychlosti seriové liky pro modul bluetooth
        */
        long btRate;

        int rychlost;
        int uhel;
};


#endif //SEKI_BLUETOOTH_H

