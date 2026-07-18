#include "simulation.h"

// Constructeur
Simulation::Simulation(void) 

{
	clTV3D = new CLTV3DMoteur();
	clODEMoteur = new ODEMoteur();

	IsAtmelConnected = false;
}

// Destructeur
Simulation::~Simulation(void)
{
	delete(clODEMoteur);
	delete(clTV3D);
}

// Méthode d'animation du simulateur
void Simulation::animer(void)
{
/*
		int KeyPressed, i;
		KeyPressed = clTV3D->CheckKeyPressed(); 
		moteurODE->ReactToKeyPressed(KeyPressed);
		moteurODE->DoPhysics(KeyPressed);
		moteurODE->UpdatePhysics();
*/

		float TimeElapsed = clTV3D->GetTimeElapsed();
		clODEMoteur->update();
		GetGeomsPosNMatrixNMoveMeshes();
		//clTV3D->SetCamera();	
		clTV3D->Render(TimeElapsed);
		
		clTV3D->clInput->GetInputState();   //  a ameliorer pour etre comme getKBInput
		clTV3D->clInput->GetUserKBInput(TimeElapsed);
		// Test si le bouton 1 de a souris a été clicker is si oui, test sur quel 
	// objet et agit en consequence..
	if((clTV3D->clInput->stMouse.ButtonDown != NULL)  &&  clTV3D->clInput->stMouse.B1 != NULL)  // vérifier si je doit enlever la premiere condition
	{
		clTV3D->GetMeshClicked();
	}
		//InputMSG = clTV3D->InputAction();
	//	if(InputMSG != NULL)
	//	{
	//	PostMessage(WindowHWND, WM_COMMAND, InputMSG, NULL); 
	//	}
	
	/*
	if(clTV3D->bCEGUI_ACTIVE)
			clTV3D->PumpInputToCEGUI();
	*/
	
	/*	
		if(IsAtmelConnected)
		{
			clTV3D->IsAtmelConnected = true;
			clTV3D->ShowAtmelConnection(true);
		}
		else{
			clTV3D->IsAtmelConnected = false;
			clTV3D->ShowAtmelConnection(false);
			}

	*/	
}
/*void Simulation::GetCamPosNSpeedFactor(void)
{
	D3DCamPos = clTV3D->D3DCamPos;
	SpeedFactor = clTV3D->SpeedFactor;
	
}*/



// Méthode d'initialisation du simulateur
bool Simulation::initialiser(HWND WindowHandle, char *srchpath)
{
	clODEMoteur->initialiser();
	clTV3D->Init3D(WindowHandle, srchpath);
	WindowHWND = WindowHandle;
	return true;
} 


// Méthose qui récupère la position du Geom ODE et qui position le Mesh TV3D en conséquence
void Simulation::GetGeomsPosNMatrixNMoveMeshes(void) 
{ 
int i;
cTV_3DVECTOR GeomPos;
cTV_3DMATRIX GeomRotMat;


for(i=0 ;i<MAX_PHYSIC_MESH;i++) 
{  
	if((clODEMoteur->GeomTable[i] != NULL) && (clTV3D->MeshTable[i] != NULL))
	{
		if( ( (clODEMoteur->GeomTable[i] != NULL) && (clTV3D->MeshTable[i] == NULL) ) || ( (clODEMoteur->GeomTable[i] == NULL)&&(clTV3D->MeshTable[i] != NULL) )  )
			MessageBox(NULL, LPCWSTR("Mauvaise correspondance dans la table TV3D-ODE"), LPCWSTR("Error"), MB_OK);
		else
		{
		GeomPos.x = clODEMoteur->GetGeomPositionX(clODEMoteur->GeomTable[i]);           
		GeomPos.y = clODEMoteur->GetGeomPositionY(clODEMoteur->GeomTable[i]);  
		GeomPos.z = clODEMoteur->GetGeomPositionZ(clODEMoteur->GeomTable[i]); 
		
		GeomRotMat.m11 = clODEMoteur->GetGeomRotMatrix11(clODEMoteur->GeomTable[i]);
		GeomRotMat.m12 = clODEMoteur->GetGeomRotMatrix12(clODEMoteur->GeomTable[i]);
		GeomRotMat.m13 = clODEMoteur->GetGeomRotMatrix13(clODEMoteur->GeomTable[i]);
		GeomRotMat.m14 = clODEMoteur->GetGeomRotMatrix14(clODEMoteur->GeomTable[i]);
		GeomRotMat.m21 = clODEMoteur->GetGeomRotMatrix21(clODEMoteur->GeomTable[i]);
		GeomRotMat.m22 = clODEMoteur->GetGeomRotMatrix22(clODEMoteur->GeomTable[i]);
		GeomRotMat.m23 = clODEMoteur->GetGeomRotMatrix23(clODEMoteur->GeomTable[i]);
		GeomRotMat.m24 = clODEMoteur->GetGeomRotMatrix24(clODEMoteur->GeomTable[i]);
		GeomRotMat.m31 = clODEMoteur->GetGeomRotMatrix31(clODEMoteur->GeomTable[i]);
		GeomRotMat.m32 = clODEMoteur->GetGeomRotMatrix32(clODEMoteur->GeomTable[i]);
		GeomRotMat.m33 = clODEMoteur->GetGeomRotMatrix33(clODEMoteur->GeomTable[i]);
		GeomRotMat.m34 = clODEMoteur->GetGeomRotMatrix34(clODEMoteur->GeomTable[i]);
		GeomRotMat.m41 = clODEMoteur->GetGeomRotMatrix41(clODEMoteur->GeomTable[i]);
		GeomRotMat.m42 = clODEMoteur->GetGeomRotMatrix42(clODEMoteur->GeomTable[i]);
		GeomRotMat.m43 = clODEMoteur->GetGeomRotMatrix43(clODEMoteur->GeomTable[i]);
		GeomRotMat.m44 = clODEMoteur->GetGeomRotMatrix44(clODEMoteur->GeomTable[i]);
	
		clTV3D->pTVMath->TVMatrixTranspose(&GeomRotMat, &GeomRotMat);

		GeomRotMat.m41 =  GeomPos.x;
		GeomRotMat.m42 =  GeomPos.y;
		GeomRotMat.m43 =  GeomPos.z;
		GeomRotMat.m44 =  1;
	
		clTV3D->MeshTable[i]->SetRotationMatrix(&GeomRotMat);
		clTV3D->MeshTable[i]->SetPosition(GeomPos.x, GeomPos.y, GeomPos.z);   
		}
	}		
} //  fin du for
} 