#include "stdafx.h"
#include "TV3DMoteur.h"


CLTV3DMoteur *CLTV3DMoteur::_singleton = NULL;

/*Constructeur de la classe*/
CLTV3DMoteur::CLTV3DMoteur()
{
	clGlobalVar = CLGlobalVar::getInstance();
	char * PathToApp = clGlobalVar->SearchPath;

	// Create the pTV Interface first:
	pTV = new CTVEngine();
	pTV->SetSearchDirectory(PathToApp);
	pTV->SetDebugMode(true, true); // Set the debug file/options. Do this before the 3D init so it can log any errors found during init.
	pTV->SetDebugFile(strcat(PathToApp, "\\debugfile.txt"));
	pTV->EnableProfiler(true, true); // silent profiler only shows in debug file
	pTV->AllowMultithreading(true);
	pTV->Init3DWindowed(clGlobalVar->w3DWindowHandle); // Init the engine
	pTV->SetAntialiasing(true, cTV_MULTISAMPLE_2_SAMPLES);
	pTV->GetViewport()->SetAutoResize(true);// engine will analyse the window size and update the internal viewport when needed
	pTV->SetAngleSystem(cTV_ANGLE_DEGREE);
	pTV->ShowWinCursor(true);

	pTV->DisplayFPS(true,0);

	pTV->AddToLog("TV3DMoteur init completed.");
	//pTV->SetVSync(false);  // tester les 2 pour voir la différence...

	//pTV->Screenshot("test.png", cTV_IMAGE_PNG);

	pTVScene = new CTVScene();
	pTVMaths = new CTVMathLibrary();
	pTVMaterialfactory = new CTVMaterialFactory();
	pTVTexturefactory = new CTVTextureFactory();
	p2DImmediate = new CTVScreen2DImmediate();


	// Create the Standard lighting material
	iStdMaterial = pTVMaterialfactory->CreateMaterial("StdMat");
	pTVMaterialfactory->SetAmbient(iStdMaterial, 0.1f, 0.1f, 0.1f, 1);
	pTVMaterialfactory->SetDiffuse(iStdMaterial, 0.8f, 0.8f, 0.8f, 1);
	pTVMaterialfactory->SetSpecular(iStdMaterial, 0.2f, 0.2f, 0.2f, 1);
	pTVMaterialfactory->SetPower(iStdMaterial, 20);
	pTVMaterialfactory->SetEmissive(iStdMaterial, 0, 0, 0, 1);
	
	//Create the translucent Material
	iTranslucentMaterial = pTVMaterialfactory->CreateMaterial("TranslucentMat");
	pTVMaterialfactory->SetAmbient(iTranslucentMaterial, 0.25f, 0.25f, 0.25f, 1);
	pTVMaterialfactory->SetDiffuse(iTranslucentMaterial, 0.75f, 0.75f, 0.75f, 1);
	pTVMaterialfactory->SetSpecular(iTranslucentMaterial, 1, 1, 1, 1);
	pTVMaterialfactory->SetPower(iTranslucentMaterial, 15);

	// Create the GlassStylish lighting material
	iGlassMat = pTVMaterialfactory->CreateMaterial("BallShellMat");
	pTVMaterialfactory->SetAmbient(iGlassMat, 0.1f, 0.1f, 0.1f, 0.4f);
	pTVMaterialfactory->SetDiffuse(iGlassMat, 0.8f, 0.8f, 0.8f, 0.4f);
	pTVMaterialfactory->SetSpecular(iGlassMat, 0.2f, 0.2f, 0.2f, 0.4f);
	pTVMaterialfactory->SetPower(iGlassMat, 20);
	pTVMaterialfactory->SetEmissive(iGlassMat, 0, 0, 0, 0.3f);
}
/*Destructeur de la classe*/
CLTV3DMoteur::~CLTV3DMoteur(void)
{
	clGlobalVar->kill();
	pTV->ReleaseAll();
	if (pTVScene) { delete(pTVScene); pTVScene = NULL; }
	if (pTVMaths) { delete(pTVMaths); pTVMaths = NULL; }
	if (pTVMaterialfactory) { delete(pTVMaterialfactory); pTVMaterialfactory = NULL; }
	if (pTVTexturefactory) { delete(pTVTexturefactory); pTVTexturefactory = NULL; }
	if (p2DImmediate) { delete(p2DImmediate); p2DImmediate = NULL; }
	// Destroy the engine last 
	if (pTV) { delete(pTV); pTV = NULL; }
}

