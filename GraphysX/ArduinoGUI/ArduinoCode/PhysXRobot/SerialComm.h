

#define BUF_SIZE 4


class SerialComm
{
public:
      SerialComm() 
      {	 }
      void init(int baud)
      {
       clearBuf();  
       Serial.begin(baud);
       Serial.println("Ready");
      }
		  void clearBuf() 
		  {
  			int i = 0;
  			while ( i < BUF_SIZE) {
  				inputByte[i] = 0;
  				i++;
  			  }
		  }
		  bool readBuf()  
		  {
  			if (Serial.available() == BUF_SIZE) {
  				int i = 0;
  				while ( i < BUF_SIZE)
  				{
  					inputByte[i] = Serial.read();
  					delay(100);
  					i++;
  				}
  				return true;
  			} else {
  				return false;
  			}
		}
	
		byte inputByte[BUF_SIZE]; // message buffer
};

