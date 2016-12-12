#include "LedControl.h" //  need the library
#include "ledDisplay.h"

#define SONARMAX 4000
#define SONERMIN 0

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
        for(int i=1;i<9;i++)
            lc.setRow(0,i,seki[j][i]);
        delay(700);
    }
}


void LedDisplay::sipka()
{
  byte pole[7][8]={
              {B00000000,B00010000,B00100000,B00111100,B00111100,B00100000,B00010000,B00000000}, // left
              {B00000000,B00001000,B00000100,B00111100,B00111100,B00000100,B00001000,B00000000}, // right
              {B00000000,B00000000,B00111100,B01011010,B00011000,B00011000,B00000000,B00000000}, // front
              {B00000000,B00000000,B00011000,B00011000,B01011010,B00111100,B00000000,B00000000}, // back
              {B00000000,B00000000,B01111000,B01100000,B01010000,B01001000,B00000100,B00000000}, // front_left
              {B00000000,B00000000,B00011110,B00000110,B00001010,B00010010,B00100000,B00000000}, // front_right
              {B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000}}; // resoult

    // smazaní dat
    for(int i = 0; i< 8; i++)
        this->lc.setRow(0,i,B00000000);

    for(int i = 0; i< 8; i++)
        this->lc.setRow(0,i,pole[this->smer][i]);

    vypisSensor();
}

void LedDisplay::setSmer(char type){
    this->smer = type;
    sipka();
}

void LedDisplay::setSensor(unsigned int sonarDistCenterLeft, unsigned int sonarDistCenterRight, unsigned int sonarDistRight, unsigned int sonarDistLeft){
    this->sonarDistCenterLeft = sonarDistCenterLeft;
    this->sonarDistCenterRight = sonarDistCenterRight;
    this->sonarDistRight = sonarDistRight;
    this->sonarDistLeft = sonarDistLeft;
    sipka();
}

void LedDisplay::vypisSensor(){
      if (millis() <= this->lastUpdate) 
      return;

    // center left
    // display culonm row true/false
    int i = 0;

    // center left
    i = map(sonarDistCenterLeft,SONARMIN,SONARMAX,0,4);
    for (; i >= 0; i--) {
        lc.setLed(0,0,i,true);
    }
    //center right
    i = map(sonarDistCenterRight,SONARMIN,SONARMAX,0,4);
    for (; i >= 0; i--) {
        lc.setLed(0,7,i,true);
    }

    //right
    i = map(sonarDistRight,SONARMIN,SONARMAX,0,4);
    for (; i >= 0; i--) {
        lc.setLed(0,i,0,true);
    }

    //left
    i = map(sonarDistLeft,SONARMIN,SONARMAX,0,4);
    for (; i >= 0; i--) {
        lc.setLed(0,i,7,true);
    }
    
}
