#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <Compass.h>

namespace rolley
{
    Compass::Compass() {}

    void Compass::setup() {
        Wire.begin();
        this->_compass.init();
        this->_compass.enableDefault();

        /*
        Calibration values; the default values of +/-32767 for each axis
        lead to an assumed magnetometer bias of 0. Use the Calibrate example
        program to determine appropriate values for your particular unit.
        */
        this->_compass.m_min = (LSM303::vector<int16_t>){-2747, -2185, -1870};
        this->_compass.m_max = (LSM303::vector<int16_t>){+2581, +2467, +3182};
    }

    void Compass::update() {
        this->_compass.read();
    }

    float Compass::heading() {
        return this->_compass.heading();
    }

    char* Compass::test() {
        int result;
        static char response[10];
        char tmp[6];

        this->update();
        result = snprintf(response, 10, "cH%s", dtostrf(this->heading(), 5, 1, tmp));
        return(response);
    }
}

