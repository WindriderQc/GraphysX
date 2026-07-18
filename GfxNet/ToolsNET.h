#pragma once
#include <vcclr.h>
#include <vector>
#include <string>
#include <list>
#include <iostream>

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Collections::Generic;
using namespace System::Xml;
using namespace System::IO;

namespace ToolsNET
{ 
	// Math //


	void fatalError(std::string errorString);
	String^ charsToSystemString(const char* chars);
	String^ stdStringToSystemString(std::string string);
	// convert_system_string
	void MarshalString(String ^ s, std::string& os);
	void MarshalString(String ^ s, std::wstring& os);

	std::string OpenFile(String^ filter);
	String^ SaveFile(String^ filter);
}


/*
// SYSTEM STRING TO CHAR*
char cPathToMesh[100] = { 0 };
if (Obj->PathToMesh->Length < sizeof(cPathToMesh)) // make sure it fits & allow space for null terminator
sprintf(cPathToMesh, "%s", Obj->PathToMesh);
else
System::Windows::Forms::MessageBox::Show(L"SceneBuilder - " + Obj->PathToMesh, L"PathToMesh trop long (100 char max)", System::Windows::Forms::MessageBoxButtons::OK);

*/