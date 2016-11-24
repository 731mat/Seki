//
// Created by mat on 5.11.16.
//

#include "sekacka.h"
#include "ibt.h"
#include "drivers.h"
#include "bluetooth.h"
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
#define pinSonarCenterLeftTrigger 11   // sonar center Trigger
#define pinSonarCenterLeftEcho 12      // sonar center echo

#define pinSonarCenterRightTrigger 13   // sonar center Trigger
#define pinSonarCenterRightEcho 8      // sonar center echo

#define pinSonarLeftTrigger 38     // sonar left Trigger
#define pinSonarLeftEcho 40        // sonar left Echo

#define pinSonarRightTrigger 39    // sonar left Trigger
#define pinSonarRightEcho 37       // sonar left Echo

// ---- bumper --------------------------------------
#define pinBumperFront A10
#define pinBumperBack A11
#define pinBumperLeft A12
#define pinBumperRight A13


// ------ orther  -----------------------------------------------
#define pinLed 13                 // led info

#define CONSOLE_BAUDRATE    19200       // baudrate used for console
#define Console Serial
#define ESP8266port Serial2
//#define Bluetooth Serial1

Ibt motorL(pinMotorLeftEnable, pinMotorLeftPWMR, pinMotorLeftPWML, pinMotorLeftSense);
Ibt motorR(pinMotorRightEnable, pinMotorRightPWMR, pinMotorRightPWML, pinMotorRightSense);
Bluetooth bt;


Sekacka::Sekacka(){

    usePrintInfo=1;     // co se bude vypisovat z print info

    startTime = 0;     // start in milis()
    nextTimeInfo = 0;     // cislo kdy bude budouci aktualizace
    updateTimeInfo = 500; // update co 0.5 sec.

    nextTimeMotor = 0;       // cislo kdy bude budouci aktualizace
    updateTimeMotor = 100;   // cas k update motor
    timeUpdateTime = 0;     //smycky k rozjizdeni
    timeRotage = 0;         // smycky k otoceni sekiho
    timeRotageMotor = 25;  // kolik smycek "timeRotage" je zapotřebi k otoceni sekiho

    // bumper
    bumperUse       = 0;      // use bumper?
    bumperCenterUse   = 0;      // use CENTER
    bumperLeftUse   = 0;      // use LEFT
    bumperRightUse  = 0;      // use RIGHT

    // sonar
    sonarUse       = 1;      // use ultra sonic sensor?
    sonarLeftUse   = 0;      // use LEFT ultra sonic sensor
    sonarRightUse  = 0;      // use RIGHT ultra sonic sensor
    sonarCenterLeftUse = 1;      // use CENTER ultra sonic sensor
    sonarCenterRightUse = 1;      // use CENTER ultra sonic sensor

    nextTimeSonar  = 0;      // cas dalsi aktualizace ultra sonic sensor
    sonarDistCenterLeft = 0;     // vzdalenost senzor - prekazkou
    sonarDistCenterRight = 0;     // vzdalenost senzor - prekazkou
    sonarDistRight = 0;      // vzdalenost senzor - prekazkou
    sonarDistLeft  = 0;      // vzdalenost senzor - prekazkou
    distMin = 1000;            // vzdalenost senzor - prekazkou kdy musi zastavit
    distSlow = 2000;           // vzdalenost senzor - prekazkou  kdy musi spomalit


    drive = false; // zapnuti pohybu
    charBluetooth = 'S';

    bumperNaraz = false;


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
    pinMode(pinSonarCenterLeftTrigger, OUTPUT);
    pinMode(pinSonarCenterLeftEcho, INPUT);
    pinMode(pinSonarCenterRightTrigger, OUTPUT);
    pinMode(pinSonarCenterRightEcho, INPUT);
    pinMode(pinSonarLeftTrigger, OUTPUT);
    pinMode(pinSonarLeftEcho, INPUT);
    pinMode(pinSonarRightTrigger, OUTPUT);
    pinMode(pinSonarRightEcho, INPUT);

    // bumper
    pinMode(pinBumperFront, INPUT_PULLUP);
    pinMode(pinBumperBack, INPUT_PULLUP);
    pinMode(pinBumperLeft, INPUT_PULLUP);
    pinMode(pinBumperRight, INPUT_PULLUP);



    attachInterrupt(pinBumperBack, naraz, CHANGE);
    attachInterrupt(pinBumperFront, naraz, CHANGE);
    attachInterrupt(pinBumperLeft, naraz, CHANGE);
    attachInterrupt(pinBumperRight, naraz, CHANGE);



    // led
    pinMode(pinLed, OUTPUT);
    digitalWrite(pinLed, LOW);

    // motor Stop
    motorL.setStop();
    motorR.setStop();
    Console.println("Motors set STOP");
    bt.writeBT("START");
    bt.setName("ads");

    //set time
    startTime = millis();
    drive = true;
    //delay(5000);

}

