// Motor controller:
#include "arduino_motor_driver.h"
 
// int dirPin, int speedPin, int brakePin
#define LEFT_MOTOR_INIT 7, 6, 5  
#define RIGHT_MOTOR_INIT 4, 3, 2

#define MAX_SPEED 255
#define SPEED 50


 class Vehicule
	{  
    public: 
        Vehicule()  : leftMotor(LEFT_MOTOR_INIT), rightMotor(RIGHT_MOTOR_INIT)
        {}  
    protected:
      
        void move() {
            leftMotor.setSpeed(SPEED);
            rightMotor.setSpeed(SPEED);
        }
        void stop() {
            leftMotor.setSpeed(0);
            rightMotor.setSpeed(0);
        }
        bool turn() {
           leftMotor.setSpeed(MAX_SPEED);
           rightMotor.setSpeed(-MAX_SPEED); 
        }
          
    private:
        Motor leftMotor;
        Motor rightMotor; 
 };

