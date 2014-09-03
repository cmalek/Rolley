#ifndef Rolley_Compass_h
#define Rolley_Compass_h

#include <Wire.h>
#include <LSM303.h>

namespace rolley
{
    class Compass
    {
        public:
            Compass();
            void setup();

            void update();
            float heading();

            char* test();
        private:
            LSM303 _compass;
    };
}
#endif
