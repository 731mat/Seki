 //
// Created by mat on 5.11.16.
//

#include "sekacka.h"
#include "ibt.h"
#include "drivers.h"
#include "bluetooth.h"
#include "ledDisplay.h"
#include <Arduino.h>


// ------ pins---------------------------------------
#define pinMotorLeftEnable 24     //  Forward drive left enable
#define pinMotorLeftPWMR 4         //  Forward left motor PWM pin
#define pinMotorLeftPWML 5         //  backward left motor PWM pin
#define pinMotorLeftSense A0       //  left motor current sense

#define pinMotorRightEnable 28    // Forward drive right enable
#define pinMotorRightPWMR 6        // Forward right motor PWM pin
#define pinMotorRightPWML 7        // backward right motor PWM pin
#define pinMotorRightSense A1      // Right motor current sense

// ----- sonar ----------------------------------------
#define pinSonarCenterLeftTrigger 50   // sonar center Trigger
#define pinSonarCenterLeftEcho 52      // sonar center echo

#define pinSonarCenterRightTrigger 46   // sonar center Trigger
#define pinSonarCenterRightEcho 48      // sonar center echo

#define pinSonarLeftTrigger 42     // sonar left Trigger
#define pinSonarLeftEcho 44        // sonar left Echo

#define pinSonarRightTrigger 39    // sonar left Trigger
#define pinSonarRightEcho 37       // sonar left Echo


//  --- mower ---------------------------------------
#define pinMowerPWM 45

// ---- LEDdisplay ------------------------
#define pinLedDisplayCLK 33
#define pinLedDisplayCS 35
#define pinLedDisplayDIN 31

// ---- bumper --------------------------------------
#define pinBumperFront 2
#define pinBumperBack 3
#define pinBumperLeft A12
#define pinBumperRight A13


// ------ orther  -----------------------------------------------
#define pinLed 13                 // led info
#define pinButtonDrive 32
#define pinBuzzer 36

// ----- Serial line speed --------------------------------
#define CONSOLE_BAUDRATE    115200       // baudrate used for console
#define ESP8266_BAUDRATE    115200       // baudrate used for console

#define Console Serial
#define ESP8266port Serial2
//#define Bluetooth Serial1

Ibt motorL(pinMotorLeftEnable, pinMotorLeftPWMR, pinMotorLeftPWML, pinMotorLeftSense);
Ibt motorR(pinMotorRightEnable, pinMotorRightPWMR, pinMotorRightPWML, pinMotorRightSense);
Bluetooth bt;
LedDisplay ld(pinLedDisplayDIN, pinLedDisplayCS, pinLedDisplayCLK, 1);


Sekacka::Sekacka(){

    usePrintInfo=1;     // co se bude vypisovat z print info

    stateStartTime = 0;     // start in milis()
    nextTimeInfo = 0;     // cislo kdy bude budouci aktualizace
    updateTimeInfo = 500; // update co 0.5 sec.

    nextTimeMotor = 0;       // cislo kdy bude budouci aktualizace
    updateTimeMotor = 100;   // cas k update motor
    timeUpdateTime = 0;     //smycky k rozjizdeni
    timeRotage = 0;         // smycky k otoceni sekiho
    timeRotageMotor = 25;  // kolik smycek "timeRotage" je zapotřebi k otoceni sekiho

    // button
    buttonUse = 1;
    
    // bumper
    bumperUse       = 0;      // use bumper?
    bumperCenterUse   = 0;      // use CENTER
    bumperLeftUse   = 0;      // use LEFT
    bumperRightUse  = 0;      // use RIGHT

    // sonar
    sonarUse       = 1;      // use ultra sonic sensor?
    sonarLeftUse   = 1;      // use LEFT ultra sonic sensor
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

    timeButtonDelay = 0;
    previousButtonDrive = LOW;

    drive = false; // zapnuti pohybu
    charBluetooth = 'X';
}


