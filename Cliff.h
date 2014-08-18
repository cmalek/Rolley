#ifndef Rolley_Cliff_h
#define Rolley_Cliff_h

namespace rolley
{
    class Cliff
    {
        public:
            Cliff();
            void setup(uint8_t, uint8_t);

            boolean is_cliff();
            boolean is_front_cliff();
            boolean is_left_cliff();
            boolean is_right_cliff();

            String test();
        private:
            uint8_t _left_pin;
            uint8_t _right_pin;
    };
}
#endif
