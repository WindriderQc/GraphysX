#pragma once

using namespace System;
using namespace System::IO;
using namespace System::IO::Ports;
using namespace System::Threading;


typedef unsigned char byte;


public ref class ArduinoControllerMain {

public:
	ArduinoControllerMain()
	{
		SetComPort();
		std::cout << "Serial comm constructor. ";
		if (portFound)
			std::cout << " PortFound!";
		else
			std::cout << " Port Not Found...";
	};
	~ArduinoControllerMain()
	{
		currentPort->Close();
	}

	void sendMsg(UCHAR command, UCHAR pin, UCHAR value)
	{
		array<byte>^ buffer = { 0, 0, 0, 0, 0 };
		buffer[0] = Convert::ToByte(16);
		buffer[1] = Convert::ToByte(command);
		buffer[2] = Convert::ToByte(pin);
		buffer[3] = Convert::ToByte(value);
		buffer[4] = Convert::ToByte(4);
		currentPort->Write(buffer, 0, 5);
		Thread::Sleep(2000);  // let time for the arduino to treat the command before we read the answer
	}
	String^  readMsg()
	{
		int intReturnASCII = 0;
		char charReturnValue = (Char)intReturnASCII;
		int count = currentPort->BytesToRead;
		String^ returnMessage = "";
		while (count > 0)
		{
			intReturnASCII = currentPort->ReadByte();
			returnMessage = returnMessage + Convert::ToChar(intReturnASCII);
			count--;
		}
		return returnMessage;
	}

	int readSonicDistance()
	{
		try
		{
			sendMsg(129, 0, 0);			
			String^ returnMessage = readMsg();
			
			int foundS1 = returnMessage->IndexOf("-Ok");
			returnMessage = returnMessage->Remove(foundS1);
			int distance = int::Parse(returnMessage); // string to int  //  System::String^ str = i.ToString(); // int to string  TODO: a mettre dans tools
			return(distance);
	
		}
		catch (Exception^ e)
		{
			String^ test = e->Message;
			return false;
		}
	}

private:
	SerialPort^ currentPort;
	bool portFound;

	void SetComPort()
	{
		try
		{
			array<String^>^ ports = SerialPort::GetPortNames();
			for each(String^ port in ports)
			{
				currentPort = gcnew SerialPort(port, 9600);
				currentPort->Open();
				if (DetectArduino())
				{
					portFound = true;
					break;
				}
				else
				{
					portFound = false;
				}
			}
		}
		catch (Exception^ e)
		{
			String^ test = e->Message;
		}
	}
	bool DetectArduino()
	{
		try
		{
			//The below setting are for the Hello handshake
			array<byte>^ buffer = { 0, 0, 0, 0, 0 };
			buffer[0] = Convert::ToByte(16);
			buffer[1] = Convert::ToByte(128);
			buffer[2] = Convert::ToByte(0);
			buffer[3] = Convert::ToByte(0);
			buffer[4] = Convert::ToByte(4);
			currentPort->Write(buffer, 0, 5);
			Thread::Sleep(1000);  // let time for the arduino to treat the command before we read the answer
			
			int intReturnASCII = 0;
			char charReturnValue = (Char)intReturnASCII;
			int count = currentPort->BytesToRead;
			String^ returnMessage = "";
			while (count > 0)
			{
				intReturnASCII = currentPort->ReadByte();
				returnMessage = returnMessage + Convert::ToChar(intReturnASCII);
				count--;
			}
			//ComPort.name = returnMessage;
		
			if (returnMessage->Contains("HELLO FROM ARDUINO"))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		catch (Exception^ e)
		{
			String^ test = e->Message;
			return false;
		}
	}
};