void Sekacka::setup(){
    //set console
    Console.begin(CONSOLE_BAUDRATE);
    ESP8266port.begin(ESP8266_BAUDRATE);
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

    // button
    pinMode(pinButtonDrive, INPUT);
    
    // buzzer
    pinMode(pinBuzzer, OUTPUT);
    digitalWrite(pinBuzzer, LOW);
    
    // mower
    pinMode(pinMowerPWM, OUTPUT);


    // bumper
    pinMode(pinBumperFront, INPUT_PULLUP);
    pinMode(pinBumperBack, INPUT_PULLUP);
    pinMode(pinBumperLeft, INPUT_PULLUP);
    pinMode(pinBumperRight, INPUT_PULLUP);



    attachInterrupt(digitalPinToInterrupt(pinBumperBack), naraz, HIGH);
    //attachInterrupt(digitalPinToInterrupt(pinBumperFront), naraz, HIGH);
    //attachInterrupt(pinBumperLeft, naraz, HIGH);
    //attachInterrupt(pinBumperRight, naraz, HIGH);

    //ledDisplay
    ld.init();

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
    stateStartTime = millis();
    drive = false;
    loopCounter = 0;
    //delay(5000);

}

void static Sekacka::naraz(){
 //   Serial.println("naraz");
   //1 drive = !drive;
}

void Sekacka::loop(){
    loopCounter++;
    readSensors();
    readSerial();
    bt.readBT();
    drive = bt.getValueDrive();
    
    if (millis() >= nextTimeInfo && usePrintInfo !=0) {
        nextTimeInfo = millis() + updateTimeInfo;
        printInfo();
        ld.printInfo();
        loopCounter = 0;
    }
    
    if (millis() >= nextOffBuzzer)
      buzzerDriver(pinBuzzer, 0);

    if (millis() >= nextTimeMotor && drive) {
        nextTimeMotor = millis() + updateTimeMotor;
        motorUpdate();
    }else
      if(millis() >= nextTimeMotor && !drive){
        nextTimeMotor = millis() + updateTimeMotor;
        motorMower(pinMowerPWM, 0);
        driveBluetooth();
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
            Console.print("  CENTER L:");
            Console.print(sonarDistCenterLeft);
            Console.print("  CENTER R:");
            Console.print(sonarDistCenterRight);
            Console.print("  left:");
            Console.print(sonarDistLeft);
            Console.print("  drive:");
            Console.print(drive);
            Console.print("  btUhel:");
            Console.print(bt.getUhel());
            Console.print("  btRychlost:");
            Console.print(bt.getRychlost());
            Console.println();
            break;
    }
}

void Sekacka::printJsonData(){
    String s = "{";
    s+= "\"m1v\":" + String(motorL.getValue()) + ",\"m1s\":" + String(motorL.getSmer()) + ",";
    s+= "\"m2v\":" + String(motorR.getValue()) + ",\"m2s\":" + String(motorR.getSmer()) + ",";
    s+= "\"sonCENTERL\":" + String(sonarDistCenterLeft) + ",\"sonCENTERR\":" + String(sonarDistCenterRight) + ",";
    s+= "\"sonRIGHT\":" + String(sonarDistRight) + ",\"sonLEFT\":" + String(sonarDistLeft) + ",";
    s+= "\"countLoop\":" + String(loopCounter) + ",";
    s+= "\"startTime\":" + String(millis() - stateStartTime);
    s+= "}";
    ESP8266port.print(s);
    bt.writeBT(s);
    //Console.println(s);
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
        ld.setSensor(sonarDistCenterLeft,sonarDistCenterRight, sonarDistRight, sonarDistLeft);
        addToHistorySensor(sonarDistCenterLeft,sonarDistCenterRight, sonarDistRight, sonarDistLeft);
    }
    if(buttonUse){
        int cteni = 0;
        cteni = digitalRead(pinButtonDrive);

        if (cteni == HIGH && previousButtonDrive == LOW && millis() - timeButtonDelay > 1000) {
            setDrive();
            timeButtonDelay = millis();
        }
        previousButtonDrive = cteni;
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
            if((sonarDistCenterLeft > distSlow || sonarDistCenterLeft == 0) && (sonarDistCenterRight > distSlow  || sonarDistCenterRight == 0))
                return 1;
            break;
        case FRONT_SLOW:
            if(sonarDistCenterLeft < distSlow && sonarDistCenterLeft > distMin ||
               sonarDistCenterRight < distSlow && sonarDistCenterRight > distMin)
                return 1;
            break;
        case LEFT:
            if(sonarDistLeft > distMin || sonarDistLeft == 0)
                return 1;
            break;
        case STOP:
            if((sonarDistCenterLeft < distMin && sonarDistCenterLeft != 0) || (sonarDistCenterRight < distMin && sonarDistCenterRight != 0))
              return 1;
            break;
    }
    return 0;
}

