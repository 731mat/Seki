//
// Created by mat on 6.11.16.
//

#ifndef SEKI_IBT_H
#define SEKI_IBT_H




class Ibt {
public:
    Ibt(int pinMotorLeftEnable, int pinMotorPWMR, int pinMotorPWML, int pinMotorSense);
    void setData(bool smer, int value);
    void setStop();
    void aplikovat();
    bool getSmer();
    int getValue();

private:
    int pinMotorEnable;
    int pinMotorPWML;
    int pinMotorPWMR;
    int pinMotorSense;
    int pin;
    bool smer;
    int value;
};


#endif //SEKI_IBT_H
