using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Globalization;

using System.IO;
using System.IO.Ports;

using ScreenSys;




// TODO :  creer  define pour wait time constant pour la comm serie...  presentement un thread.sleep de 1000 ou 2000.... :S

namespace GraphysX
{
    public partial class ArduinoGUI : Form
    {
        public              
            ArduinoGUI()
        {
            InitializeComponent();
        }

        private SonicMap sonicMap;
        private FormTv mainViewport;
     

        private void bSetPin_Click(object sender, EventArgs e)
        {
            //this.serialPort1.Write("7");  // In the future,  you'll expand on this to write your custom data to the board
	        byte command, pin, value;
            command = 127;
            pin = Convert.ToByte(this.textBox1.Text);
            int v = 0;
            if(this.checkPinState.Checked) v = 255;
            value = Convert.ToByte(v);
		    sendMsg(command, pin, value);
            /* switch (command) {			 
		        case 128: case 129: case 255:
		        default:
			        SerialCtrl.sendMsg(command, 0, 0); } */
		
		    String sMsg = readMsg();
            this.labelReceivedMsg.Text = sMsg;
            if (sMsg.Contains("SHUTDOWN")) 
		    {
                //Exit();
		    }	
		        
            this.labelDistance.Text = Convert.ToString(readSonicDistance());  // TODO : bug si est plus tot car le -OK dans le received msg est en double...
        }
        private void ArduinoGUI_Load(object sender, EventArgs e)
        {
            mainViewport = new FormTv();
            String[] ports = SerialPort.GetPortNames();
            comboBoxPort.Items.AddRange(ports);
        }
        private void ArduinoGUI_FormClosing(object sender, FormClosingEventArgs e)
        {
            this.serialPort1.Close();
            this.mainViewport.Close();
          
        }
        private void comboBoxPort_SelectedValueChanged(object sender, EventArgs e)
        {
            serialPort1.PortName = comboBoxPort.Text; 
            serialPort1.Open();
         
           
            if (DetectArduino())
            {
                Console.Write(" Arduino PortFound!");
            }
            else
            {
                serialPort1.Close();
                Console.Write(" Arduino Port Not Found...");
            }
        }
        private bool DetectArduino()
	    {
			//The below setting are for the Hello handshake
			byte[] buffer = { 0, 0, 0, 0 };
			buffer[0] = Convert.ToByte(16);
			buffer[1] = Convert.ToByte(128);
			buffer[2] = Convert.ToByte(0);
			buffer[3] = Convert.ToByte(0);
			this.serialPort1.Write(buffer, 0, 4);
			Thread.Sleep(1000);  // let time for the arduino to treat the command before we read the answer
			
			int intReturnASCII = 0;
			char charReturnValue = (Char)intReturnASCII;
			int count = this.serialPort1.BytesToRead;
			String returnMessage = "";
			while (count > 0)
			{
				intReturnASCII = this.serialPort1.ReadByte();
				returnMessage = returnMessage + Convert.ToChar(intReturnASCII);
				count--;
			}
			//ComPort.name = returnMessage;
		
			if (returnMessage.Contains("HELLO FROM ARDUINO"))
			{
                textBoxSerial.Text = returnMessage;
                return true;
			}
			else
			{
				return false;
			}
	    }
        public void sendMsg(byte command, byte pin, byte value)
	    {
		    byte[] buffer = { 0, 0, 0, 0 };
		    buffer[0] = Convert.ToByte(16);
		    buffer[1] = Convert.ToByte(command);
		    buffer[2] = Convert.ToByte(pin);
		    buffer[3] = Convert.ToByte(value);
		    serialPort1.Write(buffer, 0, 4);  // TODO :  mettre un check si port open sinon crash
		    Thread.Sleep(2000);  // let time for the arduino to treat the command before we read the answer
	    }
	    public String readMsg()
	    {
		    int intReturnASCII = 0;
		    char charReturnValue = (Char)intReturnASCII;
		    int count = serialPort1.BytesToRead;
		    String returnMessage = "";
		    while (count > 0)
		    {
			    intReturnASCII = serialPort1.ReadByte();
			    returnMessage = returnMessage + Convert.ToChar(intReturnASCII);
			    count--;
		    }
		    return returnMessage;
	    }
	    public int readSonicDistance()
	{
		try   //  TODO : good pratice ...  pas mettre de try catch
		{
			sendMsg(129, 0, 0);			
			String returnMessage = readMsg();
			
			int foundS1 = returnMessage.IndexOf("-Ok");
			returnMessage = returnMessage.Remove(foundS1);
			int distance = int.Parse(returnMessage); // string to int  //  System::String^ str = i.ToString(); // int to string  TODO: a mettre dans tools
			return(distance);
	
		}
		catch (Exception e)
		{
			String test = e.Message;
			return -1;
		}
	}
        
	    
        private void bSonicScan_Click(object sender, EventArgs e)
        {
            byte command, pin, value;
            command = 130;
            pin = 0;
            value = 0;

            //String clean = readMsg();
            sendMsg(command, pin, value);
            Thread.Sleep(3500);  // let time for the arduino to treat the command before we read the answer
            String sMsg = readMsg();
            textBoxSerial.Text = sMsg;
            if (sMsg != null)
            {
                sonicMap = new SonicMap(sMsg);
            }
            else
            { 
            //  Exception 
            }
           
            
            // fill(0, 255, 0);
           // ellipse(x + 250, y + 250, 10, 10);
            System.Drawing.SolidBrush myBrush = new System.Drawing.SolidBrush(System.Drawing.Color.Blue);
            System.Drawing.Graphics formGraphics;
            formGraphics = this.CreateGraphics();

            for(int i = 0; i < sonicMap.map.Length; i++) 
            {
                formGraphics.FillRectangle(myBrush, new Rectangle(Convert.ToInt16(sonicMap.map[i].x + 450)
                                                                , Convert.ToInt16(sonicMap.map[i].z + 100)
                                                                , 5, 5));
            }
           
            myBrush.Color = System.Drawing.Color.Red;
            formGraphics.FillRectangle(myBrush, new Rectangle(450, 100, 10, 10));
            formGraphics.FillRectangle(myBrush, new Rectangle(450, 100 + 50, 15, 2));
            formGraphics.FillRectangle(myBrush, new Rectangle(450, 100 + 100, 15, 2));
            formGraphics.FillRectangle(myBrush, new Rectangle(450, 100 + 150, 15, 2));
            formGraphics.FillRectangle(myBrush, new Rectangle(450, 100 + 200, 15, 2));
            formGraphics.FillRectangle(myBrush, new Rectangle(450, 100 + 250, 15, 2));
            formGraphics.FillRectangle(myBrush, new Rectangle(450, 100 + 500, 15, 2));
            myBrush.Dispose();
            formGraphics.Dispose();

        }
        private void bUp_Click(object sender, EventArgs e)
        {
            sendMsg(8, 0, 0);
        }
        private void bLeft_Click(object sender, EventArgs e)
        {
            sendMsg(4, 0, 0);
        }
        private void bRight_Click(object sender, EventArgs e)
        {
            sendMsg(6, 0, 0);
        }
        private void bStop_Click(object sender, EventArgs e)
        {
            sendMsg(2, 0, 0);
        }
    }

