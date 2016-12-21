#include "LedControl.h" //  need the library
#include "ledDisplay.h"

#define SONARMAX 4000
#define SONARMIN 0

LedDisplay::LedDisplay(int pinDIN, int pinCS, int pinCLK, int countDisplays){
    this->lc=LedControl(31,33,35,1);
}
void LedDisplay::init()
{
    this->lc.shutdown(0,false);// turn off power saving, enables display
    this->lc.setIntensity(0,8);// sets brightness (0~15 possible values)
    this->lc.clearDisplay(0);// clear screen

    byte seki[4][8]={
                  {B00111100,B01000010,B01000000,B01000000,B00111110,B00000010,B01000010,B00111100}, // S
                  {B01111110,B01000000,B01000000,B01111100,B01000000,B01000000,B01000000,B01111110}, // E
                  {B01000100,B01001000,B01010000,B01100000,B01100000,B01010000,B01001000,B01000100}, // K
                  {B00011000,B00000000,B00011000,B00011000,B00011000,B00011000,B00011000,B00011000}};// I

    for(int j=0;j<5;j++)
    {
        for(int i=0;i<9;i++)
            lc.setRow(0,i,seki[j][i]);
        delay(100);
    }
}

void LedDisplay::printInfo(){
    sipka();
    vypisSensor();
}

void LedDisplay::sipka()
{
    int index = 8;
    switch (this->smer){
        case MOTOR_FRONT:       index = 0;   break;
        case MOTOR_FRONT_LEFT:  index = 4;   break;
        case MOTOR_FRONT_RIGHT: index = 5;   break;
        case MOTOR_BACK:        index = 1;   break;
        case MOTOR_BACK_LEFT:   index = 7;   break;
        case MOTOR_BACK_RIGHT:  index = 6;   break;
        case MOTOR_LEFT:        index = 2;   break;
        case MOTOR_RIGHT:       index = 3;   break;
        default:                index = 8;   break;
    }
   
    byte pole[9][8]={
            {B00000000,B00001000,B00000100,B00111100,B00111100,B00000100,B00001000,B00000000}, // front
            {B00000000,B00010000,B00100000,B00111100,B00111100,B00100000,B00010000,B00000000}, // back
            {B00000000,B00000000,B00111100,B01011010,B00011000,B00011000,B00000000,B00000000}, // left
            {B00000000,B00000000,B00011000,B00011000,B01011010,B00111100,B00000000,B00000000}, // right
            {B00000000,B00000000,B00111100,B00001100,B00010100,B00100100,B01000000,B00000000}, // front_left
            {B00000000,B01000000,B00100100,B00010100,B00001100,B00111100,B00000000,B00000000}, // front_right
            {B00000000,B00000010,B00100100,B00101000,B00110000,B00111100,B00000000,B00000000}, // back_rig
            {B00000000,B00000000,B00111100,B00110000,B00101000,B00100100,B00000010,B00000000}, // back_lef
            {B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000}}; // resoult
              
    this->lc.clearDisplay(0);
    for(int i = 0; i< 9; i++)
        this->lc.setRow(0,i,pole[index][i]);
}

void LedDisplay::setSmer(char type){
    this->smer = type;
}

void LedDisplay::setSensor(unsigned int sonarDistCenterLeft, unsigned int sonarDistCenterRight, unsigned int sonarDistRight, unsigned int sonarDistLeft){
    if (sonarDistCenterLeft > SONARMAX || sonarDistCenterLeft == 0)
        this->sonarDistCenterLeft = SONARMAX;
    else
        this->sonarDistCenterLeft = sonarDistCenterLeft;

    if (sonarDistCenterRight > SONARMAX || sonarDistCenterRight == 0)
        this->sonarDistCenterRight = SONARMAX;
    else
        this->sonarDistCenterRight = sonarDistCenterRight;

    if (sonarDistRight > SONARMAX || sonarDistRight == 0)
        this->sonarDistRight = SONARMAX;
    else
        this->sonarDistRight = sonarDistRight;

    if (sonarDistLeft > SONARMAX || sonarDistLeft == 0)
        this->sonarDistLeft = SONARMAX;
    else
        this->sonarDistLeft = sonarDistLeft;
}

void LedDisplay::vypisSensor(){
    // center left
    // display culonm row true/false
    int i = 0;

    // center left
    i = map(sonarDistCenterLeft,SONARMIN,SONARMAX,0,4);
    for (; i >= 0; i--) {
        lc.setLed(0,7,i,true);
    }

    // center left
    i = map(sonarDistCenterRight,SONARMIN,SONARMAX,0,4);
    for (; i >= 0; i--) {
        lc.setLed(0,0,i,true);
    }
    
    //  left
    i = map(sonarDistLeft,SONARMIN,SONARMAX,0,4);
    for (; i >= 0; i--) {
        lc.setLed(0,i,7,true);
    }
    
}