void Sekacka::naraz(){
    motorPohyb(MOTOR_STOP,0);
    bumperNaraz = !bumperNaraz;
}

void Sekacka::loop(){
    //stateTime = millis() - stateStartTime;;
    readSensors();
    readSerial();
    readBluetooth();
    if (millis() >= nextTimeInfo && usePrintInfo !=0) {
        nextTimeInfo = millis() + updateTimeInfo;
        printInfo();
    }

    if (millis() >= nextTimeMotor && drive) {
        nextTimeMotor = millis() + updateTimeMotor;
        motorUpdate();
    }else
      if(!drive){
        motorR.setStop();
        motorL.setStop();
      }
      
}

void Sekacka::printInfo(){
    printJsonData();
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
            Console.print("  CENTER:");
            Console.print(sonarDistCenterLeft);
            Console.println();
            break;
    }
}

void Sekacka::printJsonData(){
    String s = "{";
    s+= "m1v:" + motorL.getValue() + ",m1s:" + motorL.getSmer() + ",";
    s+= "m2v:" + motorR.getValue() + ",m2s:" + motorR.getSmer() + ",";
    s+= "sonCENTERL:" + sonarDistCenterLeft + ",sonCENTERR:" + sonarDistCenterRight + ",";
    s+= "sonRIGHT:" + sonarDistRight + ",sonLEFT:" + sonarDistLeft + ",";
    s+= "}";
    ESP8266port.print(s);
    Console.println(s);
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
                senSonarTurn = SEN_SONAR_CENTER_LEFT;
                break;
            case SEN_SONAR_CENTER_LEFT:
                if (sonarCenterLeftUse) sonarDistCenterLeft = readSensor(SEN_SONAR_CENTER_LEFT);
                senSonarTurn = SEN_SONAR_CENTER_RIGHT;
                break;
            case SEN_SONAR_CENTER_RIGHT:
                if (sonarCenterRightUse) sonarDistCenterRight = readSensor(SEN_SONAR_CENTER_RIGHT);
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
        case SEN_SONAR_CENTER_LEFT: return(readHCSR04(pinSonarCenterLeftTrigger, pinSonarCenterLeftEcho)); break;
        case SEN_SONAR_CENTER_RIGHT: return(readHCSR04(pinSonarCenterRightTrigger, pinSonarCenterRightEcho)); break;
        case SEN_SONAR_LEFT: return(readHCSR04(pinSonarLeftTrigger, pinSonarLeftEcho)); break;
        case SEN_SONAR_RIGHT: return(readHCSR04(pinSonarRightTrigger, pinSonarRightEcho)); break;
    }
    return 0;
}

bool Sekacka::sonarMuzu(char type){
    switch (type) {
        case FRONT:
            if(sonarDistCenterLeft > distSlow && sonarDistCenterRight > distSlow || sonarDistCenterLeft == 0 && sonarDistCenterRight == 0)
                return 1;
            break;
        case FRONT_SLOW:
            if(sonarDistCenterLeft < distSlow && sonarDistCenterLeft > distMin &&
               sonarDistCenterRight < distSlow && sonarDistCenterRight > distMin)
                return 1;
            break;
        case LEFT:
            if(sonarDistLeft < distSlow && sonarDistLeft > distMin)
                return 1;
            break;
    }
    return 0;
}


