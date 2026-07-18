#include "stdafx.h"
#include "Tools.h"
#include <direct.h> // pour _getcwd

#include "tinyxml.h"
#include "tiny_obj_loader.h"

namespace Tools
{
	std::mt19937 randomEngine((int)time(nullptr));

	int random_int(int min, int max)
	{
		std::uniform_int_distribution<int> rand(min, max);
		return(rand(randomEngine));
	}
	float random_float(float min, float max)
	{
		std::uniform_real_distribution<float> rand(min, max);
		return(rand(randomEngine));
	}
	int RandomDice(int Size) // Lancer de Dés evenly distributed.
	{
		// srand(UINT(time(NULL)));  // Called once so we can generate random number after...  
		static int const max = RAND_MAX / Size * Size;
		int r = rand();
		while (r >= max) { r = rand(); }
		return r % Size + 1;
	}
	GraphysX::Spline CreateTrianglePile(int NbrEtage, cTV_3DVECTOR pos, float BoxSize)
	{
		int iEtage, iLigne;
		GraphysX::Spline TrianglePos;
		cTV_3DVECTOR LineStart;
		//  determiné le nombre de boite dans la pile par NbrÉtage^2 je crois.  et établir un vecteur de CTV_3DVECTOR avec ce 
		//  nombre pour contruire la liste finale de position de la boite.
		// int iNbrBox = (NbrEtage*(NbrEtage + 1)) / 2;
		for (iEtage = 0; iEtage < NbrEtage; iEtage++){
			LineStart = cTV_3DVECTOR(pos.x - (BoxSize / 2)*iEtage, pos.y + (NbrEtage - iEtage)*BoxSize, pos.z);
			for (iLigne = 0; iLigne <= iEtage; iLigne++){
				TrianglePos.push_back(cTV_3DVECTOR(LineStart.x + (iLigne)*BoxSize, LineStart.y, LineStart.z));
			}
		}
		return(TrianglePos);
	}
	void DevConsoleCreate() {

		CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
		int consoleHandleR, consoleHandleW;
		long stdioHandle;
		FILE *fptr;

		AllocConsole();
		std::wstring strW = L"Dev Console";
		SetConsoleTitle((LPCWSTR)strW.c_str());

		HWND consoleWindow = GetConsoleWindow();
		SetWindowPos(consoleWindow, 0, 0, 500,1024, 320, SWP_NOZORDER);

		EnableMenuItem(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE, MF_GRAYED);
		DrawMenuBar(GetConsoleWindow());

		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleInfo);

		stdioHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
		consoleHandleR = _open_osfhandle(stdioHandle, _O_TEXT);
		fptr = _fdopen(consoleHandleR, "r");
		*stdin = *fptr;
		setvbuf(stdin, NULL, _IONBF, 0);

		stdioHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
		consoleHandleW = _open_osfhandle(stdioHandle, _O_TEXT);
		fptr = _fdopen(consoleHandleW, "w");
		*stdout = *fptr;
		setvbuf(stdout, NULL, _IONBF, 0);

		stdioHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
		*stderr = *fptr;
		setvbuf(stderr, NULL, _IONBF, 0);

