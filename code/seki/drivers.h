//
// Created by mat on 6.11.16.
//

#ifndef SEKI_DRIVERS_H
#define SEKI_DRIVERS_H

#define MAX_ECHO_TIME 5000
#define MIN_ECHO_TIME 200
#define NO_ECHO 0

/**
 * @file   drivers.h
 * @Author Hloušek Matěj (matej.hlousek@email.cz)
 * @date   Listopad,2016
 *
 * drivers class. Třída pro přístup k modulům
 */

/**
 * funkce k čtení vzdálenost z sonaru
 * @param triggerPin
 * @param echoPin
 * @return
 */
unsigned int readHCSR04(int triggerPin, int echoPin);

/**
 * funkce k ovládání motoru moweru
 * @param pin na kterém pinu je připojený regulátor motoru
 * @param value hodnota rychlosti motoru
 */
void motorMower(int pin,int value);


#endif //SEKI_DRIVERS_H
