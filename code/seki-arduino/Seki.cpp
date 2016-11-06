//
// Created by mat on 5.11.16.
//

#include "Seki.h"
#include <Arduino.h>

// ------ pins---------------------------------------
#define pinMotorLeftEnableR 22      // Forward drive enable input
#define pinMotorLeftEnableL 24      // Forward drive enable input
#define pinMotorLeftPWM 5           // M1_IN1 left motor PWM pin
#define pinMotorLeftSense A1       // M1_FB  left motor current sense

#define pinMotorRightEnableR 22      // Forward drive enable input
#define pinMotorRightEnableL 24      // Forward drive enable input
#define pinMotorRightPWM 5           // M1_IN1 left motor PWM pin
#define pinMotorRightSense A1       // M1_FB  left motor current sense

#define CONSOLE_BAUDRATE    19200       // baudrate used for console


Seki robot;


Seki::Seki(){
    name = "SEKI";
}

void Seki::setup(){
    Wire.begin();
    Console.begin(CONSOLE_BAUDRATE);
    //while (!Console) ; // required if using Due native port
    Console.println("SETUP");

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
}

void Seki::loop(){
}

