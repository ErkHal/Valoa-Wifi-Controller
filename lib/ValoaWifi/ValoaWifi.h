#ifndef VALOA_WIFI_H
#define VALOA_WIFI_H

#include "Arduino.h"

/*
###############################################
                                                
    ValoaWifi Controller written by ErkHal      
    for WeMos D1 Mini Pro hardware              
    https://github.com/ErkHal/Valoa-wifi       

###############################################
*/

/*
 Json buffer capacity, check this value for your requests & hardware
 from https://arduinojson.org/v5/assistant
*/
const int JSON_BUFFER_CAPACITY = 130;

class ValoaWifi {
    public:
        ValoaWifi(int redPin, int greenPin, int bluePin);
        void setColor(int redValue, int greenValue, int blueValue);
    private:
        int redChannel, greenChannel, blueChannel;
};

#endif