		//if (!AttachConsole(ATTACH_PARENT_PROCESS))  // -1 == ATTACH_PARENT_PROCESS or Process ID
		//	System::Windows::Forms::MessageBox::Show(L"Unable to attach console window", L"Error", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Exclamation);

	}
	bool IsPositionInsideBoundingBox(cTV_3DVECTOR pos, cTV_3DVECTOR BoxMin, cTV_3DVECTOR BoxMax)
	{
		bool isInside = false;
		if ((pos.x > BoxMin.x) && (pos.x < BoxMax.x)){
			if ((pos.y > BoxMin.y) && (pos.y < BoxMax.y)){
				if ((pos.z > BoxMin.z) && (pos.z < BoxMax.z))
					isInside = true;
			}
		}
		return(isInside);
	}
	bool IsPositionInsideMesh(cTV_3DVECTOR pos, CTVMesh* mesh)
	{
		cTV_3DVECTOR BoxMin, BoxMax;
		mesh->GetBoundingBox(&BoxMin, &BoxMax);

		bool isInside = false;
		if ((pos.x > BoxMin.x) && (pos.x < BoxMax.x)){
			if ((pos.y > BoxMin.y) && (pos.y < BoxMax.y)){
				if ((pos.z > BoxMin.z) && (pos.z < BoxMax.z))
					isInside = true;
			}
		}
		return(isInside);
	}
	std::string getexepath()
	{
		//char result[MAX_PATH];
		//DWORD res = GetCurrentDirectoryA(MAX_PATH, result);
		//return std::string(result, res);
	
		char buffer[MAX_PATH];
		GetModuleFileNameA(NULL, buffer, MAX_PATH);
		std::string::size_type pos = std::string(buffer).find_last_of("\\/");
		return std::string(buffer).substr(0, pos);  // soustrait le nom de l'.exe et retour le path ensuite
	}
	void  MsgBox(std::string x, std::string y)
	{
		std::ostringstream oss1;
		oss1 << x;
		std::ostringstream oss2;
		oss2 << y;
		MessageBoxA(NULL, oss2.str().c_str(), oss1.str().c_str(), MB_OK | MB_ICONQUESTION);
	}
	std::string floatToStr(float f) {
		std::ostringstream os;
		os << f;
		return os.str();
	};
	std::string getTimeString()
	{
		time_t rawtime;
		struct tm * timeinfo;
		char buffer[80];

		time(&rawtime);
		timeinfo = localtime(&rawtime);

		strftime(buffer, 80, "%d.%m.%Y_%I.%M.%S.png", timeinfo);
		std::string str(buffer);
		return(str);
	};
	/*std::string intToString(int value)
	{
		int n = 42;
	char s[LMAX]; // Nombre maximal de chiffres + 1

	sprintf(s, "%d", n); // Conversion de l'entier

	}*/
	std::vector<cTV_3DVECTOR> XmlSplineRead(std::string filename)
	{
		std::vector<cTV_3DVECTOR> Spline;
		cTV_3DVECTOR vec;

		TiXmlDocument doc(filename.c_str());
		if (!doc.LoadFile()) {
			std::cout << "erreur lors du chargement" << std::endl;
			std::cout << "error #" << doc.ErrorId() << " : " << doc.ErrorDesc() << std::endl;
		}
		TiXmlHandle hdl(&doc); //  il est préférable d'utiliser des handles pour se déplacer dans notre arbre DOM
		TiXmlElement *elem = hdl.FirstChildElement().FirstChildElement().Element();
		if (!elem) {
			std::cout << "le noeud à atteindre n'existe pas" << std::endl;
		}
		while (elem) {
			vec.x = (float)atof(elem->Attribute("x"));
			vec.y = (float)atof(elem->Attribute("y"));
			vec.z = (float)atof(elem->Attribute("z"));
			Spline.push_back(vec);
			elem = elem->NextSiblingElement(); // iteration
		}
		return Spline;
	}
	void XmlSplineAddNode(cTV_3DVECTOR vec, std::string filename)
	{
		TiXmlDocument doc(filename.c_str());
		if (!doc.LoadFile()) {
		std::cout << "erreur lors du chargement" << std::endl;
		std::cout << "error #" << doc.ErrorId() << " : " << doc.ErrorDesc() << std::endl;
		return;
		}
		TiXmlHandle hdl(&doc); //  il est préférable d'utiliser des handles pour se déplacer dans notre arbre DOM
		TiXmlElement *elem = hdl.FirstChildElement().FirstChildElement().Element();
		if (!elem) {
		std::cout << "le noeud à atteindre n'existe pas" << std::endl;
		return;
		}
		TiXmlElement *f = doc.FirstChildElement(); // on récupère le noeud parent
		TiXmlElement NewPoint("point");
		NewPoint.SetDoubleAttribute("x", vec.x);
		NewPoint.SetDoubleAttribute("y", vec.y);
		NewPoint.SetDoubleAttribute("z", vec.z);
		f->InsertEndChild(NewPoint);
		doc.SaveFile(filename.c_str());  // enregistrement des modifications
	}
	GraphysX::Spline OBJtoSpline(std::string filePath)
	{
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		std::string err;
		bool ret = tinyobj::LoadObj(shapes, materials, err, filePath.c_str());
		if (!err.empty()) { std::cerr << err << std::endl; }// `err` may contain warning message.
		if (!ret)  exit(1);
		std::cout << "# of shapes    : " << shapes.size() << std::endl;
		std::cout << "# of materials : " << materials.size() << std::endl;

		UINT nbrVertices;
		GraphysX::Spline retSpline;
		for (size_t i = 0; i < shapes.size(); i++) {
			// SECTION INUTILE - ARRANGER POUR UTILISER mesh.positions!!!
			nbrVertices = shapes[i].VerticesPos.size();
			for (size_t v = 0; v < nbrVertices; v++) {
				retSpline.push_back(Vector3(shapes[i].VerticesPos[v].x, shapes[i].VerticesPos[v].y, shapes[i].VerticesPos[v].z));
			}
		}
		return(retSpline);
	}
}







