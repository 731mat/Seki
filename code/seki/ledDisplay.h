//
// Created by mat on 27.11.16.
//

#ifndef SEKI_LEDDISPLAY_H
#define SEKI_LEDDISPLAY_H


class LedDisplay {
    public:
        void init();
        void setSmer();
        void setSensor();


    private:
        LedControl lc;

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

