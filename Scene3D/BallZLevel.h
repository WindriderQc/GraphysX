#pragma once
#include <string>

using namespace std;

namespace BallZ
{ 
	class BallZLevel
	{
	public:
		BallZLevel() {};
		BallZLevel(const string xmlFile, const string asciiFile, const bool baddFloor,	const string floorTexName, const string ringsFile,	const int iringColor,	const float fringSize, int numHuman) 
			: XMLFile(xmlFile)
			, ASCIIFilePath(asciiFile)
			, bAddFloor(baddFloor)
			, FloorTexName(floorTexName)
			, RingsFile(ringsFile)
			, iRingColor(iringColor)
			, fRingSize(fringSize)
			, iNumHuman(numHuman)
		{}
		
		
		const string XMLFile = "..\\StockRoom\\Suzanne1.xml";
		const string ASCIIFilePath = "..\\StockRoom\\Suzanne1.ASCII";
		const bool bAddFloor = true;
		const string FloorTexName = "concrete.png";
		const string RingsFile = "..\\StockRoom\\Suzanne1.obj";
		const int iRingColor = RGBA_YELLOW;
		const float fRingSize = 0.2f;
		int iNumHuman = 200;

	};

}