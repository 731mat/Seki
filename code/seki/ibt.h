//
// Created by mat on 6.11.16.
//

#ifndef SEKI_IBT_H
#define SEKI_IBT_H




class Ibt {
public:
    Ibt(int pinMotorLeftEnableR, int pinMotorEnableL, int pinMotorPWM, int pinMotorSense);
    void setData(bool smer, int value);
    void setStop();
    void aplikovat();

private:
    int pinMotorEnableR;
    int pinMotorEnableL;
    int pinMotorPWM;
    int pinMotorSense;
    int pin;
    bool smer;
    int value;
};


#endif //SEKI_IBT_H