void CLTV3DMoteur::DrawTV3DLine(cTV_3DVECTOR VecA, cTV_3DVECTOR VecB, int Color){
	p2DImmediate->Draw_Line3D(VecA.x, VecA.y, VecA.z, VecB.x, VecB.y, VecB.z, Color, -2);
}
void CLTV3DMoteur::DrawMeshAxis(CTVMesh* mesh, int iAxisLength){
	cTV_3DVECTOR VecPos;
	VecPos = mesh->GetPosition();
	p2DImmediate->Draw_Line3D(VecPos.x, VecPos.y, VecPos.z, VecPos.x + iAxisLength, VecPos.y, VecPos.z, WHITE_RGBA, -2);  // X Axis
	p2DImmediate->Draw_Line3D(VecPos.x, VecPos.y, VecPos.z, VecPos.x, VecPos.y + iAxisLength, VecPos.z, GREEN_RGBA, -2);	// Y Axis
	p2DImmediate->Draw_Line3D(VecPos.x, VecPos.y, VecPos.z, VecPos.x, VecPos.y, VecPos.z + iAxisLength, RED_RGBA, -2);	// Z Axis 
}
void CLTV3DMoteur::DrawTV3DAxis(void){
	p2DImmediate->Draw_Line3D(0, 0, 0, 10000, 0, 0, WHITE_RGBA, -2);  // X Axis   
	p2DImmediate->Draw_Line3D(0, 0, 0, 0, 10000, 0, GREEN_RGBA, -2); // Y Axis   
	p2DImmediate->Draw_Line3D(0, 0, 0, 0, 0, 10000, RED_RGBA, -2);  // Z Axis  
}
void CLTV3DMoteur::DrawWireframe(CTVMesh* Mesh)
{
	int i, vec1, vec2, vec3, Group, color,NbrFaces;
	float Temp;
	sVERTEX sVertex[3];
	NbrFaces = Mesh->GetTriangleCount();
	for (i = 0; i < NbrFaces * 3; i = i + 3)
	{
		Mesh->GetTriangleInfo(i / 3, &vec1, &vec2, &vec3, &Group);
		Mesh->GetVertex(vec1, &sVertex[0].x, &sVertex[0].y, &sVertex[0].z, &Temp, &Temp, &Temp, &Temp, &Temp, &Temp, &Temp, &color);
		Mesh->GetVertex(vec2, &sVertex[1].x, &sVertex[1].y, &sVertex[1].z, &Temp, &Temp, &Temp, &Temp, &Temp, &Temp, &Temp, &color);
		Mesh->GetVertex(vec3, &sVertex[2].x, &sVertex[2].y, &sVertex[2].z, &Temp, &Temp, &Temp, &Temp, &Temp, &Temp, &Temp, &color);
		p2DImmediate->Draw_Line3D(sVertex[0].x, sVertex[0].y, sVertex[0].z, sVertex[1].x, sVertex[1].y, sVertex[1].z, WHITE_RGBA);
		p2DImmediate->Draw_Line3D(sVertex[1].x, sVertex[1].y, sVertex[1].z, sVertex[2].x, sVertex[2].y, sVertex[2].z, WHITE_RGBA);
		p2DImmediate->Draw_Line3D(sVertex[2].x, sVertex[2].y, sVertex[2].z, sVertex[0].x, sVertex[0].y, sVertex[0].z, WHITE_RGBA);
	}
}
void CLTV3DMoteur::DrawSpline(vecPosition3D Spline, int RGBAColor){
	bool bFirst = true;
	cTV_3DVECTOR PrevPoint;
	for each(cTV_3DVECTOR vec in Spline)
	{
		if (bFirst)
		{
			PrevPoint = vec;
			bFirst = false;
		}
		p2DImmediate->Draw_Line3D(PrevPoint.x, PrevPoint.y, PrevPoint.z, vec.x, vec.y, vec.z, RGBAColor);
		PrevPoint = vec;
	}
}
void CLTV3DMoteur::SetMeshOpacity(CTVMesh* Mesh, float fOpacity)
{
	int iMat = Mesh->GetMaterial(-1);
	pTVMaterialfactory->SetOpacity(iMat, fOpacity);
	Mesh->SetMaterial(iMat, -1);
}

