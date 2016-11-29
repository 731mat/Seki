#include "LedControl.h" //  need the library

LedDisplay::LedDisplay(int pinDIN, int pinCS, int pinCLK, int countDisplays){
    lc=LedControl(pinDIN,pinCS,pinCLK,countDisplays);
}
void init()
{
    lc.shutdown(0,false);// turn off power saving, enables display
    lc.setIntensity(0,8);// sets brightness (0~15 possible values)
    lc.clearDisplay(0);// clear screen

    byte seki[4][8]={{B00111100,B01000010,B01000000,B01000000,B00111110,B00000010,B01000010,B00111100},
                  {B01111110,B01000000,B01000000,B01111100,B01000000,B01000000,B01000000,B01111110},
                  {B01000100,B01001000,B01010000,B01100000,B01100000,B01010000,B01001000,B01000100},
                  {B00011000,B00000000,B00011000,B00011000,B00011000,B00011000,B00011000,B00011000}};

    for(j=0;j<5;j++)
    {
        for(i=1;i<9;i++)
            lc.setRow(0,i,seki[j][i]);
        delay(700);
    }
}


void sipka()
{
    byte left[8]={B00000000,B00010000,B00100000,B00111100,B00111100,B00100000,B00010000,B00000000};
    byte right[8]={B00000000,B00001000,B00000100,B00111100,B00111100,B00000100,B00001000,B00000000};
    byte front[8]={B00000000,B00000000,B00111100,B01011010,B00011000,B00011000,B00000000,B00000000};
    byte back[8]={B00000000,B00000000,B00011000,B00011000,B01011010,B00111100,B00000000,B00000000};
    byte front_left[8]={B00000000,B00000000,B01111000,B01100000,B01010000,B01001000,B00000100,B00000000};
    byte front_right[8]={B00000000,B00000000,B00011110,B00000110,B00001010,B00010010,B00100000,B00000000};
    byte resoult[8]={B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000};

    // smazaní dat
    for(int i = 0; i< 8; i++)
        lc.setRow(0,i,resoult[i]);

    switch(this->smer){
        case MOTOR_FRONT:
            resoult = front;
            break;
        case MOTOR_BACK:
            resoult = back;
            break;
        case MOTOR_LEFT:
            resoult = left;
            break;
        case MOTOR_RIGHT:
            resoult = right;
            break;
        case MOTOR_FRONT_LEFT:
            resoult = front_left;
            break;
        case MOTOR_FRONT_RIGHT:
            resoult = front_right;
            break;
    }

    for(int i = 0; i< 8; i++)
        lc.setRow(0,i,resoult[i]);

    vypisSensor();
}

void setSmer(char type){
    this->smer;
    sipka();
}

void setSensor(unsigned int sonarDistCenterLeft, unsigned int sonarDistCenterRight, unsigned int sonarDistRight, unsigned int sonarDistLeft){
    this->sonarDistCenterLeft = sonarDistCenterLeft;
    this->sonarDistCenterRight = sonarDistCenterRight;
    this->sonarDistRight = sonarDistRight;
    this->sonarDistLeft = sonarDistLeft;
    sipka();
}

void vypisSensor(){

    // center left
    // display culonm row true/false
    int i = 0;

    // center left
    i = map(sonarDistCenterLeft,0,4000,0,4);
    for (; i >= 0; i--) {
        lc.setLed(0,0,i,true);
    }
    //center right
    i = map(sonarDistCenterRight,0,4000,0,4);
    for (; i >= 0; i--) {
        lc.setLed(0,7,i,true);
    }

    //right
    i = map(sonarDistRight,0,4000,0,4);
    for (; i >= 0; i--) {
        lc.setLed(0,i,0,true);
    }

    //left
    i = map(sonarDistLeft,0,4000,0,4);
    for (; i >= 0; i--) {
        lc.setLed(0,i,7,true);
    }
}