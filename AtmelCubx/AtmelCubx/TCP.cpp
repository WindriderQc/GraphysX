// TCP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Windows.h"

#include "TCPComm.h"


int _tmain(int argc, _TCHAR* argv[])
{
	CLTCPComm* clTCPComm = new CLTCPComm(CLIENT);
	clTCPComm->Run();
	//getch();
	Sleep(2000);
	return 0;
}

