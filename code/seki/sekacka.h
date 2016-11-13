//
// Created by mat on 5.11.16.
//

#ifndef SEKI_Sekacka_H
#define SEKI_Sekacka_H

enum {
    SEN_SONAR_CENTER,      // 0..SONAR_TRIGGER_DISTANCE
    SEN_SONAR_LEFT,        // 0..SONAR_TRIGGER_DISTANCE
    SEN_SONAR_RIGHT,       // 0..SONAR_TRIGGER_DISTANCE
};

enum {
    MOTOR_FRONT,
    MOTOR_FRONT_POMALU,
    MOTOR_FRONT_LEFT,
    MOTOR_FRONT_RIGHT,
    MOTOR_BACK,
    MOTOR_BACK_LEFT,
    MOTOR_BACK_RIGHT,
    MOTOR_LEFT,
    MOTOR_RIGHT,
    MOTOR_STOP,
};

class Sekacka {
public:
    Sekacka();
    virtual void setup(void);
    virtual void loop(void);
    void printInfo();
    void readSensors(); // vyber druhu senzoru
    int readSensor(char type);   // vyber urciteho senzoru
    void motorPohyb(char type);
    void motorUpdate();

    //serial
    void printMenu();
    void menu();
    void readSerial();
    void testMotors();
    void testSonar();
    // bluetooth
    void readBluetooth();
    int usePrintInfo; // print info

    // time
    unsigned long startTime;
    unsigned long nextTimeInfo;
    int updateTimeInfo;
    unsigned long nextTimeMotor;
    int updateTimeMotor;
    int timeUpdateTime;
    int timeRotage;
    int timeRotageMotor;
    // zapnuti pohybu
    bool drive;
    char charBluetooth;

    // --------- sonar ----------------------------------
    // ultra sonic sensor distance-to-obstacle (cm)
    char sonarUse;      // use ultra sonic sensor?
    char sonarLeftUse;
    char sonarRightUse;
    char sonarCenterUse;
    unsigned int sonarDistCenter;
    unsigned int sonarDistRight;
    unsigned int sonarDistLeft;
    unsigned long nextTimeSonar;
    unsigned long nextTimeCheckSonar;
    int distMin;
    int distSlow;
};
#endif //SEKI_Seki_H
