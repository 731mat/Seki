//
// Created by mat on 6.11.16.
//

#ifndef SEKI_DRIVERS_H
#define SEKI_DRIVERS_H

#define MAX_ECHO_TIME 5000
#define MIN_ECHO_TIME 200
#define NO_ECHO 0

// sonar
unsigned int readHCSR04(int triggerPin, int echoPin);


#endif //SEKI_DRIVERS_H
