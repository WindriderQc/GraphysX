// PhysXRobot
// Robot Vehicle firmware for the Arduino platform
#include "Robot.h"
#include "SerialComm.h" //* This uses a 4 byte message 
/*
	Byte 0 is the start of command marker. This is always decimal 16 converted to byte (Convert.ToByte(16);)
	Byte 1 is the type of command: 127 = Send data to pins  128 = Identify
	Byte 2 is the pin to receive data
	Byte 3 is the valus for the pin

	So send 100 to pin 3 is: 16,127,3,100
	And ask the Arduino to return the identify message is: 16,128,0,0
*/
//#include "Tools.h"
//#define RANDOM_ANALOG_PIN 5             /**< unused analog pin to use as random seed */
// 0 a 1  Rx Tx
// 2 a 7 pour moteur
#define TEST_PIN A5
// 9 a 11 pour sonic
#define LED_PIN A4


SerialComm serialComm;
Robot robot;

#include <LiquidCrystal.h>
//#include "LCD_Optrex.h"

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
//LCD_Optrex lcd(13, A5, 12, 11, 10, 9, 8);

void setup()
{
  pinMode(TEST_PIN, OUTPUT);
	pinMode(LED_PIN, OUTPUT);
	digitalWrite(LED_PIN, HIGH);
	delay(2000);
	digitalWrite(LED_PIN, LOW);
  serialComm.init(9600);
  robot.init();
  lcd.begin(20,4);
  lcd.clear(); // required in order to have the first print succesful
  lcd.print("Hello!");
}

void loop()
{
	//unsigned long currentTime = millis();
  lcd.setCursor(15,1);
  lcd.print(millis()/1000);
  
	int distance = robot.sonicSensor.read();
	if ( distance <= TOO_CLOSE && distance >= 0) {   //(0 or less means over range)
		digitalWrite(LED_PIN, HIGH);
	}
	else {
		digitalWrite(LED_PIN, LOW);
  }
  
  if(serialComm.readBuf())
  {
    if(serialComm.inputByte[0] == 16) //Check for start of Message 
    {     
       switch (serialComm.inputByte[1]) //Detect Command type
       {
        case 127:  //Set PIN and value
             switch (serialComm.inputByte[2])
              {
                case 11:
                  robot.sonicSensor.servo1.write(serialComm.inputByte[3]);
                  Serial.print("Sonic Sensor Servo Set");
                  break;
                case TEST_PIN:
                  if(serialComm.inputByte[3] == 255) {
                    digitalWrite(TEST_PIN, HIGH); 
                    Serial.print("TEST pin Set");
                  }
                  else {
                    digitalWrite(TEST_PIN, LOW); 
                  }
                break;
              } 
            break;
        case 128:  // Welcome Message
            Serial.print("HELLO FROM ARDUINO");   
            break;
        case 129:  //Read Ultrasonic sensor
            Serial.print(distance); 
            break;
    		case 130:  // SweepScan Sonic Sensor
      			robot.sonicSensor.sweepRead();
      			break;		
        case 255:
            Serial.print("SHUTDOWN");
            break;
        case 2:  // Robot Stop 
        case 4:  // Robot TurnL   // TODO : improve... car tourne juste d'un coté présentement.
        case 6:  // Robot TurnR 
        case 8:  // Robot Move   // TODO : improve... va juste vers l'avant présentement.
            robot.iAction = serialComm.inputByte[1];
            break; 
        } 
        serialComm.clearBuf();
        Serial.print("-Ok");  //Let the PC know we are ready for more data   //  TODO :  a remplacer par un buffer circulaire
    }
  }
	robot.run(); 
  lcd.setCursor(0,3);
  lcd.print(robot.sAction);
 
}
