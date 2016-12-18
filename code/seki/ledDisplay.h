//
// Created by mat on 27.11.16.
//

#include "LedControl.h"

#ifndef SEKI_LEDDISPLAY_H
#define SEKI_LEDDISPLAY_H

/**
 * @file   ledDisplay.h
 * @Author Hloušek Matěj (matej.hlousek@email.cz)
 * @date   Listopad,2016
 *
 * LedDisplay class. Třída přistupuje k led display a procesoru MAX7219
 */
class LedDisplay {
    public:
        /**
         * A constructor.
         * @param pinDIN pin na kterým posíláme data  DATA IN
         * @param pinCS pin na který
         * @param pinCLK pin na který budeme posílat čas
         * @param countDisplays počet display zapojených v serii když je jeden tak 1
         */
        LedDisplay(int, int, int, int);
        /**
         * Metoda která inicializuje základní nastavení
         */
        void init();
        /**
         * Metoda která nadstavuje privátní proměnné třídy
         * @param type type enum motor smer
         */
        void setSmer(char);
        /**
         * Metoda která nadstavuje privátní proměnné třídy
         * @param sonarDistCenterLeft unsigned int  - naměřená vzdálenost
         * @param sonarDistCenterRight unsigned int  - naměřená vzdálenost
         * @param sonarDistRight unsigned int  - naměřená vzdálenost
         * @param sonarDistLeft unsigned int  - naměřená vzdálenost
         */
        void setSensor(unsigned int,unsigned int,unsigned int,unsigned int);
        /**
         * Metoda která vypíše sensor
         */
        void vypisSensor();



        void printInfo();
        /**
         * Enum pro urční směru jízdy
         * určuje směr jízdy
         */
        enum {
            MOTOR_FRONT,          /**< enum value MOTOR_FRONT. */
            MOTOR_FRONT_LEFT,     /**< enum value MOTOR_FRONT_LEFT. */
            MOTOR_FRONT_RIGHT,    /**< enum value MOTOR_FRONT_RIGHT. */
            MOTOR_BACK,           /**< enum value MOTOR_BACK. */
            MOTOR_BACK_LEFT,      /**< enum value MOTOR_BACK_LEFT. */
            MOTOR_BACK_RIGHT,     /**< enum value MOTOR_BACK_RIGHT. */
            MOTOR_LEFT,           /**< enum value MOTOR_LEFT. */
            MOTOR_RIGHT,          /**< enum value MOTOR_RIGHT. */
            MOTOR_STOP,           /**< enum value MOTOR_STOP. */
        };


    private:

        //s->lc=LedControl(pinDIN,pinCS,pinCLK,countDisplays);
        LedControl lc= LedControl(31,33,35,1);
        /**
        *  vykreslení šipky
        */
        void sipka();
        /**
        *  čas poslední smyčky
        */
        unsigned int lastUpdate;
        /**
        *  směr ulozený
        */
        char smer;
        /**
        *  vzdalenost center left
        */
        unsigned int sonarDistCenterLeft;
        /**
        *  vzdalenost center right
        */
        unsigned int sonarDistCenterRight;
        /**
        *  vzdalenost right
        */
        unsigned int sonarDistRight;
        /**
        *  vzdalenost left
        */
        unsigned int sonarDistLeft;
        /**
        *  počet disaplay v serii
        */
        int countDisplays;
};


#endif //SEKI_LEDDISPLAY_H

