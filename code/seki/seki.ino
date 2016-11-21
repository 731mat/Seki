//       _____ ______ _  _______        __   __
//      / ____|  ____| |/ /_   _|      /_ | /_ |
//     | (___ | |__  | ' /  | |   __   _| |  | |
//      \___ \|  __| |  <   | |   \ \ / / |  | |
//      ____) | |____| . \ _| |_   \ V /| |  | |
//     |_____/|______|_|\_\_____|   \_(_)_|(_)_|

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