    // This is pretending to be the data structure you are using
    public class AngleRadius
    {
        private double mAngle;
        private double mRadius;

        // constructor from string for convenience
        public AngleRadius(string AngleRadiusString)
        {
            string[] angleRadiusParts = AngleRadiusString.Split(':');

            Console.WriteLine(angleRadiusParts[0] + "   -   " + angleRadiusParts[1]);
            mAngle = double.Parse(angleRadiusParts[0], CultureInfo.InvariantCulture);
            mRadius = double.Parse(angleRadiusParts[1], CultureInfo.InvariantCulture);
            Console.WriteLine(mAngle + "   **   " + mRadius + " - parsed");

            // change from polar coordinate to cartesian coordinate
            x = Convert.ToInt16(mRadius * Math.Cos(mAngle * Math.PI / 180)); // convert degrees to radians
            z = Convert.ToInt16(mRadius * Math.Sin(mAngle * Math.PI / 180));
        }

        public int x { get; private set; }
        public int z { get; private set; }
    }
    public class SonicMap
    {
        public SonicMap(string sonicMsg)
        {
            map = new AngleRadius[360 / 2 + 2];  // 2 x 180 (back n forth) divisé par intervalle 2  + 2 pour le repeat du premier et dernier
            string[] entries = sonicMsg.Split(';');

            int i = 0;
            foreach (string s in entries)
            {
                if (!s.Contains("Ok"))
                {
                    map[i] = new AngleRadius(s);
                    i++;
                }
            }
        }

        public AngleRadius[] map;
    }
}
