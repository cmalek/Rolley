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
        this->_compass.setup();
    }

    //
    // MOTORS
    //

    void Rolley::forward(uint8_t speed)
    {
        /* 
        * Go forward.
        *
        * :param speed: 0-100, motors will move at this speed
        */
        this->_motors.forward(speed);
        this->_encoders.set_left_direction(FORWARD);
        this->_encoders.set_right_direction(FORWARD);
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
            this->_encoders.set_left_direction(FORWARD);
            this->_encoders.set_right_direction(BACK);
        } else {
            Serial.println("    spinning RIGHT");
            this->_encoders.set_left_direction(BACK);
            this->_encoders.set_right_direction(FORWARD);
        }
    }

    //
    // MOTORS: measured movement
    //

    void Rolley::_move_meters_setup(uint8_t speed, int direction)
    {
        this->encoders_reset();
        if (direction == MOTOR_FORWARD) {
            this->forward(speed);
        } else {
            this->backward(speed);
        }
	}

    void Rolley::_move_meters_now(uint8_t speed, float meters, int direction)
    {
        /*
         * This will cause the robot to move approximately
         * `meters` meters in `direction` (where direction
         * is one in [MOTOR_FORWARD, MOTOR_BACKWARD]) at
         * speed `speed`, and then stop. Don't return until
		 * all movement is done.
         *
         * :param speed:  0-100, motors will move at this speed
         * :param meters: robot will move approximately this
         *                many meters
         * :param direction:  one of MOTOR_FORWARD, MOTOR_BACKWARD
         */
		this->_move_meters_setup(speed, direction);
        while (fabs(this->encoders_distance()) < meters) {
            delay(100);
        }
        this->stop();
    }

    void Rolley::forward_meters_now(uint8_t speed, float meters)
    {
        /*
         * This will cause the robot to move approximately
         * `meters` meters forward at speed `speed`, and then 
         * stop.  Don't return until all movement is done.
         *
         * :param speed:  0-100, motors will move at this speed
         * :param meters: robot will move approximately this
         *                many meters
         */
        this->_move_meters_now(speed, meters, MOTOR_FORWARD);
    }

    void Rolley::backward_meters_now(uint8_t speed, float meters)
    {
        /*
         * This will cause the robot to move approximately
         * `meters` meters backward at speed `speed`, and then 
         * stop.
         *
         * :param speed:  0-100, motors will move at this speed
         * :param meters: robot will move approximately this
         *                many meters
         */
        this->_move_meters_now(speed, meters, MOTOR_REVERSE);
    }

	void Rolley::move_meters(uint8_t speed, float meters, int direction)
	{
		this->_move_meters_setup(speed, direction);
		this->_move_meters = meters;
	}
    
	void Rolley::forward_meters(uint8_t speed, float meters)
	{
        this->move_meters(speed, meters, MOTOR_FORWARD);
	}

	void Rolley::backward_meters(uint8_t speed, float meters)
	{
        this->move_meters(speed, meters, MOTOR_REVERSE);
	}

	boolean Rolley::is_done_moving()
	{
        if (fabs(this->encoders_distance()) >= this->_move_meters) {
			return true;
		}
		return false;
	}

    void Rolley::spin_degrees_now(rolley::directions_t direction, uint8_t speed, float degrees)
    {
        float start_angle = this->compass_heading();
        this->spin(direction, speed);
        this->compass_update();
        while (fabs(this->compass_heading() - start_angle) < degrees) {
            delay(50);
            this->compass_update();
        }
        this->stop();
    }

    void Rolley::spin_degrees(rolley::directions_t direction, uint8_t speed, float degrees)
    {
        float heading;

        this->compass_update();
        Serial.print("    compass:START:");
        Serial.print(this->_compass.test());
        heading = this->compass_heading();
        if (direction == LEFT) {
            this->_heading = heading - degrees;
        } else {
            this->_heading = heading + degrees;
        }
        if (this->_heading < 0) {
            this->_heading += 360;
        }
        if (this->_heading > 360) {
            this->_heading -= 360;
        }
        Serial.print(":target:");
        Serial.print(this->_heading);
        this->spin(direction, speed);
    }
    
	boolean Rolley::is_done_spinning()
	{
        this->compass_update();
        if (fabs(this->compass_heading() - this->_heading) <= 5) {
            Serial.print("    compass:DONE:");
            Serial.println(this->_compass.test());
			return true;
		}
		return false;
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
   
    // 
    // COMPASS
    //
   
    void Rolley::compass_update()
    {
        this->_compass.update();
    }

    float Rolley::compass_heading()
    {
        return this->_compass.heading();
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

    void Rolley::spin_test()
    {
        this->spin_degrees_now(LEFT, 40, 45);
        delay(2000);
        this->spin_degrees_now(RIGHT, 40, 45);
        delay(2000);
        this->spin_degrees_now(LEFT, 40, 90);
        delay(2000);
        this->spin_degrees_now(RIGHT, 40, 90);
        delay(2000);
        this->spin_degrees_now(LEFT, 40, 135);
        delay(2000);
        this->spin_degrees_now(RIGHT, 40, 135);
        delay(2000);
        this->spin_degrees_now(LEFT, 40, 180);
        delay(2000);
        this->spin_degrees_now(RIGHT, 40, 180);
        delay(5000);
    }

    void Rolley::movement_test()
    {
        Serial.print("FORWARD:  ");
        Serial.print(this->_encoders.test());
        this->encoders_reset();
        this->forward_meters_now(100, .5);
        Serial.print("|");
        Serial.println(this->_encoders.test());
        delay(2000);
        Serial.print("FORWARD:  ");
        this->encoders_reset();
        Serial.print(this->_encoders.test());
        this->backward_meters_now(100, .5);
        Serial.print("|");
        Serial.println(this->_encoders.test());
        delay(2000);
    }

    void Rolley::sensor_test()
    {
        Serial.print(this->_servo.test());
        Serial.print("|");
        Serial.println(this->_sonar.test());
        Serial.print("|");
        Serial.print(this->_bump.test());
        Serial.print("|");
        Serial.print(this->_encoders.test());
        Serial.print("|");
        Serial.print(this->_cliff.test());
        Serial.print("|");
        Serial.println(this->_compass.test());
    }
}
