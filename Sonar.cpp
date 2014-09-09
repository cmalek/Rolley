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
        return(this->_sonar->ping_cm());
    }

    void Sonar::set_wall_distance(float distance)
    {
        this->_sonar_obstacle_distance = distance;
    }

    boolean Sonar::is_wall()
    {
        float cm = this->get_distance();
        if ((cm > 0) && (cm < this->_sonar_obstacle_distance)) {
            return true;
        }
        return false;
    }

    char* Sonar::test()
    {
        static char response[16];
        int result;
        char tmp[5];

        result = snprintf(response, 15, "sD%s;sW%d", dtostrf(this->get_distance(), 4, 1, tmp), this->is_wall());
        return(response);
    }

}
