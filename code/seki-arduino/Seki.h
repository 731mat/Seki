//
// Created by mat on 5.11.16.
//

#ifndef ARDUMOWER_SEKI_H
#define ARDUMOWER_SEKI_H


#include "robot.h"

class Seki
{
public:
    String name;
    Seki();
    virtual void setup(void);
    virtual void loop(void);
};


#endif //ARDUMOWER_SEKI_H
