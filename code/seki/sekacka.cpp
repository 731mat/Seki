//
// Created by mat on 5.11.16.
//

#include "sekacka.h"
#include "ibt.h"
#include "drivers.h"
#include <Arduino.h>


// ------ pins---------------------------------------
#define pinMotorLeftEnable 24     //  Forward drive left enable
#define pinMotorLeftPWMR 2         //  Forward left motor PWM pin
#define pinMotorLeftPWML 3         //  backward left motor PWM pin
#define pinMotorLeftSense A0       //  left motor current sense

#define pinMotorRightEnable 28    // Forward drive right enable
#define pinMotorRightPWMR 4        // Forward right motor PWM pin
#define pinMotorRightPWML 5        // backward right motor PWM pin
#define pinMotorRightSense A1      // Right motor current sense

// ----- sonar ----------------------------------------
#define pinSonarCenterTrigger 11   // sonar center Trigger
#define pinSonarCenterEcho 12      // sonar center echo

#define pinSonarRightTrigger 34    // sonar right Trigger
#define pinSonarRightEcho 36       // sonar right Echo

#define pinSonarLeftTrigger 38     // sonar left Trigger
#define pinSonarLeftEcho 40        // sonar left Echo

// ------ orther  -----------------------------------------------
#define pinLed 13                 // led info

#define CONSOLE_BAUDRATE    19200       // baudrate used for console
#define Console Serial
//#define ESP8266port Serial2
//#define Bluetooth Serial1

// ----- inicializace motoru L,R
//Ibt(int pinMotorLeftEnableR, int pinMotorEnableL, int pinMotorPWMR, int pinMotorPWML, int pinMotorSense);
Ibt motorL(pinMotorLeftEnable, pinMotorLeftPWMR, pinMotorLeftPWML, pinMotorLeftSense);
Ibt motorR(pinMotorRightEnable, pinMotorRightPWMR, pinMotorRightPWML, pinMotorRightSense);


Sekacka::Sekacka(){

    usePrintInfo=0;     // co se bude vypisovat z print info

    startTime = 0;     // start in milis()
    nextTimeInfo = 0;     // cislo kdy bude budouci aktualizace
    updateTimeInfo = 500; // update co 0.5 sec.

    nextTimeMotor = 0;       // cislo kdy bude budouci aktualizace
    updateTimeMotor = 100;   // cas k update motor
    timeUpdateTime = 0;     //smycky k rozjizdeni
    timeRotage = 0;         // smycky k otoceni sekiho
    timeRotageMotor = 25;  // kolik smycek "timeRotage" je zapotřebi k otoceni sekiho

    // sonar
    sonarUse       = 1;      // use ultra sonic sensor?
    sonarLeftUse   = 0;      // use LEFT ultra sonic sensor
    sonarRightUse  = 0;      // use RIGHT ultra sonic sensor
    sonarCenterUse = 1;      // use CENTER ultra sonic sensor

    nextTimeSonar  = 0;      // cas dalsi aktualizace ultra sonic sensor
    sonarDistCenter = 0;     // vzdalenost senzor - prekazkou
    sonarDistRight = 0;      // vzdalenost senzor - prekazkou
    sonarDistLeft  = 0;      // vzdalenost senzor - prekazkou
    distMin = 1000;            // vzdalenost senzor - prekazkou kdy musi zastavit
    distSlow = 2000;           // vzdalenost senzor - prekazkou  kdy musi spomalit


    drive = false; // zapnuti pohybu



}


void Sekacka::setup(){
    //set console
    Console.begin(CONSOLE_BAUDRATE);
    Console.println("SETUP");

    // left wheel motor
    pinMode(pinMotorLeftEnable, OUTPUT);   digitalWrite(pinMotorLeftEnable, LOW);
    pinMode(pinMotorLeftPWMR, OUTPUT);
    pinMode(pinMotorLeftPWML, OUTPUT);
    pinMode(pinMotorLeftSense, INPUT);

    // left wheel motor
    pinMode(pinMotorRightEnable, OUTPUT);   digitalWrite(pinMotorRightEnable, LOW);
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
    Console.println("Motors set STOP");

    //set time
    startTime = millis();
    drive = true;
    delay(5000);

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
            Console.print("M1 - V ");
            Console.print(motorL.getValue());
            Console.print(" S ");
            Console.print(motorL.getSmer());
            Console.print("   M2 - V ");
            Console.print(motorR.getValue());
            Console.print("  S  ");
            Console.print(motorR.getSmer());
            Console.println();
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
    Console.println(sonarDistCenter);
    //zpomalení před překážkou
    
    if(sonarDistCenter < distSlow && sonarDistCenter > distMin){
        motorL.setData(true,100);
        motorR.setData(true,100);
    return;
    }

    // otočení
    if(sonarDistCenter <= distMin &&  sonarDistCenter > 0 || timeRotage > 0  && timeRotage < timeRotageMotor){
      if(timeRotage == 0){
        motorL.setStop();
        motorR.setStop();
        delay(1000);
        }
        timeRotage++;
        if(timeRotage < timeRotageMotor){
            motorR.setData(true, 50);
            motorL.setData(false, 50);
            timeUpdateTime = 0;
            return 0;
        }else
        timeRotage = 0;
    }

    // pomalý rozjezd
    if(timeUpdateTime < updateTimeMotor/2){
        motorL.setData(true,map(timeUpdateTime,0,50,0,255));
        motorR.setData(true,map(timeUpdateTime,0,50,0,255));
    }else{
        motorL.setData(true,255);
        motorR.setData(true,255);
    }
}







////////    Console /////////////////////////////////////////


void Sekacka::readSerial() {
    // Console input
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
        if (Console.available() > 0) {
            ch = (char)Console.read();
            switch (ch){
                case '0':
                    return;
                case '1':
                    testMotors();
                    printMenu();
                    break;
                case '2':
                    testSonar();
                    printMenu();
                    break;
                case 'D':
                    Console.println("SET drive TRUE");
                    if(drive == true) drive = false;
                    else drive = true;
                    break;
                case 'I':
                    if(usePrintInfo == 0) usePrintInfo = 1;
                    else usePrintInfo = 0;
                    printMenu();
                    break;


            }
        }
        delay(10);
    }
}


void Sekacka::printMenu(){
    Console.println();
    Console.println(F(" MAIN MENU:"));
    Console.println(F("1=test motors"));
    Console.println(F("2=test sonar"));
    Console.println(F("I=infoStart"));
    Console.println(F("D=start drive"));
    Console.println(F("0=exit"));
    Console.println();
}



void Sekacka::testMotors(){
    for (int i = 0; i < 256; i++) {
        motorR.setData(1,i);
        printInfo();
        delay(100);
        Console.println(i);
    }
    for (int i = 256; i > 0;i--) {
        motorR.setData(1,i);
        printInfo();
        delay(100);
        Console.println(i);
    }
    motorR.setStop();


}

void Sekacka::testSonar(){
  char ch = "";
    while(true) {
         readSensors();
         Console.println(sonarDistCenter);
         if (Console.available() > 0) 
            ch = (char)Console.read();
         if(ch == "m")
            return 0;
    }
}
