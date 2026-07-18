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
	pTVMaterialfactory->SetPower(iStdMaterial, 30);
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
	if (pTVScene) delete(pTVScene); pTVScene = NULL; 
	if (pTVMaths)  delete(pTVMaths); pTVMaths = NULL; 
	if (pTVMaterialfactory)  delete(pTVMaterialfactory); pTVMaterialfactory = NULL; 
	if (pTVTexturefactory)  delete(pTVTexturefactory); pTVTexturefactory = NULL; 
	if (p2DImmediate) delete(p2DImmediate); p2DImmediate = NULL; 
	// Destroy the engine last 
	pTV->ReleaseAll();
	if (pTV)  delete(pTV); pTV = NULL; 
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

