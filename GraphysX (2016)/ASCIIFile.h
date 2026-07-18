#pragma once

#include <vector>
#include <string>
#include <fstream>
using namespace std;

namespace Tools {
	class ASCIIFile
	{
	public:
		 GRAPHYSX_API ASCIIFile();
		 GRAPHYSX_API vector<string> load(string fileName);
		 GRAPHYSX_API void print();

	private:
		vector<string> _ASCIIData;

	};

}