vecPosition3D CLTV3DMoteur::CreateTrianglePile(int NbrEtage, cTV_3DVECTOR pos, float BoxSize)
{
	int iEtage, iLigne;
	vecPosition3D TrianglePos;
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

int RandomDice(int Size) // Lancer de Dés evenly distributed.
{
	static int const max = RAND_MAX / Size * Size;
	int r = rand();
	while (r >= max) { r = rand(); }
	return r % Size + 1;
}
void DevConsoleCreate(){

	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	int consoleHandleR, consoleHandleW;
	long stdioHandle;
	FILE *fptr;

	AllocConsole();
	std::wstring strW = L"Dev Console";
	SetConsoleTitle((LPCWSTR)strW.c_str());

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

}
vector<cTV_3DVECTOR> XmlSplineRead(string filename)
{
	vector<cTV_3DVECTOR> Spline;
	cTV_3DVECTOR vec;

	TiXmlDocument doc(filename.c_str());
	if (!doc.LoadFile()){
		cout << "erreur lors du chargement" << endl;
		cout << "error #" << doc.ErrorId() << " : " << doc.ErrorDesc() << endl;
	}
	TiXmlHandle hdl(&doc); //  il est préférable d'utiliser des handles pour se déplacer dans notre arbre DOM
	TiXmlElement *elem = hdl.FirstChildElement().FirstChildElement().Element();
	if (!elem){
		cout << "le noeud à atteindre n'existe pas" << endl;
	}
	while (elem){
		vec.x = atof(elem->Attribute("x"));
		vec.y = atof(elem->Attribute("y"));
		vec.z = atof(elem->Attribute("z"));
		Spline.push_back(vec);
		elem = elem->NextSiblingElement(); // iteration 
	}
	return Spline;
}
void XmlSplineAddNode(cTV_3DVECTOR vec, string filename)
{
	TiXmlDocument doc(filename.c_str());
	if (!doc.LoadFile()){
		cout << "erreur lors du chargement" << endl;
		cout << "error #" << doc.ErrorId() << " : " << doc.ErrorDesc() << endl;
		return;
	}
	TiXmlHandle hdl(&doc); //  il est préférable d'utiliser des handles pour se déplacer dans notre arbre DOM
	TiXmlElement *elem = hdl.FirstChildElement().FirstChildElement().Element();
	if (!elem){
		cout << "le noeud à atteindre n'existe pas" << endl;
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


using namespace System;
//using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace System::Xml;
using namespace System::IO;

// convert_system_string
void MarshalString(String ^ s, string& os) {
	using namespace Runtime::InteropServices;
	const char* chars =
		(const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
	os = chars;
	Marshal::FreeHGlobal(IntPtr((void*)chars));
}
void MarshalString(String ^ s, wstring& os) {
	using namespace Runtime::InteropServices;
	const wchar_t* chars =
		(const wchar_t*)(Marshal::StringToHGlobalUni(s)).ToPointer();
	os = chars;
	Marshal::FreeHGlobal(IntPtr((void*)chars));
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