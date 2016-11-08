//
// Created by mat on 5.11.16.
//

#include "sekacka.h"
#include "ibt.h"
#include "drivers.h"
#include <Arduino.h>


// ------ pins---------------------------------------
#define pinMotorLeftEnableR 22     //  Forward drive left enable
#define pinMotorLeftEnableL 24     //  backward drive left enable
#define pinMotorLeftPWMR 2         //  Forward left motor PWM pin
#define pinMotorLeftPWML 3         //  backward left motor PWM pin
#define pinMotorLeftSense A0       //  left motor current sense

#define pinMotorRightEnableR 26    // Forward drive right enable
#define pinMotorRightEnableL 28    // backward drive right enable
#define pinMotorRightPWMR 4        // Forward right motor PWM pin
#define pinMotorRightPWML 5        // backward right motor PWM pin
#define pinMotorRightSense A1      // Right motor current sense

// ----- sonar ----------------------------------------
#define pinSonarCenterTrigger 24   // sonar center Trigger
#define pinSonarCenterEcho 22      // sonar center echo

#define pinSonarRightTrigger 30    // sonar right Trigger
#define pinSonarRightEcho 32       // sonar right Echo

#define pinSonarLeftTrigger 34     // sonar left Trigger
#define pinSonarLeftEcho 36        // sonar left Echo

// ------ orther  -----------------------------------------------
#define pinLed 13                 // led info

#define CONSOLE_BAUDRATE    19200       // baudrate used for console

// ----- inicializace motoru L,R
//Ibt(int pinMotorLeftEnableR, int pinMotorEnableL, int pinMotorPWMR, int pinMotorPWML, int pinMotorSense);
Ibt motorL(pinMotorLeftEnableR, pinMotorLeftEnableL, pinMotorLeftPWMR, pinMotorLeftPWML, pinMotorLeftSense);
Ibt motorR(pinMotorRightEnableR, pinMotorRightEnableL, pinMotorRightPWMR, pinMotorRightPWML, pinMotorRightSense);


Sekacka::Sekacka(){

    usePrintInfo=0;     // co se bude vypisovat z print info

    startTime = 0;     // start in milis()
    nextTimeInfo = 0;     // cislo kdy bude budouci aktualizace
    updateTimeInfo = 500; // update co 0.5 sec.

    nextTimeMotor = 0;       // cislo kdy bude budouci aktualizace
    updateTimeMotor = 100;   // cas k update motor
    timeUpdateTime = 0;     //smycky k rozjizdeni
    timeRotage = 0;         // smycky k otoceni sekiho
    timeRotageMotor = 500;  // kolik smycek "timeRotage" je zapotřebi k otoceni sekiho

    // sonar
    sonarUse       = 0;      // use ultra sonic sensor?
    sonarLeftUse   = 0;      // use LEFT ultra sonic sensor
    sonarRightUse  = 0;      // use RIGHT ultra sonic sensor
    sonarCenterUse = 0;      // use CENTER ultra sonic sensor

    nextTimeSonar  = 0;      // cas dalsi aktualizace ultra sonic sensor
    sonarDistCenter = 0;     // vzdalenost senzor - prekazkou
    sonarDistRight = 0;      // vzdalenost senzor - prekazkou
    sonarDistLeft  = 0;      // vzdalenost senzor - prekazkou
    distMin = 20;            // vzdalenost senzor - prekazkou kdy musi zastavit
    distSlow = 80;           // vzdalenost senzor - prekazkou  kdy musi spomalit


    drive = false; // zapnuti pohybu



}


void Sekacka::setup(){
    //set console
    Serial.begin(CONSOLE_BAUDRATE);
    Serial.println("SETUP");

    // left wheel motor
    pinMode(pinMotorLeftEnableR, OUTPUT);   digitalWrite(pinMotorLeftEnableR, LOW);
    pinMode(pinMotorLeftEnableL, OUTPUT);   digitalWrite(pinMotorLeftEnableL, LOW);
    pinMode(pinMotorLeftPWMR, OUTPUT);
    pinMode(pinMotorLeftPWML, OUTPUT);
    pinMode(pinMotorLeftSense, INPUT);

    // left wheel motor
    pinMode(pinMotorRightEnableR, OUTPUT);   digitalWrite(pinMotorRightEnableR, LOW);
    pinMode(pinMotorRightEnableL, OUTPUT);   digitalWrite(pinMotorRightEnableL, LOW);
    pinMode(pinMotorRightPWMR, OUTPUT);
    pinMode(pinMotorRightPWML, OUTPUT);
    pinMode(pinMotorRightSense, INPUT);

    // sonar
    pinMode(pinSonarCenterTrigger, OUTPUT);
    pinMode(pinSonarCenterEcho, INPUT);
    pinMode(pinSonarLeftTrigger, OUTPUT);
    pinMode(pinSonarLeftEcho, INPUT);
    pinMode(pinSonarRightTrigger, OUTPUT);
    pinMode(pinSonarRightEcho, INPUT);

    // led
    pinMode(pinLed, OUTPUT);
    digitalWrite(pinLed, LOW);

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
    readSerial();

    if (millis() >= nextTimeInfo && usePrintInfo !=0) {
        nextTimeInfo = millis() + updateTimeInfo;
        printInfo();
    }

    if (millis() >= nextTimeMotor && drive) {
        nextTimeMotor = millis() + updateTimeMotor;
        motorUpdate();
    }

}

void Sekacka::printInfo(){
    switch (usePrintInfo){
        case 1:
            Serial.print("M1 - s - ");
            Serial.println();
            break;
    }

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


void Sekacka::motorUpdate(){
    timeUpdateTime++;

    //zpomalení před překážkou
    if(sonarDistCenter < distSlow && sonarDistCenter > distMin){
        motorL.setData(true,100);
        motorR.setData(true,100);
    }

    // otočení
    if(sonarDistCenter <= distMin || timeRotage > 0  && timeRotage < timeRotageMotor){
        timeRotage++;
        if(timeRotage < timeRotageMotor){
            motorR.setData(true, 50);
            motorL.setData(false, 50);
            timeUpdateTime = 0;
            return 0;
        }
    }else
        timeRotage = 0;

    // pomalý rozjezd
    if(timeUpdateTime < updateTimeMotor/10){
        motorL.setData(true,map(timeUpdateTime,0,10,0,255));
        motorR.setData(true,map(timeUpdateTime,0,10,0,255));
    }else{
        motorL.setData(true,255);
        motorR.setData(true,255);
    }
}







////////    SERIAL /////////////////////////////////////////


void Sekacka::readSerial() {
    // serial input
    if (Serial.available() > 0) {
        char ch = (char)Serial.read();
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
        if (Serial.available() > 0) {
            ch = (char)Serial.read();
            switch (ch){
                case '0':
                    return;
                case '1':
                    testMotors();
                    printMenu();
                    break;
                case 'D':
                    testMotors();
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
    Serial.println(F("I=infoStart"));
    Serial.println(F("D=start drive"));
    Serial.println(F("0=exit"));
    Serial.println();
}



void Sekacka::testMotors(){
    for (int i = 0; i < 256; i++) {
        motorR.setData(1,i);
        printInfo();
        delay(200);
    }
    for (int i = 256; i > 0;i--) {
        motorR.setData(0,i);
        printInfo();
        delay(200);
    }
    motorR.setStop();

    for (int i = 0; i < 256; i++) {
        motorL.setData(1,i);
        printInfo();
        delay(200);

    }
    for (int i = 256; i > 0;i--) {
        motorL.setData(0,i);
        printInfo();
        delay(200);
    }
    motorL.setStop();
}

