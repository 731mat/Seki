#include "robot.h"

Robot::Robot(){
  name = "Seki";
}


void Robot::printSettingSerial(){
 
  // ------- wheel motors ---------------------------------------------------------
  Console.println(F("---------- wheel motors --------------------------------------"));
  Console.print  (F("motorAccel                                 : "));
}

void Robot::setup()  {     
  setMotorPWM(0, 0, false);
  loadSaveErrorCounters(true);
  loadUserSettings();
  if (!statsOverride) loadSaveRobotStats(true);
  else loadSaveRobotStats(false);
  setUserSwitches();
  stateStartTime = millis();  
  beep(1);  
  Console.println(F("START"));  
  Console.print(F("Ardumower "));
  Console.println(VER);
  #ifdef USE_DEVELOPER_TEST
    Console.println("Warning: USE_DEVELOPER_TEST activated");
  #endif
  Console.print(F("Config: "));
  Console.println(name);  
  Console.println(F("press..."));
  Console.println(F("  d for menu"));    
  Console.println(F("  v to change console output (sensor counters, values, perimeter etc.)"));    
  Console.println(consoleModeNames[consoleMode]);
} 



void Robot::printMenu(){  
  Console.println();
  Console.println(F(" MAIN MENU:"));
  Console.println(F("1=test motors"));
  Console.println(F("2=test odometry"));
  Console.println(F("3=communications menu"));
  Console.println(F("5=calibrate IMU acc next side"));
  Console.println(F("6=calibrate IMU com start/stop"));  
  Console.println(F("7=delete IMU calib"));
  Console.println(F("8=ADC calib (perimeter sender, charger must be off)"));  
  Console.println(F("9=save user settings"));  
  Console.println(F("l=load factory settings"));  
  Console.println(F("r=delete robot stats"));  
  Console.println(F("x=read settings"));  
  Console.println(F("e=delete all errors"));  
  Console.println(F("0=exit"));  
  Console.println();
}



void Robot::testMotors(){
}

void Robot::menu(){  
}


void Robot::readSerial() {
  // serial input
  if (Console.available() > 0) {     
     char ch = (char)Console.read();
     resetIdleTime();
     switch (ch){
       case 'd': 
         menu(); // menu
         break;
  }
}


void Robot::loop()  {
  readSerial();
  if (millis() >= nextTimeInfo) {        
    nextTimeInfo = millis() + 1000; 
    printInfo(Console);
  }
  }