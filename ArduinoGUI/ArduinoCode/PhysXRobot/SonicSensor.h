#include <Servo.h>

// constants
#define TOO_CLOSE 10                    /**< distance to obstacle in centimeters */
#define MAX_DISTANCE (TOO_CLOSE * 100)   /**< maximum distance to track with sensor */
#define SERVO_1_PIN A2 //11
// Define pins for ultrasonic
#define TRIG_SONIC A1 //10
#define ECHO_SONIC A0 //9

#define SONIC_MAP_FROM 0
#define SONIC_MAP_TO 180
#define SONIC_MAP_INTERVAL 2
#define COMM_DELAY 15

class SonicSensor
{
   public:
        SonicSensor() 
        { }

        void setup(){
          pinMode(TRIG_SONIC, OUTPUT); // trig pin will have pulses output
          pinMode(ECHO_SONIC, INPUT); // echo pin should be input to get pulse width
          
          pinMode(SERVO_1_PIN, OUTPUT);
          servo1.attach(SERVO_1_PIN); 
          /*servo1.write(0);
          
          int pos = 0;    // variable to store the servo position
          for (pos = 0; pos <= 180; pos++) { // goes from 0 degrees to 180 degrees
            // in steps of 1 degree
            servo1.write(pos);              // tell servo to go to position in variable 'pos'
            delay(15);                       // waits 15ms for the servo to reach the position
            }
          for (pos = 180; pos >= 0; pos--) { // goes from 180 degrees to 0 degrees
            servo1.write(pos);              
            delay(15);                       
            }*/
          servo1.write(90);
        }
     		
		float read() {
			// Duration will be the input pulse width and distance will be the distance to the obstacle in centimeters
			float duration, distance;
			// Output pulse with 1ms width on trigPin
			digitalWrite(TRIG_SONIC, HIGH);
			delay(1);
			digitalWrite(TRIG_SONIC, LOW);
			// Measure the pulse input in echo pin
			duration = pulseIn(ECHO_SONIC, HIGH);
			// Distance is half the duration devided by 29.1 (from datasheet)
			distance = (duration/2) / 29.1;
			return distance;
		}		 
		void readNPrint(float angle){
      servo1.write(angle);
      Serial.print(angle);
      Serial.print(":");

     /* float a, b, c, d,e;  // TODO È faire un class average...  si ya pas une lib pour ca déjà
      a = read();
      b = read();
      c = read();

      Serial.print((a+b+c)/3);*/
      Serial.println(read());
      Serial.println(";");
      //
    } 
    void sweepRead(){
    
      servo1.write(SONIC_MAP_FROM);
      for(float x=SONIC_MAP_FROM; x <= SONIC_MAP_TO ; x+=SONIC_MAP_INTERVAL)
      {
        readNPrint(x);
        delay(COMM_DELAY);
      }
      //Serial.print("----reverse----");
      for(float y=SONIC_MAP_TO; y >= SONIC_MAP_FROM; y-=SONIC_MAP_INTERVAL)  // TODO:  inutile de le faire en double je crois....
      {
        readNPrint(y);
        delay(COMM_DELAY);
      }
      servo1.write( (SONIC_MAP_TO - SONIC_MAP_FROM) / 2);
    }
		Servo servo1; 
};



