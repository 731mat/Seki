#include "drivers.h"

void setIBT2(int pinDir, int pinPWM, int speed){
  if (speed > 0){
    digitalWrite(pinDir, HIGH) ;
    analogWrite(pinPWM, abs(speed));
  } else {
    digitalWrite(pinDir, LOW) ;
    analogWrite(pinPWM, abs(speed));
  }
}
