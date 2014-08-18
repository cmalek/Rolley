// Copyright (C) 2012 Mark R. Stevens
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// Library for SEN0038 wheel encoders
//

#ifndef Encoders_h
#define Encoders_h

#include <RolleyCommon.h>
#include <Arduino.h>

namespace rolley
{
    //! @class Encoders
    //!
    //! @brief Measure number of revolutions of an encoder
    //!
    //! With ideas from:
    //!     http://www.dfrobot.com/forum/index.php?topic=354.0
    //!     Lauren from DFRobot
    //!
    //! And
    //!     http://www.pololu.com/docs/0J20/2.a
    //!
    class Encoders
    {
        public:

            //! @brief Constructor
            //!
            Encoders();

            //! @brief Used to reduce ambiguity in pin assignment
            //!
            static const uint8_t LeftWheelEncoder  = 14;
            static const uint8_t RightWheelEncoder = 15;

            //! @brief Setup the interrupt callbacks
            //!
            void setup(int);

            //! @brief Compute the left distance traveled
            //!
            //! @param[in] Return the distance in meters
            //!
            float left();
            void reset_left();
            void set_left_direction(rolley::directions_t);

            //! @brief Compute the right distance traveled
            //!
            //! @param[in] Return the distance in meters
            //!
            float right();
            void reset_right();
            void set_right_direction(rolley::directions_t);

            float distance();
            float angle();

            String test();
        private:
            int _wheel_width;

            float convert(int);

    };
}
#endif