void Sekacka::driveBluetooth(){
    int maxRychlost = map(bt.getRychlost(),0,100,0,255);

    if(bt.getUhel() >=0 && bt.getUhel() <=45){
        motorR.setData(true,map(bt.getUhel(),45,0,0,maxRychlost));
        motorL.setData(true,maxRychlost);
    }
    if(bt.getUhel() >=45 && bt.getUhel() <=90){
        motorR.setData(false,map(bt.getUhel(),45,90,0,maxRychlost));
        motorL.setData(false,maxRychlost);
    }
    if(bt.getUhel() >=90 && bt.getUhel() <=135){
        motorR.setData(false,maxRychlost);
        motorL.setData(false,map(bt.getUhel(),135,90,0,maxRychlost));
    }
    if(bt.getUhel() >=135 && bt.getUhel() <=180){
        motorR.setData(true,maxRychlost);
        motorL.setData(true,map(bt.getUhel(),135,180,0,maxRychlost));
    }

    if(bt.getUhel() >= 0 && bt.getUhel() <= 11)
        ld.setSmer(MOTOR_FRONT);
    if(bt.getUhel() >= 11 && bt.getUhel() <= 33)
        ld.setSmer(MOTOR_FRONT_RIGHT);
    //righ    
    if(bt.getUhel() >=33 && bt.getUhel() <= 56)
        ld.setSmer(MOTOR_RIGHT);

    if(bt.getUhel() >= 56 && bt.getUhel() <= 78)
        ld.setSmer(MOTOR_BACK_RIGHT);
    //   dole
    if(bt.getUhel() >= 78 && bt.getUhel() <= 101)
        ld.setSmer(MOTOR_BACK);

    if(bt.getUhel() >=101 && bt.getUhel() <=123)
        ld.setSmer(MOTOR_BACK_LEFT);

    // left
    if(bt.getUhel() >=123 && bt.getUhel() <=146)
        ld.setSmer(MOTOR_LEFT);

    if(bt.getUhel() >=146 && bt.getUhel() <=168)
        ld.setSmer(MOTOR_FRONT_LEFT);

    if(bt.getUhel() >=168 && bt.getUhel() <=0)
        ld.setSmer(MOTOR_FRONT);

   if(maxRychlost == 0)
      ld.setSmer(MOTOR_STOP);
}

void Sekacka::motorUpdate(){

    motorMower(pinMowerPWM, 255);
    timeUpdateTime++;

    // couvání
    if(loopAutoBack > 10 && loopAutoBack < 50){
        motorPohyb(MOTOR_BACK,50);
        loopAutoBack++;
        timeUpdateTime = 0;
        return;
    }

    //zpomalení před překážkou
    // funguje jen jednou - smazat
    if(sonarMuzu(FRONT_SLOW) && timeRotage == 0){
        motorPohyb(MOTOR_FRONT, 50);
        return;
    }

    // otočení
    if(sonarMuzu(STOP) && sonarMuzu(LEFT)|| (timeRotage > 0  && timeRotage < timeRotageMotor && sonarMuzu(LEFT))){
      if(timeRotage == 0){
        motorPohyb(MOTOR_STOP,0);
        delay(500);
      }
        // na nic - smazat
        if(sonarMuzu(LEFT) == 0){
          //motorPohyb(MOTOR_STOP,0);
          //return;
        }
        timeRotage++;
        motorPohyb(MOTOR_LEFT,0);
        timeUpdateTime = 0;
        return 0;
    }else
        timeRotage = 0;

    // když nemuže dopředu a ani vlevo
    if(sonarMuzu(STOP) && !sonarMuzu(LEFT)){
        loopAutoBack++;
        motorPohyb(MOTOR_STOP,0);
        buzzer(1500);
        return;
    }
    // pomalý rozjezd
    if(timeUpdateTime < updateTimeMotor/2){
        motorPohyb(MOTOR_FRONT, map(timeUpdateTime,0,50,0,100));
        loopAutoBack = 0; // resetování couvání
    }else{
        motorPohyb(MOTOR_FRONT, 100);
    }
}



