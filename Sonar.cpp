#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <Sonar.h>

namespace rolley
{

    Sonar::Sonar() : 
        _sonar_obstacle_distance(SONAR_OBSTACLE_DISTANCE)
    {}

    void Sonar::setup(NewPing *sonar) 
    {
        this->_sonar = sonar;
    }


    float Sonar::get_distance()
    {
        return(this->_sonar->ping_median() / US_ROUNDTRIP_CM);
    }

    boolean Sonar::is_wall()
    {
        float cm = this->get_distance();
        if ((cm > 0) && (cm < this->_sonar_obstacle_distance)) {
            return true;
        }
        return false;
    }

    String Sonar::test()
    {
        String response = String("");
        response += "sD";
        response += this->get_distance();
        response += ";";
        response += "sW";
        response += this->is_wall();
        return(response);
    }

}
