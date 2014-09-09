#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <RolleyServo.h>

namespace rolley
{
    RolleyServo::RolleyServo() :
        _range_start(0),
        _range_end(180),
        _pos(90),
        _increment(1)
    {}

    void RolleyServo::setup(Servo *servo, uint8_t pin) 
    {
        this->_servo = servo;
        this->_servo->attach(pin);
        this->set_position(this->_pos);
    }

    int RolleyServo::get_position()
    {
        return (this->_pos);
    }

    void RolleyServo::set_position(int pos)
    {
        this->_servo->write(pos);
    }

    void RolleyServo::set_scan_range(int start = 0, int end = 180, int step = 1)
    {
        this->_range_start = start;
        this->_range_end = end;
        this->_range_step = step;
    }

    void RolleyServo::scan()
    {
        if (this->_pos <= this->_range_start) {
            this->set_position(this->_range_start);
            this->_pos = this->_range_start;
            delay(50);
        }

        if (this->_pos >= this->_range_end) {
            this->set_position(this->_range_end);
            this->_pos = this->_range_end;
            delay(50);
        }

        if (this->_pos + this->_increment > this->_range_end) {
            this->_increment = -this->_range_step;
        }
        if (this->_pos + this->_increment < this->_range_start) {
            this->_increment = this->_range_step;
        }
        this->_pos += this->_increment;
        this->set_position(this->_pos);
        delay(10);
    }

    char* RolleyServo::test()
    {
        int result;
        static char response[10];

        this->scan();
        result = snprintf(response, 10, "SP%03d;SI%+d", this->get_position(), this->_increment);
        return(response);
    }
}
