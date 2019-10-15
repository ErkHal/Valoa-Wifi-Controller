/*
###############################################
                                                
    ValoaWifi Controller written by ErkHal      
    for WeMos D1 Mini Pro hardware              
    https://github.com/ErkHal/Valoa-wifi       

###############################################
*/

#include <ValoaWifi.h>
#include "Arduino.h"

ValoaWifi::ValoaWifi(int redPin, int greenPin, int bluePin) {
  this->redChannel = redPin;
  this->greenChannel = greenPin;
  this->blueChannel = bluePin;
}

// This function writes the PWM values to color channel
void ValoaWifi::setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(this->redChannel, redValue);
  analogWrite(this->greenChannel, greenValue);
  analogWrite(this->blueChannel, blueValue);
}