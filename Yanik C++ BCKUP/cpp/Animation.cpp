#include "StdAfx.h"
#include "Animation.h"


// Constructeur
CLAnimation::CLAnimation(int iDuration, bool bEtat)
{
pTVScene = new CTVScene();
iAnimNbrFrame = iDuration;
bState = bEtat;
iActualFrame = 0;
}

//Destructeur
CLAnimation::~CLAnimation(void)
{
delete(pTVScene); pTVScene = NULL;
}

void CLAnimation::Translate(CTVMesh* TV3DMesh, cTV_3DVECTOR Force, float SpeedFactor)
{
	VecRotation = TV3DMesh->GetRotation();
	VecPosition = TV3DMesh->GetPosition();

	if(bState == 1)
	{
		if((iAnimNbrFrame == -1) || (iAnimNbrFrame >= iActualFrame))  // Si l'anim est infini(-1) ou pas terminée
		{
		TV3DMesh->SetPosition(VecPosition.x + (Force.x * SpeedFactor), VecPosition.y + (Force.y * SpeedFactor), VecPosition.z + (Force.z * SpeedFactor));
		iActualFrame++;
		}else{
			  bState = 0;
			  }
	}
}

void CLAnimation::Rotate(CTVMesh* TV3DMesh, cTV_3DVECTOR Force, float SpeedFactor)
{
	VecRotation = TV3DMesh->GetRotation();
	VecPosition = TV3DMesh->GetPosition();

		if(bState == 1)
		{
			if((iAnimNbrFrame == -1) || (iAnimNbrFrame >= iActualFrame))  // Si l'anim est infini(-1) ou pas terminée
			{
				VecRotation.x = VecRotation.x + (Force.x * SpeedFactor);// Force est la rotation en degré pour chaque axes
				if(VecRotation.x >360)  VecRotation.x = 0;
				VecRotation.y = VecRotation.y + (Force.y * SpeedFactor); 
				if(VecRotation.y >360)  VecRotation.y = 0;
				VecRotation.z =	VecRotation.z + (Force.z * SpeedFactor);
				if(VecRotation.z >360) 	VecRotation.z = 0;
				
				TV3DMesh->SetRotation(VecRotation.x, VecRotation.y, VecRotation.z);  
			}else{
				 bState = 0;
				 }
		}
}		

void CLAnimation::ChangeColor(CTVMesh* TV3DMesh, int Intensity, float SpeedFactor)
{
		if(bState == 1)
		{
			if((iAnimNbrFrame == -1) || (iAnimNbrFrame >= iActualFrame))  // Si l'anim est infini(-1) ou pas terminée
			{
				
			}else{
				 bState = 0;
				 }
		}
}		

void CLAnimation::Stop(void)
{
bState = NULL;
}

void CLAnimation::Start(void)
{
bState = true;
}



///////////////////////////////////////////////
//  Animator
//  Banque de méthode qui interagisse avec la scene
///////////////////////////////////////////////////


// Constructeur
CLAnimator::CLAnimator(void)
{
pTVScene = new CTVScene();
pMath = new CTVMathLibrary();
}

//Destructeur
CLAnimator::~CLAnimator(void)
{
delete(pTVScene); pTVScene = NULL;
}


//  A MODIFIER CAR LA FUNCTION EST FAITE POUR MOVER DE KOI AU DESSUS DU SOL SEULEMENT
void CLAnimator::UpdateMeshToDestination(CTVMesh* Mesh, cTV_3DVECTOR dest, CTVLandscape* Land, float fTimeElapsed)
{
// We need a position for the Mesh
cTV_3DVECTOR MeshPosition = Mesh->GetPosition();
// We need a destination for the Mesh
cTV_3DVECTOR MeshDestination = dest;
// We need a direction for the Mesh to make it point in the right direction.
cTV_3DVECTOR MeshDirection; 
// We need angles for the Mesh
float MeshAngleY;

cTV_3DVECTOR dVector;
pMath->TVVec3Subtract( &dVector, &MeshDestination, &MeshPosition);
pMath->TVVec3Normalize( &dVector, &dVector);
MeshDirection = dVector;
// Update the Mesh's angle
if (MeshDirection.z > 0 ) 
	MeshAngleY = pMath->Rad2Deg(atan(MeshDirection.x / MeshDirection.z));
else
	MeshAngleY = pMath->Rad2Deg(atan(MeshDirection.x / MeshDirection.z)) + 180;
				
// Set the  mesh rotation
Mesh->SetRotation(0.0f, MeshAngleY, 0.0f);
        
		
// Check if Mesh has reached destination, if not, update the Mesh
// position by adding a scale of the vector destination.
if ((pMath->GetDistance3D(MeshPosition.x, 0, MeshPosition.z, MeshDestination.x, 0, MeshDestination.z)) > 2) 
  {
	// Update all the Mesh's position
	cTV_3DVECTOR dV2;
	pMath->TVVec3Scale( &dV2, &MeshDirection,(fTimeElapsed * 0.1f));
    pMath->TVVec3Add( &MeshPosition, &MeshPosition, &dV2);
	MeshPosition.y = Land->GetHeight(MeshPosition.x, MeshPosition.z) + 10;
	// Update the  mesh position
	Mesh->SetPosition(MeshPosition.x, MeshPosition.y, MeshPosition.z);
  }
		
}