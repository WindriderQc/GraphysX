#include "stdafx.h"
#include "ToolsNET.h"

namespace ToolsNET
{
	void fatalError(std::string errorString)
	{
		std::cout << "Fatal Error: " << errorString << std::endl;
		System::Diagnostics::Debug::WriteLine("Exit - FatalError: " + charsToSystemString(errorString.c_str()));
		exit(-1);
	}
	String^ charsToSystemString(const char* chars)
	{
		String^ str = gcnew String(std::string(chars).c_str());
		return(str);
	}
	String^ stdStringToSystemString(std::string string)
	{
		String^ str = gcnew String(string.c_str());
		return(str);
	}
	// convert_system_string
	void MarshalString(String ^ s, std::string& os) {
		using namespace Runtime::InteropServices;
		const char* chars =
			(const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
		os = chars;
		Marshal::FreeHGlobal(IntPtr((void*)chars));
	}
	void MarshalString(String ^ s, std::wstring& os) {
		using namespace Runtime::InteropServices;
		const wchar_t* chars =
			(const wchar_t*)(Marshal::StringToHGlobalUni(s)).ToPointer();
		os = chars;
		Marshal::FreeHGlobal(IntPtr((void*)chars));
	}
	std::string OpenFile(String^ filter) {
		OpenFileDialog^ sfd = gcnew OpenFileDialog();
		sfd->Filter = filter;
		if (sfd->ShowDialog() != System::Windows::Forms::DialogResult::OK)
		{
			return false;
		}
		std::string path;
		ToolsNET::MarshalString(sfd->FileName, path);
		return path;
	}
	String^ SaveFile(String^ filter) {
		SaveFileDialog^ sfd = gcnew SaveFileDialog();
		sfd->Filter = filter;
		if (sfd->ShowDialog() != System::Windows::Forms::DialogResult::OK)
		{
			return "";
		}
		return sfd->FileName;
	}
}