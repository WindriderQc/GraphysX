#include "stdafx.h"
#include "ASCIIFile.h"

namespace Tools {
	ASCIIFile::ASCIIFile()
	{
	}
	vector<string> ASCIIFile::load(string fileName)
	{
		ifstream file;
		file.open(fileName);
		if (file.fail())  {
			perror(fileName.c_str());  // TODO: semble ne pas marcher....    envoie le msg dans stderr... qui est le stream d'error par defalut.. mais est-ce lié a std::cout??
			std::cout << std::endl << "ASCII File loading error" << std::endl;
			system("PAUSE");
			exit(1);
		}
		string line;

		while (getline(file, line))  {
			_ASCIIData.push_back(line);
		}
		file.close();

		return _ASCIIData;
	}
	void ASCIIFile::print()
	{
		for (UINT i = 0; i < _ASCIIData.size(); i++)  {
			printf("%s", _ASCIIData[i].c_str());
			printf("\n");
		}
	}
}