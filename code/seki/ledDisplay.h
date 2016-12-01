//
// Created by mat on 27.11.16.
//

#include "LedControl.h"

#ifndef SEKI_LEDDISPLAY_H
#define SEKI_LEDDISPLAY_H


class LedDisplay {
    public:
        LedDisplay(int, int, int, int);
        void init();
        void setSmer(char);
        void setSensor(unsigned int,unsigned int,unsigned int,unsigned int);
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
        LedControl lc= LedControl(12,11,10,1);
        void sipka();
        void vypisSensor();

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
        int countDisplays;
};


#endif //SEKI_LEDDISPLAY_H

