#include <Encoders.h>
#include <RolleyCommon.h>
#include <Arduino.h>

static volatile int left_count = 0;
static volatile int right_count = 0;

static volatile int last_left = 0;
static volatile int last_right = 0;

static int left_increment = 1;
static int right_increment = 1;

// Interrupt based on wheel encoder
//
ISR(PCINT1_vect)
{

  // Get the value at the pins
  //
  const int left = PINC & (1 << 0);
  const int right = PINC & (1 << 1);

  // Determine which pin to update
  // CPM: I admit that I don't understand why we're XORing
  //      current sample with the last sample
  if (left ^ last_left)
  {
    left_count += left_increment;
  }
  if (right ^ last_right)
  {
    right_count += right_increment;
  }

  // reset
  //
  last_left = left;
  last_right = right;
}

namespace rolley
{

    inline void ResetLeft()
    {
        noInterrupts();
        left_count = 30;
        interrupts();
    }

    inline void ResetRight()
    {
        noInterrupts();
        right_count = 30;
        interrupts();
    }

    inline int getRightCounts()
    {
        noInterrupts();
        const int tmp = left_count;
        interrupts();
        return tmp;
    }

    inline int getLeftCounts()
    {
        noInterrupts();
        const int tmp = right_count;
        interrupts();
        return tmp;
    }

    Encoders::Encoders() {}

    void Encoders::setup(float wheel_width)
    {
        this->_wheel_width = wheel_width;

        // Make sure the pin is set for input
        //
        pinMode(LeftWheelEncoder, INPUT);
        pinMode(RightWheelEncoder, INPUT);

        // Enable the pull-up resistor
        //
        digitalWrite(LeftWheelEncoder, HIGH);
        digitalWrite(RightWheelEncoder, HIGH);

        // Prevent interrupts while initializing
        //
        noInterrupts();

        // Pin 14 for left wheel, Pin 15 for the right wheel
        //
        PCICR = 1 << PCIE1;
        PCMSK1 |= (1 << PCINT8) | (1 << PCINT9);
        MCUCR = (1 << ISC00);

        // Clear the interrupt flags in case they were set before for any reason.
        // On the AVR, interrupt flags are cleared by writing a logical 1
        // to them.
        //
        PCIFR |= (1 << PCIF0) | (1 << PCIF1) | (1 << PCIF2);

        // re-enable interrupts
        //
        interrupts();
    }

    void Encoders::set_left_direction(rolley::directions_t direction)
    {
        if (direction == FORWARD) {
            left_increment = 1;
        } else {
            left_increment = -1;
        }
    }

    void Encoders::set_right_direction(rolley::directions_t direction)
    {
        if (direction == FORWARD) {
            right_increment = 1;
        } else {
            right_increment = -1;
        }
    }

    float Encoders::convert(int count)
    {
        static const float ppr = 20.0f;
        static const float pi = 3.1416f;
        static const float diameter = 0.0635f;
        static const float circumference = pi * diameter;
        return circumference * count / ppr;
    }

    void Encoders::reset_left()
    {
        ResetLeft();
    }

    void Encoders::reset_right()
    {
        ResetRight();
    }

    float Encoders::left()
    {
        return this->convert(getLeftCounts());
    }

    float Encoders::right()
    {
        return this->convert(getRightCounts());
    }

    float Encoders::distance()
    {
        return ((this->left() + this->right())/2);
    }

    char* Encoders::test()
    {
        int result;
        static char response[17];
        char left[7];
        char right[7];

        result = snprintf(response, 16, "EL%s;ER%s", dtostrf(this->left(), 5, 2, left), dtostrf(this->right(), 5, 2, right));
        return(response);
    }
}
