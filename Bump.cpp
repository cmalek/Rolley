#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <Bump.h>

namespace rolley
{
    Bump::Bump() {}

    void Bump::setup(uint8_t left_pin, uint8_t middle_pin, uint8_t right_pin) 
    {
        // Setup bump sensors
        pinMode(left_pin, INPUT);
        digitalWrite(left_pin, HIGH); // Turn on pullup resistor
        pinMode(middle_pin, INPUT);
        digitalWrite(middle_pin, HIGH); // Turn on pullup resistor
        pinMode(right_pin, INPUT);
        digitalWrite(right_pin, HIGH); // Turn on pullup resistor

        this->_left.attach(left_pin); 
        this->_left.interval(5);
        this->_middle.attach(middle_pin); 
        this->_middle.interval(5);
        this->_right.attach(right_pin); 
        this->_right.interval(5);
        //
    }


    boolean Bump::update()
    {
        this->_left.update();
        this->_right.update();
        this->_middle.update();
    }

    boolean Bump::is_bump()
    {
        this->update();
        int left = this->_left.read();
        int right = this->_right.read();
        int middle = this->_middle.read();
        return((left == LOW) || (middle == LOW) || (right == LOW));
    }

    boolean Bump::is_front_bump()
    {
        this->update();
        int middle = this->_middle.read();
        return(middle == LOW);
    }

    boolean Bump::is_left_bump()
    {
        this->update();
        int left = this->_left.read();
        int right = this->_right.read();
        int middle = this->_middle.read();
        return((left == LOW) && (middle != LOW) && (right != LOW));
    }

    boolean Bump::is_right_bump()
    {
        this->update();
        int left = this->_left.read();
        int right = this->_right.read();
        int middle = this->_middle.read();
        return((right == LOW) && (middle != LOW) && (left != LOW));
    }

    // 
    // TEST
    //

    String Bump::test()
    {
        String response = String("");
        response += this->is_left_bump() ? "BL" : "0";
        response += ";";
        response += this->is_front_bump() ? "BF" : "0";
        response += ";";
        response += this->is_right_bump() ? "BR" : "0";
        return(response);
    }
}
