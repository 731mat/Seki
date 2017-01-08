//
// Created by mat on 5.11.16.
//

#ifndef SEKI_Sekacka_H
#define SEKI_Sekacka_H

/**
 * @file   sekacka.h
 * @Author Hloušek Matěj (matej.hlousek@email.cz)
 * @date   Listopad,2016
 *
 * Seki class. Třída přistupuje k zařízením sekačky a stará se o ovlád
 */
class Sekacka {
public:
    /**
     * A constructor.
     */
    Sekacka();
    /**
     * A pure virtual member.
     * metoda která nadstavý výchozí parametry programu.
     */
    virtual void setup(void);
    /**
     * A pure virtual member.
     * výchozí smyčka která se pořád opakuje
     */
    virtual void loop(void);


private:
   /**
     * Enum pro urční směru jízdy
     * určuje směr jízdy
     */
    enum {
        MOTOR_FRONT,          /**< enum value MOTOR_FRONT. */
        MOTOR_FRONT_LEFT,     /**< enum value MOTOR_FRONT_LEFT. */
        MOTOR_FRONT_RIGHT,    /**< enum value MOTOR_FRONT_RIGHT. */
        MOTOR_BACK,           /**< enum value MOTOR_BACK. */
        MOTOR_BACK_LEFT,      /**< enum value MOTOR_BACK_LEFT. */
        MOTOR_BACK_RIGHT,     /**< enum value MOTOR_BACK_RIGHT. */
        MOTOR_LEFT,           /**< enum value MOTOR_LEFT. */
        MOTOR_RIGHT,          /**< enum value MOTOR_RIGHT. */
        MOTOR_STOP,           /**< enum value MOTOR_STOP. */
    };
    
    /**
     * a function variable.
     * Metoda která vypisuje info
     */
    void printInfo();
    /**
     * Metoda která se stará o bnovu dat z senzoru
     */
    void readSensors();
    /**
     * Metoda která zařizuje čtení dat ze senzoru
     * @param char type enum
     */
    int readSensor(char);
    /**
     * Metoda přistupuje k třídám k pohybu daných motoru
     * @param char type enum
     * @param char type int hodnota
     */
    void motorPohyb(char,int);
    /**
     * Metoda která žídí motory
     */
    void motorUpdate();
    /**
     * Metoda která navraví hodnutu na dotaz vzdálenosti v daném směru
     * @param char type enum
     */
    bool sonarMuzu(char);

    /**
     * Metoda která vypisuje možnosti menu
     */
    void printMenu();
    /**
     * Metoda která zajistí výběr z měnu
     */
    void menu();
    /**
     * metoda která realtime čte Console
     */
    void readSerial();
    /**
     * Metoda která testuje motor
     */
    void testMotors();
    /**
     * Metoda která testuje senzor
     */
    void testSonar();

    void testButton();

    /**
     * metoda která realtime čte Bluetooth
     */
    void readBluetooth();

    void setDrive();

    /**
     * metoda která reaguje ma přerušení
     */
    void static naraz();

    /**
     * metoda která sestavuje jsoun data a následně posíla na seriovou linku definovanou jako ESP8266port
     */
    void printJsonData();

    void buzzer(int);
    void driveBluetooth();


    int previousButtonDrive;

    int loopCounter;
    /**
    * a private variable.
    * proměnná která zařizuje druh výpisu na Consoli
    */
    int usePrintInfo;
    /**
    * a public variable.
    * čas začátku smyčkyv loop
    */
    unsigned long stateStartTime;
    unsigned long timeButtonDelay;
    /**
    * čas kdy bude další výpis informací
    */
    unsigned long nextTimeInfo;
    /**
    * cas jake době bude vypsano info
    */
    int updateTimeInfo;
    /**
    *  cas další obnovy motoru
    */
    unsigned long nextTimeMotor;
    /**
    *  cas po jake době bude obnoven motor
    */
    int updateTimeMotor;
    /**
    *  smycky obnov motoru
    */
    int timeUpdateTime;
    /**
    *  smycky couvání
    */
    int loopAutoBack;
    /**
    *  cas obnovy
    */
    int timeRotage;
    /**
    *  maximalni cas otoceni
    */
    int timeRotageMotor;
    /**
    *  zapnití pohybu
    */
    bool drive;
    /**
    *  Bluetooth výstup pro manuální ovladani
    */
    char charBluetooth;

    char buttonUse;

    // --------- bumper ---------------------------------
    /**
    *  použití bumper k pohybu
    */
    char bumperUse;
    /**
    *  použití bumper center
    */
    char bumperCenterUse;
    /**
    *  použití bumper left
    */
    char bumperLeftUse;
    /**
    *  použití bumper right
    */
    char bumperRightUse;


    // --------- sonar ----------------------------------

    /**
    *  použití sonaru k pohybu
    */
    char sonarUse;
    /**
    *  použití sonaru LEFT
    */
    char sonarLeftUse;
    /**
    *  použití sonaru RIGHT
    */
    char sonarRightUse;
    /**
    *  použití sonaru CENTER RIGHT
    */
    char sonarCenterRightUse;
    /**
    *  použití sonaru CENTER LEFT
    */
    char sonarCenterLeftUse;
    /**
    *  vzdalenost center left
    */
    unsigned int sonarDistCenterLeft;
    /**
    *  vzdalenost center right
    */
    unsigned int sonarDistCenterRight;
    /**
    *  vzdalenost right
    */
    unsigned int sonarDistRight;
    /**
    *  vzdalenost left
    */
    unsigned int sonarDistLeft;
    /**
    *  cas další obnovy senzoru
    */
    unsigned long nextTimeSonar;
    /**
    *  vzdalenost STUJ
    */
    int distMin;
    /**
    *  vzdalenost SPOMAL
    */
    int distSlow;

    unsigned long nextOffBuzzer;
    

    /**
     * Enum pro povolení jízdy od senzoru
     * určení směru jízdy, použití pro senzory aby určili vzdálenost pro jístu v daném směru
     */
    enum {
        FRONT,        /**< enum value FRONT. */
        FRONT_SLOW,   /**< enum value FRONT_SLOW. */
        LEFT,         /**< enum value LEFT. */
        BACK,         /**< enum value BACK. */
        STOP,         /**< enum value STOP. */
    };
    /**
     * Enum pro určení senzoru
     * slouží k rozpoznání seznosru od sebe
     */
    enum {
        SEN_SONAR_CENTER_LEFT,    /**< enum value SEN_SONAR_CENTER_LEFT. */
        SEN_SONAR_CENTER_RIGHT,   /**< enum value SEN_SONAR_CENTER_RIGHT. */
        SEN_SONAR_LEFT,           /**< enum value SEN_SONAR_LEFT. */
        SEN_SONAR_RIGHT,          /**< enum value SEN_SONAR_RIGHT. */
    };


};
#endif //SEKI_Seki_H
