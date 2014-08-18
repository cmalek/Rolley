#ifndef Rolley_Bump_h
#define Rolley_Bump_h

#include <Bounce2.h>

namespace rolley
{
    class Bump
    {
        public:
            Bump();
            void setup(uint8_t, uint8_t, uint8_t);

            boolean update();
            boolean is_bump();
            boolean is_front_bump();
            boolean is_left_bump();
            boolean is_right_bump();

            String test();
        private:
            Bounce _left;
            Bounce _middle;
            Bounce _right;
    };
}
#endif
