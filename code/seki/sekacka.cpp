//
// Created by mat on 5.11.16.
//

#include "sekacka.h"
#include "ibt.h"
#include "drivers.h"
#include <Arduino.h>

// ------ pins---------------------------------------
#define pinMotorLeftEnableR 22      // Forward drive enable input
#define pinMotorLeftEnableL 24      // Forward drive enable input
#define pinMotorLeftPWM 2           // M1_IN1 left motor PWM pin
#define pinMotorLeftSense A0       // M1_FB  left motor current sense

#define pinMotorRightEnableR 26      // Forward drive enable input
#define pinMotorRightEnableL 28      // Forward drive enable input
#define pinMotorRightPWM 3           // M1_IN1 left motor PWM pin
#define pinMotorRightSense A1       // M1_FB  left motor current sense

// ----- sonar ----------------------------------------
#define pinSonarCenterTrigger 24
#define pinSonarCenterEcho 22

#define pinSonarRightTrigger 30
#define pinSonarRightEcho 32

#define pinSonarLeftTrigger 34
#define pinSonarLeftEcho 36

// -----------------------------------------------------


#define CONSOLE_BAUDRATE    19200       // baudrate used for console


//Ibt(int pinMotorLeftEnableR, int pinMotorEnableL, int pinMotorPWM, int pinMotorSense);
Ibt motorL(pinMotorLeftEnableR, pinMotorLeftEnableL, pinMotorLeftPWM, pinMotorLeftSense);
Ibt motorR(pinMotorRightEnableR, pinMotorRightEnableL, pinMotorRightPWM, pinMotorRightSense);

Sekacka::Sekacka(){
    startTime = 0;     // start in milis()
    nextTimeInfo = 0;     // cislo kdy bude budouci aktualizace
    updateTimeInfo = 500; // update co 0.5 sec.

    // sonar
    sonarUse       = 0;      // use ultra sonic sensor?
    sonarLeftUse   = 0;
    sonarRightUse  = 0;
    sonarCenterUse = 0;

    nextTimeSonar  = 0;
    sonarDistCenter = 0;
    sonarDistRight = 0;
    sonarDistLeft  = 0;

}


void Sekacka::setup(){
    //set console
    Serial.begin(CONSOLE_BAUDRATE);
    Serial.println("SETUP");

    // left wheel motor
    pinMode(pinMotorLeftEnableR, OUTPUT);   digitalWrite(pinMotorLeftEnableR, LOW);
    pinMode(pinMotorLeftEnableL, OUTPUT);   digitalWrite(pinMotorLeftEnableL, LOW);
    pinMode(pinMotorLeftPWM, OUTPUT);
    pinMode(pinMotorLeftSense, INPUT);

    // left wheel motor
    pinMode(pinMotorRightEnableR, OUTPUT);   digitalWrite(pinMotorRightEnableR, LOW);
    pinMode(pinMotorRightEnableL, OUTPUT);   digitalWrite(pinMotorRightEnableL, LOW);
    pinMode(pinMotorRightPWM, OUTPUT);
    pinMode(pinMotorRightSense, INPUT);

    // sonar
    pinMode(pinSonarCenterTrigger, OUTPUT);
    pinMode(pinSonarCenterEcho, INPUT);
    pinMode(pinSonarLeftTrigger, OUTPUT);
    pinMode(pinSonarLeftEcho, INPUT);
    pinMode(pinSonarRightTrigger, OUTPUT);
    pinMode(pinSonarRightEcho, INPUT);

    // motor Stop
    motorL.setStop();
    motorR.setStop();
    Serial.println("Motors set STOP");

    //set time
    startTime = millis();

}

void Sekacka::loop(){
    //stateTime = millis() - stateStartTime;

    readSensors();

    if (millis() >= nextTimeInfo) {
        nextTimeInfo = millis() + updateTimeInfo;
        printInfo();
    }
}

void Sekacka::printInfo(){

}


void Sekacka::readSensors() {
    if ((sonarUse) && (millis() >= nextTimeSonar)){
        static char senSonarTurn = SEN_SONAR_RIGHT;
        nextTimeSonar = millis() + 80;

        switch(senSonarTurn) {
            case SEN_SONAR_RIGHT:
                if (sonarRightUse) sonarDistRight = readSensor(SEN_SONAR_RIGHT);
                senSonarTurn = SEN_SONAR_LEFT;
                break;
            case SEN_SONAR_LEFT:
                if (sonarLeftUse) sonarDistLeft = readSensor(SEN_SONAR_LEFT);
                senSonarTurn = SEN_SONAR_CENTER;
                break;
            case SEN_SONAR_CENTER:
                if (sonarCenterUse) sonarDistCenter = readSensor(SEN_SONAR_CENTER);
                senSonarTurn = SEN_SONAR_RIGHT;
                break;
            default:
                senSonarTurn = SEN_SONAR_RIGHT;
                break;
        }
    }
}

int Sekacka::readSensor(char type){
    switch (type) {
// sonar---------------------------------------------------------------------------------------------------
        case SEN_SONAR_CENTER: return(readHCSR04(pinSonarCenterTrigger, pinSonarCenterEcho)); break;
        case SEN_SONAR_LEFT: return(readHCSR04(pinSonarLeftTrigger, pinSonarLeftEcho)); break;
        case SEN_SONAR_RIGHT: return(readHCSR04(pinSonarRightTrigger, pinSonarRightEcho)); break;
    }
    return 0;
}



void Sekacka::readSerial() {
    // serial input
    if (Console.available() > 0) {
        char ch = (char)Console.read();
        switch (ch){
            case 'm':
                menu();
                break;
            case 'v':
                break;

        }
    }
}


void Sekacka::menu(){
    char ch;
    printMenu();
    while(true){
        imu.update();
        if (Console.available() > 0) {
            ch = (char)Console.read();
            switch (ch){
                case '0':
                    return;
                case '1':
                    //testMotors();
                    printMenu();
                    break;

            }
        }
        delay(10);
    }
}


void Sekacka::printMenu(){
    Serial.println();
    Serial.println(F(" MAIN MENU:"));
    Serial.println(F("1=test motors"));
    Serial.println(F("2=test odometry"));
    Serial.println(F("0=exit"));
    Serial.println();
}

