#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <Cliff.h>

namespace rolley
{
    Cliff::Cliff() {}

    void Cliff::setup(uint8_t left_pin, uint8_t right_pin) 
    {
        this->_left_pin = left_pin;
        this->_right_pin = right_pin;

        // Setup cliff sensors
        pinMode(this->_left_pin, INPUT);
        pinMode(this->_right_pin, INPUT);
    }

    boolean Cliff::is_cliff()
    {
        return(this->is_left_cliff() || this->is_front_cliff() || this->is_right_cliff());
    }

    boolean Cliff::is_front_cliff()
    {
        return(digitalRead(this->_left_pin) && digitalRead(this->_right_pin));
    }

    boolean Cliff::is_left_cliff()
    {
        return(digitalRead(this->_left_pin) && !digitalRead(this->_right_pin));
    }

    boolean Cliff::is_right_cliff()
    {
        return(digitalRead(this->_right_pin) && !digitalRead(this->_left_pin));
    }

    String Cliff::test()
    {
        String response = String("");
        response += this->is_left_cliff() ? "CL" : "0";
        response += ";";
        response += this->is_front_cliff() ? "CF" : "0";
        response += ";";
        response += this->is_right_cliff() ? "CR" : "0";
        return(response);
    }

}
