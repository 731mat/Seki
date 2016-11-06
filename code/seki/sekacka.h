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

class Sekacka {
public:
    Sekacka();
    virtual void setup(void);
    virtual void loop(void);
    void printInfo();
    void readSensors(); // vyber druhu senzoru
    int readSensor(char type);   // vyber urciteho senzoru

    //serial
    void printMenu();
    void menu();
    void readSerial();

    // time
    unsigned long startTime;
    unsigned long nextTimeInfo;
    int updateTimeInfo;

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
};


#endif //SEKI_Seki_H
