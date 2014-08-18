#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <Rolley.h>


namespace rolley
{
    Rolley::Rolley() {}

    void Rolley::setup(Servo *servo, NewPing *sonar) 
    {
        // Setup motors
        this->_motors.setup(LEFT_MOTOR_DIRECTION_PIN,
                            LEFT_MOTOR_SPEED_PIN,
                            RIGHT_MOTOR_DIRECTION_PIN,
                            RIGHT_MOTOR_SPEED_PIN);
        this->_servo.setup(servo, SERVO_PIN);
        this->_bump.setup(BUMP_LEFT_PIN, BUMP_MIDDLE_PIN, BUMP_RIGHT_PIN);
        this->_sonar.setup(sonar);
        this->_encoders.setup(WHEEL_BASE);
        this->_cliff.setup(CLIFF_LEFT_PIN, CLIFF_RIGHT_PIN);
    }

    //
    // MOTORS
    //

    void Rolley::forward(uint8_t speed)
    {
        /* 
        * Go forward.
        *
        * param speed: 0-250
        */
        this->_motors.forward(speed);
        this->_encoders.set_left_direction(FORWARD);
        this->_encoders.set_right_direction(FORWARD);
    }

    void Rolley::move_meters(uint8_t speed, float meters, int direction)
    {
        this->encoders_reset();
        if (direction == MOTOR_FORWARD) {
            this->forward(speed);
        } else {
            this->backward(speed);
        }
        while (fabs(this->encoders_distance()) < meters) {
            delay(100);
        }
        this->stop();
    }

    void Rolley::forward_meters(uint8_t speed, float meters)
    {
        this->move_meters(speed, meters, MOTOR_FORWARD);
    }

    void Rolley::backward_meters(uint8_t speed, float meters)
    {
        this->move_meters(speed, meters, MOTOR_REVERSE);
    }

    void Rolley::backward(uint8_t speed)
    {
        /* 
        * Go backward
        *
        * param speed: 0-250
        */
        this->_motors.backward(speed);
        this->_encoders.set_left_direction(BACK);
        this->_encoders.set_right_direction(BACK);
    }

    void Rolley::spin(rolley::directions_t direction, uint8_t speed)
    {
        /* 
        * Spin in place. 
        *
        * param direction: LEFT or RIGHT
        * param speed: 0-250
        */
        this->_motors.spin(direction, speed);
        if (direction == LEFT) {
            Serial.println("    spinning LEFT");
            this->_encoders.set_left_direction(BACK);
            this->_encoders.set_right_direction(FORWARD);
        } else {
            Serial.println("    spinning RIGHT");
            this->_encoders.set_left_direction(FORWARD);
            this->_encoders.set_right_direction(BACK);
        }
    }

    void Rolley::spin_degrees(rolley::directions_t direction, uint8_t speed, float degrees)
    {
        float angle;
        this->encoders_reset();
        this->spin(direction, speed);
        angle = this->encoders_angle();
        while (fabs(angle) < degrees) {
            delay(50);
            angle = this->encoders_angle();
        }
        this->stop();
    }

    void Rolley::stop()
    {
        this->_motors.stop();
    }

    //
    // SONAR
    //

    float Rolley::sonar_get_distance()
    {
        return(this->_sonar.get_distance());
    }

    boolean Rolley::is_sonar_wall()
    {
        return(this->_sonar.is_wall());
    }

    //
    // SERVO
    //

    int Rolley::servo_get_position()
    {
        return (this->_servo.get_position());
    }

    void Rolley::servo_set_position(int pos)
    {
        this->_servo.set_position(pos);
    }

    void Rolley::servo_set_scan_range(int start = 0, int end = 180)
    {
        this->_servo.set_scan_range(start, end);
    }

    void Rolley::servo_scan()
    {
        this->_servo.scan();
    }

    //
    // CLIFF
    //

    boolean Rolley::is_cliff()
    {
        return(this->_cliff.is_cliff());
    }

    boolean Rolley::is_front_cliff()
    {
        return(this->_cliff.is_front_cliff());
    }

    boolean Rolley::is_left_cliff()
    {
        return(this->_cliff.is_left_cliff());
    }

    boolean Rolley::is_right_cliff()
    {
        return(this->_cliff.is_right_cliff());
    }


    //
    // BUMP
    //

    boolean Rolley::bump_update()
    {
        this->_bump.update();
    }

    boolean Rolley::is_bump()
    {
        return(this->_bump.is_bump());
    }

    boolean Rolley::is_front_bump()
    {
        return(this->_bump.is_front_bump());
    }

    boolean Rolley::is_left_bump()
    {
        return(this->_bump.is_left_bump());
    }

    boolean Rolley::is_right_bump()
    {
        return(this->_bump.is_right_bump());
    }

    //
    // ENCODERS
    // 

    float Rolley::encoders_distance()
    {
        return this->_encoders.distance();
    }


    float Rolley::encoders_left_distance()
    {
        return this->_encoders.left();
    }

    void Rolley::encoders_reset_left_distance()
    {
        this->_encoders.reset_left();
    }

    float Rolley::encoders_right_distance()
    {
        return this->_encoders.right();
    }

    void Rolley::encoders_reset_right_distance()
    {
        this->_encoders.reset_right();
    }

    void Rolley::encoders_reset()
    {
        this->encoders_reset_left_distance();
        this->encoders_reset_right_distance();
    }

    float Rolley::encoders_angle()
    {
        return this->_encoders.angle();
    }

    // 
    // TEST
    //

    void Rolley::motor_test()
    {
        this->forward(100);
        delay(2000);
        this->backward(100);
        delay(2000);
        this->spin(LEFT, 100);
        delay(2000);
        this->spin(RIGHT, 100);
        delay(2000);
        this->stop();
        delay(2000);
    }

    void Rolley::sensor_test()
    {
        String status = String("");
        status += this->_servo.test();
        status += "|";
        status += this->_sonar.test();
        status += "|";
        status += this->_bump.test();
        status += "|";
        status += this->_encoders.test();
        status += "|";
        status += this->_cliff.test();
        Serial.println(status);
    }
}
