#include <Arduino.h>

class Motor
    {
    public:
        Motor(int dirPin, int speedPin, int brakePin) 
            : motorDirPin(dirPin), motorSpeedPin(speedPin), motorBrakePin(brakePin), currentSpeed(0)
        {
           pinMode(dirPin, OUTPUT);
           pinMode(brakePin,OUTPUT);
        }
        void setSpeed(int speed)
        {
            currentSpeed = speed;
            if (speed > 0) {  //check the direction
                digitalWrite(motorDirPin, HIGH); //set direction pin to forward
      	        digitalWrite(motorBrakePin, LOW); //disengage brake
      	        analogWrite(motorSpeedPin, currentSpeed); //set speed pin to current speed
            }
            else if (speed < 0) { //check the direction
                digitalWrite(motorDirPin, LOW);// set direction pin to backward
                digitalWrite(motorBrakePin, LOW); //disengage brake
                analogWrite(motorSpeedPin, -currentSpeed); //set speed pin to current speed
            }
	    else { //check the direction
                digitalWrite(motorBrakePin, HIGH); //engage brake
                analogWrite(motorSpeedPin, 0); //set speed pin to 0
            }
        }  
        int getSpeed() const
        {
            return currentSpeed;
        }
        
    private:
       	int motorSpeedPin;
      	int motorDirPin;
      	int motorBrakePin;
        int currentSpeed;
};

