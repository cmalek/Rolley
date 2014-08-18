#ifndef RolleyServo_h
#define RolleyServo_h

#include <Servo.h>

namespace rolley
{
    class RolleyServo
    {
        public:
            RolleyServo();
            void setup(::Servo *, uint8_t);

            int get_position();
            void set_position(int);
            void set_scan_range(int, int);
            void scan();

            String test();
        private:
            Servo *_servo;
            int _range_start;
            int _range_end;
            int _pos;
            int _increment;
    };
}
#endif