void Sekacka::motorUpdate(){
    if(bt.isCon()){
      switch(charBluetooth){
          case 'F':
              motorPohyb(MOTOR_FRONT, 100);
              return 0;
          case 'B':
              motorPohyb(MOTOR_BACK, 0);
              return 0;
          case 'L':
              motorPohyb(MOTOR_LEFT, 0);
              return 0;
          case 'R':
              motorPohyb(MOTOR_RIGHT, 0);
              return 0;
          case 'S':
              motorPohyb(MOTOR_STOP, 0);
              return 0;
          case 'G':  // frontLEFT
              motorPohyb(MOTOR_FRONT_LEFT, 0);
              return 0;
          case 'I':   // frontRIGHT
              motorPohyb(MOTOR_FRONT_RIGHT, 0);
              return 0;
          case 'H':  // backLEFT
              motorPohyb(MOTOR_BACK_LEFT, 0);
              return 0;
          case 'J':  // backRIGHT
              motorPohyb(MOTOR_BACK_RIGHT, 0);
              return 0;
      }
      timeUpdateTime = 0;
      return 0;
    }

    timeUpdateTime++;
    
    //zpomalení před překážkou
    
    if(sonarMuzu(FRONT_SLOW) && timeRotage == 0){
        motorPohyb(MOTOR_FRONT, 50);
    return;
    }

    // otočení
    if(sonarMuzu(STOP) || timeRotage > 0  && timeRotage < timeRotageMotor){
      if(timeRotage == 0){
        motorPohyb(MOTOR_STOP,0);
        delay(500);
      }
        timeRotage++;
        motorPohyb(MOTOR_LEFT,0);
        timeUpdateTime = 0;
        return 0;
    }else
        timeRotage = 0;

    // pomalý rozjezd
        if(timeUpdateTime < updateTimeMotor/2){
            motorPohyb(MOTOR_FRONT, map(timeUpdateTime,0,50,0,100));
        }else{
            motorPohyb(MOTOR_FRONT, 100);
        }
}



// char type - enum
// int value - od 0 do 100

void Sekacka::motorPohyb(char type, int value = 0){
    switch (type) {
        case MOTOR_FRONT:
            motorL.setData(true,map(value, 0,100,0,255));
            motorR.setData(true,map(value, 0,100,0,255));
        case MOTOR_FRONT_LEFT:
            motorL.setData(true, 128);
            motorR.setData(true, 255);
            break;
        case MOTOR_FRONT_RIGHT:
            motorL.setData(true, 255);
            motorR.setData(true, 128);
            break;
        case MOTOR_BACK:
            motorR.setData(false, 255);
            motorL.setData(false, 255);
            break;
        case MOTOR_BACK_LEFT:
            motorL.setData(false, 128);
            motorR.setData(false, 255);
            break;
        case MOTOR_BACK_RIGHT:
            motorL.setData(false, 255);
            motorR.setData(false, 128);
            break;

        case MOTOR_LEFT:
            motorR.setData(true, 50);
            motorL.setData(false, 50);
            break;
        case MOTOR_RIGHT:
            motorR.setData(false, 50);
            motorL.setData(true, 50);
            break;
        case MOTOR_STOP:
            motorL.setStop();
            motorR.setStop();
    }
    return 0;
}



////////    Blouetooth //////////////////////////////////////

void Sekacka::readBluetooth(){
    char charBluetoothPom;
    charBluetoothPom = bt.readBT();
    if(charBluetoothPom == 'M')
      return 0;
    charBluetooth = charBluetoothPom;
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
                    if(drive == true) drive = false;
                    else drive = true;
                    Console.print("SET drive");
                    Console.println(drive);
                    printMenu();
                    break;
                case 'I':
                    if(usePrintInfo == 0) usePrintInfo = 1;
                    else usePrintInfo = 0;
                    printMenu();
                    break;
                case 'R':
                  asm volatile ("  jmp 0");
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
    Console.println(F("R= RESET"));
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
         Console.println(sonarDistCenterLeft);
         if (Console.available() > 0) 
            ch = (char)Console.read();
         if(ch == "m")
            return 0;
    }
}