// write / read system::xml
/*
using System;
using System.Xml;
namespace ReadingXML2
{
class Class1
{
static void Main(string[] args)
{
// Create a new file in C:\\ dir
XmlTextWriter textWriter = new XmlTextWriter("C:\\myXmFile.xml", null);
// Opens the document
textWriter.WriteStartDocument();
// Write comments
textWriter.WriteComment("First Comment XmlTextWriter Sample Example");
textWriter.WriteComment("myXmlFile.xml in root dir");
// Write first element
textWriter.WriteStartElement("Student");
textWriter.WriteStartElement("r", "RECORD", "urn:record");
// Write next element
textWriter.WriteStartElement("Name", "");
textWriter.WriteString("Student");
textWriter.WriteEndElement();
// Write one more element
textWriter.WriteStartElement("Address", ""); textWriter.WriteString("Colony");
textWriter.WriteEndElement();
// WriteChars
char[] ch = new char[3];
ch[0] = 'a';
ch[1] = 'r';
ch[2] = 'c';
textWriter.WriteStartElement("Char");
textWriter.WriteChars(ch, 0, ch.Length);
textWriter.WriteEndElement();
// Ends the document.
textWriter.WriteEndDocument();
// close writer
textWriter.Close();
}
}
}




//  read
namespace ReadingXML2
{
class Class1
{
void Read()
{
int ws = 0;
int pi = 0;
int dc = 0;
int cc = 0;
int ac = 0;
int et = 0;
int el = 0;
int xd = 0;
// Read a document
XmlTextReader ^textReader = gcnew XmlTextReader("C:\\books.xml");
// Read until end of file
while (textReader->Read())
{
XmlNodeType nType = textReader->NodeType;
// If node type us a declaration
if (nType == XmlNodeType::XmlDeclaration)
{
cout << "Declaration:";
string *Name = textReader->Name;// ->ToString());
xd = xd + 1;
}
// if node type is a comment
if (nType == XmlNodeType->Comment)
{
Console->WriteLine("Comment:" + textReader->Name->ToString());
cc = cc + 1;
}
// if node type us an attribute
if (nType == XmlNodeType->Attribute)
{
Console->WriteLine("Attribute:" + textReader->Name->ToString());
ac = ac + 1;
}
// if node type is an element
if (nType == XmlNodeType->Element)
{
Console->WriteLine("Element:" + textReader->Name->ToString());
el = el + 1;
}
// if node type is an entity\
if (nType == XmlNodeType->Entity)
{
Console->WriteLine("Entity:" + textReader->Name->ToString());
et = et + 1;
}
// if node type is a Process Instruction
if (nType == XmlNodeType->Entity)
{
Console->WriteLine("Entity:" + textReader->Name->ToString());
pi = pi + 1;
}
// if node type a document
if (nType == XmlNodeType->DocumentType)
{
Console->WriteLine("Document:" + textReader->Name->ToString());
dc = dc + 1;
}
// if node type is white space
if (nType == XmlNodeType->Whitespace)
{
Console->WriteLine("WhiteSpace:" + textReader->Name->ToString());
ws = ws + 1;
}
}
// Write the summary
Console->WriteLine("Total Comments:" + cc->ToString());
Console->WriteLine("Total Attributes:" + ac->ToString());
Console->WriteLine("Total Elements:" + el->ToString());
Console->WriteLine("Total Entity:" + et->ToString());
Console->WriteLine("Total Process Instructions:" + pi->ToString());
Console->WriteLine("Total Declaration:" + xd->ToString());
Console->WriteLine("Total DocumentType:" + dc->ToString());
Console->WriteLine("Total WhiteSpaces:" + ws->ToString());
};
};
}


*/
/*
vector<cTV_3DVECTOR> ReadWriteXML(string filename )
{
vecSpline Spline;
cTV_3DVECTOR point3D;
String ^ strFilename = gcnew String(filename.c_str());
XmlDocument ^ docXml = gcnew XmlDocument;
if (File::Exists(strFilename))
{
string RootName = "";
string Value = "";
string ElmName = "";

docXml->Load(strFilename);
XmlElement ^ Rootelm = docXml->DocumentElement;
MarshalString(Rootelm->Name, RootName);
cout << RootName  << endl;
XmlNodeList ^ lstPoint = Rootelm->ChildNodes;

for (int i = 0; i < lstPoint->Count; i++)
{
MarshalString(lstPoint[i]->Name, ElmName);
cout << ElmName << endl;
MarshalString(lstPoint[i]->ChildNodes[0]->Name, ElmName);
MarshalString(lstPoint[i]->ChildNodes[0]->InnerText, Value);
cout << ElmName + " : " + Value << endl;

MarshalString(lstPoint[i]->ChildNodes[1]->Name, ElmName);
MarshalString(lstPoint[i]->ChildNodes[1]->InnerText, Value);
cout << ElmName + " : " + Value + ", ";
point3D.x = atof(Value.c_str());
MarshalString(lstPoint[i]->ChildNodes[2]->Name, ElmName);
MarshalString(lstPoint[i]->ChildNodes[2]->InnerText, Value);
cout << ElmName + " : " + Value + ", ";
point3D.y = atof(Value.c_str());
MarshalString(lstPoint[i]->ChildNodes[3]->Name, ElmName);
MarshalString(lstPoint[i]->ChildNodes[3]->InnerText, Value);
cout << ElmName + " : " + Value << endl;
point3D.z = atof(Value.c_str());
Spline.push_back(point3D);
}
}
else
MessageBox::Show(L"The file " + strFilename + L" was not found");


//if (xmlReader->NodeType == XmlNodeType::Element && xmlReader->Name->Equals("Z"))

return Spline;
}
*/
/*
void SplineXmlRead()
{
TiXmlDocument doc("Media\\Points.xml");
if (!doc.LoadFile()){
cout << "erreur lors du chargement" << endl;
cout << "error #" << doc.ErrorId() << " : " << doc.ErrorDesc() << endl;
return;
}
TiXmlElement *elem = doc.FirstChildElement()->FirstChildElement();

TiXmlHandle hdl(&doc); //  il est préférable d'utiliser des handles pour se déplacer dans notre arbre DOM
elem = hdl.FirstChildElement().FirstChildElement().Element();

if (!elem){
cout << "le noeud à atteindre n'existe pas" << endl;
return;
}
vector<cTV_3DVECTOR> PointVector;
cTV_3DVECTOR vec;
while (elem){
vec.x = atof(elem->Attribute("x"));
vec.y = atof(elem->Attribute("y"));
vec.z = atof(elem->Attribute("z"));
PointVector.push_back(vec);

elem = elem->NextSiblingElement(); // iteration
}

vector<cTV_3DVECTOR>::iterator i;
for (i = PointVector.begin(); i != PointVector.end(); i++)
cout << i->x << " " << i->y << " " << i->z << endl;

elem = hdl.FirstChildElement().FirstChildElement().Element();
TiXmlElement *f = doc.FirstChildElement(); // on récupère le noeud parent
TiXmlElement NewPoint("point");
NewPoint.SetAttribute("x", "1");
NewPoint.SetAttribute("y", "2");
NewPoint.SetAttribute("z", "4");

f->InsertEndChild(NewPoint);
doc.SaveFile("Media\\Points2.xml");  // enregistrement des modifications
}*/