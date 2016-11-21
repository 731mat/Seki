//
// Created by mat on 6.11.16.
//

#ifndef SEKI_IBT_H
#define SEKI_IBT_H


/**
 * @file   ibt.h
 * @Author Hloušek Matěj (matej.hlousek@email.cz)
 * @date   Listopad,2016
 *
 * IBT class. Třída přistupuje k regulátoru napětí pro motory
 */
class Ibt {
public:
    /**
     * A constructor.
     * @param pinMotorLeftEnable pin na kterém je enable
     * @param pinMotorPWML pin na který budeme posílat PWM pro jízdu jedním směrem
     * @param pinMotorPWMR pin na který budeme posílat PWM pro jízdu druhým směrem
     * @param pinMotorSense pin kterým probíhá čtení aktuálního proudu regulátoru
     */
    Ibt(int pinMotorEnable, int pinMotorPWMR, int pinMotorPWML, int pinMotorSense);
    /**
     * Metoda která nadstavuje privátní proměnné třídy
     * @param směr type bool - 1 vpřed - 0 vzad
     * @param value type int hodnota výstupního proudu z regulátoru v PWM od 0 do 255
     */
    void setData(bool smer, int value);
    /**
     * Metoda která nastaví privátní proměnné pro pohyb na 0 a směr 0
     */
    void setStop();
    /**
     * Metoda která aplikuje privátní proměnné k danému výstupu.
     */
    void aplikovat();
    /**
     * Metoda vypisuje členská data SMER
     */
    bool getSmer();
    /**
     * Metoda vypisuje členská data VALUE
     */
    int getValue();

private:
    /**
    *   na kterém pinu se nachází enable
    */
    int pinMotorEnable;
    /**
    *  pin na který se aplikuje pwm pro jízdu jedním směrem
    */
    int pinMotorPWML;
    /**
    *  pin na který se aplikuje pwm pro jízdu druhým směrem
    */
    int pinMotorPWMR;
    /**
    *  pin na kterém měříme hodnotu napětí na regulátoru
    */
    int pinMotorSense;
    /**
    *  směr jízdy 1,0
    */
    bool smer;
    /**
    *  hodnota PWM od 0 do 255
    */
    int value;
};


#endif //SEKI_IBT_H
