//       _____ ______ _  _______        __   ___
//      / ____|  ____| |/ /_   _|      /_ | |__ \
//     | (___ | |__  | ' /  | |   __   _| |    ) |
//      \___ \|  __| |  <   | |   \ \ / / |   / /
//      ____) | |____| . \ _| |_   \ V /| |_ / /_
//     |_____/|______|_|\_\_____|   \_(_)_(_)____|

// author: Hloušek Matěj
// school: SSPU Opava
// year: 2016


#include <Arduino.h>
#include "sekacka.h"


Sekacka seki;


void setup() {
    seki.setup();
}

void loop() {
   seki.loop();
}
