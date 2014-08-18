#ifndef Rolley_Sonar_h
#define Rolley_Sonar_h

#include <NewPing.h>

#define SONAR_MAX_DISTANCE          200
#define SONAR_OBSTACLE_DISTANCE     15

namespace rolley
{

    class Sonar
    {
        public:
            Sonar();
            void setup(NewPing *);

            float get_distance();
            boolean is_wall();

            String test();
        private:
            NewPing *_sonar;
            uint8_t _sonar_obstacle_distance;
    };
}
#endif