// char type - enum
// int value - od 0 do 100

void Sekacka::motorPohyb(char type, int value = 0){
  value = map(value, 0,100,0,255);
    ld.setSmer(type);
    switch (type) {
        case MOTOR_FRONT:
            motorL.setData(true,value);
            motorR.setData(true,value);
            break;
        case MOTOR_FRONT_LEFT:
            motorL.setData(true, 128);
            motorR.setData(true, 255);
            break;
        case MOTOR_FRONT_RIGHT:
            motorL.setData(true, 255);
            motorR.setData(true, 128);
            break;
        case MOTOR_BACK:
            Console.println(value);
            motorR.setData(false, value);
            motorL.setData(false, value);
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
            case 'D':
                setDrive();
                Console.print("SET drive");
                Console.println(drive);
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
                case '3':
                    testButton();
                    printMenu();
                    break;
                case 'D':
                    setDrive();
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
    Console.println(F("1= test motors"));
    Console.println(F("2= test sonar (exit 'm')"));
    Console.println(F("3= test button (exit 'm')"));
    Console.println(F("I= infoStart"));
    Console.println(F("D= start drive"));
    Console.println(F("R= RESET"));
    Console.println(F("0= exit (hl.smycka)"));
    Console.println();
}



void Sekacka::testMotors(){
    for (int i = 0; i < 256; i++) {
        motorR.setData(1,i);
        printInfo();
        delay(25);
    }
    for (int i = 256; i > 0;i--) {
        motorR.setData(1,i);
        printInfo();
        delay(25);
    }
    motorR.setStop();

    // test mower
    for (int i = 0; i < 256; i++) {
        motorMower(pinMowerPWM,i);
        delay(25);
        Console.println(i);
    }
    motorMower(pinMowerPWM, 0);
}

void Sekacka::testSonar(){
    char ch = "";
    while(true) {
         readSensors();
         Console.print("  CENTER L:");
         Console.print(sonarDistCenterLeft);
         Console.print("  CENTER R:");
         Console.print(sonarDistCenterRight);
         Console.print("  left:");
         Console.print(sonarDistLeft);
         Console.println();
         ch = (char)Console.read();
         if(ch == 'm')
            return 0;
    }
}
void Sekacka::testButton(){
    char ch = "";
    int i = 0;
    while(true) {
         i = digitalRead(pinButtonDrive);
         Console.println(i);
         ch = (char)Console.read();
         if(ch == 'm')
            return 0;
    }
}

void Sekacka::buzzer(int sec){
  buzzerDriver(pinBuzzer, 1);
  nextOffBuzzer = sec + millis();
  }
void Sekacka::setDrive(){
    drive = !drive;
    bt.setValueDrive(drive);
}

 void Sekacka::addToHistorySensor(int dSonarDistCenterLeft, int dSonarDistCenterRight, int dSonarDistRight, int dSonarDistLeft){
     for (int i = 100; i > 0; i--) {
         historySensor[i] = historySensor[i-1];
     }
     historySensor[0].sonarDistCenterLeft = dSonarDistCenterLeft;
     historySensor[0].sonarDistCenterRight = dSonarDistCenterRight;
     historySensor[0].sonarDistRight = dSonarDistRight;
     historySensor[0].sonarDistLeft = dSonarDistLeft;
 }