#include "Vehicule.h"
#include "SonicSensor.h"




class Robot : public Vehicule
    {
    public:
        Robot()  {   }
               
        int iAction = 2;
        String sAction = "Stop";
      	SonicSensor sonicSensor;
        void init()
        {
          sonicSensor.setup(); 
        }
        /*
         *  Update the state of the robot based on input from sensor and remote control.
         *  Must be called repeatedly while the robot is in operation.
         */
        void run() {
             switch(iAction) {
                case 8:
                  move();
                  sAction = "Move";
                  break;
                case 2:
                  stop();
                  sAction = "Stop";
                  break;
                case 4:
                  turn();
                  sAction = "RotL";
                  break;
                case 6:
                  turn();
                  sAction = "RotR";
                  break;
              }
         }
	     /* bool obstacleAhead(unsigned int distance)
        {
            return (distance <= TOO_CLOSE);
        }*/
		
		
	
	
		
	private:
	
}; 

