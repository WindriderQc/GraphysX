#pragma once

#include <vector>
#include <string>
#include <fstream>
using namespace std;

namespace Tools {
	class ASCIIFile
	{
	public:
		 ASCIIFile();
		 vector<string> load(string fileName);
		 void print();

	private:
		vector<string> _ASCIIData;

	};

}