#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <RolleyCommon.h>
#include <Drive.h>

namespace rolley
{
    Drive::Drive() {}

    void Drive::setup(uint8_t left_direction_pin,
                uint8_t left_speed_pin,
                uint8_t right_direction_pin,
                uint8_t right_speed_pin)  
    {

        this->_left_direction_pin = left_direction_pin;
        this->_left_speed_pin = left_speed_pin;
        this->_right_direction_pin = right_direction_pin;
        this->_right_speed_pin = right_speed_pin;

        // Setup motors
        pinMode(this->_left_direction_pin, OUTPUT);
        pinMode(this->_right_direction_pin, OUTPUT);
        pinMode(this->_left_speed_pin, OUTPUT);
        pinMode(this->_right_speed_pin, OUTPUT);
        digitalWrite(this->_left_direction_pin, LOW);
        digitalWrite(this->_right_direction_pin, LOW);
    }

    uint8_t Drive::convert_speed(uint8_t speed)
    {
        if (speed > 0) {
            return(map(speed, 1, 100, 110, 255));
        } else {
            return(0);
        }
    }

    void Drive::forward(uint8_t speed)
    {
        /* 
        * Go forward.
        *
        * param speed: 0-250
        */
        int act_speed = this->convert_speed(speed);
        analogWrite (this->_left_speed_pin, act_speed);
        digitalWrite(this->_left_direction_pin, MOTOR_FORWARD);    
        analogWrite (this->_right_speed_pin, act_speed);    
        digitalWrite(this->_right_direction_pin, MOTOR_FORWARD);
    }

    void Drive::backward(uint8_t speed)
    {
        /* 
        * Go backward
        *
        * param speed: 0-100
        */
        int act_speed = this->convert_speed(speed);
        analogWrite (this->_left_speed_pin, act_speed);
        digitalWrite(this->_left_direction_pin, MOTOR_REVERSE);    
        analogWrite (this->_right_speed_pin, act_speed);    
        digitalWrite(this->_right_direction_pin, MOTOR_REVERSE);
    }

    void Drive::spin(rolley::directions_t direction, uint8_t speed)
    {
        /* 
        * Spin in place. 
        *
        * param direction: LEFT or RIGHT
        * param speed: 0-250
        */
        int act_speed = this->convert_speed(speed);
        analogWrite (this->_left_speed_pin, act_speed);
        analogWrite (this->_right_speed_pin, act_speed);    
        if (direction == LEFT) {
            digitalWrite(this->_left_direction_pin, MOTOR_REVERSE);    
            digitalWrite(this->_right_direction_pin, MOTOR_FORWARD);
        } else {
            digitalWrite(this->_left_direction_pin, MOTOR_FORWARD);    
            digitalWrite(this->_right_direction_pin, MOTOR_REVERSE);
        }
    }

    void Drive::stop()
    {
        analogWrite (this->_left_speed_pin, 0);
        digitalWrite(this->_left_direction_pin, MOTOR_REVERSE);    
        analogWrite (this->_right_speed_pin, 0);    
        digitalWrite(this->_right_direction_pin, MOTOR_REVERSE);
    }

    // 
    // TEST
    //

    void Drive::test()
    {
        this->forward(100);
        delay(2000);
        this->backward(100);
        delay(2000);
        this->spin(LEFT, 100);
        delay(2000);
        this->spin(RIGHT, 100);
        delay(2000);
        this->stop();
        delay(2000);

    }
}
