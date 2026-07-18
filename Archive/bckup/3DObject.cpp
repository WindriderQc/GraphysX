#include "StdAfx.h"
#include "3DObject.h"

void ApplyForceAndTorqueCallback(const NewtonBody* body, dFloat timestep, int threadIndex)
{
	// Force and Torque callback (called everytime a modification occurs on a body)
	float masse; 
	cTV_3DVECTOR inertie; 
	cTV_3DVECTOR force; 

	// On récupère en premier lieu la masse ainsi que l'inertie
	NewtonBodyGetMassMatrix (body, &masse, &inertie.x, &inertie.y, &inertie.z);
	force.x = 0.0f;
	force.y = -masse * GRAVITY * SCALE; 
	force.z = 0.0f;
	NewtonBodyAddForce (body, &force.x); // On ajoute la force au corps

	CL3DObject *cl3DObject = (CL3DObject*)NewtonBodyGetUserData(body);
	if (cl3DObject == NULL)
	{
		char* meshname = cl3DObject->Mesh->GetMeshName(); //  RAJOUTER LE NOM DU MESH DANS LE MESSAGE...  maudite conversion de string...
		MessageBox(NULL, (LPCTSTR)"CL3DObject == NULL, un object est surement creer sans SetUserData" , NULL, NULL);
		PostQuitMessage(0);//  envoyer le msg qui ferme l'application  
	}
}
void ApplyTransformationCallback(const NewtonBody* body, const dFloat* matrix, int threadIndex) 
{
	// transformation callback (called after the body has been simulated)
	NewtonMatrix NewtonMX;
	NewtonBodyGetMatrix(body, &NewtonMX.fMatrix [0][0]);
	CL3DObject* cl3DObject = (CL3DObject*)NewtonBodyGetUserData(body);
	cl3DObject->Mesh->SetMatrix(MX); 
	//delete(MX); MX = NULL;
}

CL3DObject::CL3DObject(int iTYPE, char* name, char* PathToMesh, int Texture, cTV_3DVECTOR Pos, cTV_3DVECTOR Scale, bool bEnable, float Masse, int iPHYSIC, int iNewtonMat, NewtonWorld* world) 
	: iActualFrame(0), iAnimNbrFrame(-1), bAnimState(true), Name(name), fMasse(Masse), World(world), Collision(NULL)
	 
{
	// Bad DESIGN - DOIT ETRE AILLEUR
	bIsOnElevator = false;
	bIsFinishline = false;

	fTime = 0;
	
	clTV3D = CLTV3DMoteur::getInstance();
	//  Create and set Mesh
	Mesh = clTV3D->pTVScene->CreateMeshBuilder(name);
	switch(iTYPE)
		{
			case SPHERE:	
				Mesh->CreateSphere(Scale.x,24,24); 
				Mesh->SetPosition(Pos.x, Pos.y, Pos.z);
				break;
			case CUBE: 
				Mesh->CreateBox(Scale.x, Scale.y, Scale.z);
				Mesh->SetPosition(Pos.x, Pos.y, Pos.z);
				break;
			case BILLBOARD:
				Mesh = clTV3D->pTVScene->CreateBillboard(Texture, Pos.x, Pos.y, Pos.z, Scale.x, Scale.y, name, false);
				break;
			case FLOOR:
				Mesh = clTV3D->pTVScene->CreateMeshBuilder();
				Mesh->AddFloor(Texture, -(Scale.x / 2), -(Scale.y / 2), Scale.x / 2, Scale.y / 2, Pos.y);
				break;
			case CUSTOM:
				bool bResult = Mesh->LoadTVM(PathToMesh,true,true);
				if(bResult == false)
					MessageBoxA(NULL, "The 3DObject TVM model hasn't been found...", "Warning", MB_ICONEXCLAMATION|MB_SETFOREGROUND);
				//Mesh->SetScale(Scale.x, Scale.y, Scale.z); //fait crasher un bout de la physics
				Mesh->SetPosition(Pos.x, Pos.y, Pos.z);
				break;	

				/* case TEXT:
					XMesh = clTV3D->pTVScene->CreateMeshBuilder();
					//XMesh->LoadXFile("media\\skmech.X");
					XMesh->Create3DText("TEST", "Times New Roman", 32, 0.2f);
					XMesh->SetLightingMode(cTV_LIGHTING_MANAGED);
					XMesh->SetColor(BLUE_RGBA);
				


				Function render()
				Dim sngScale As Single

				sngScale = GetDistance3D(Player.pos.X, Player.pos.Y, Player.pos.Z, sX, sY, sZ)
				If sngScale > 2.86 Then sngScale = 2.86
				TextMesh.ScaleMesh sngScale, sngScale * 0.7, sngScale

				TextMesh.LookAtPoint Player.pos

				TextMesh.render
				End Function

				Function DestroyMe()
				Set TextMesh = Nothing
				End Function

				Public Sub SetMaterial(ar As Single, ag As Single, ab As Single, da As Single, er As Single, eg As Single, eb As Single)
				MatFactory.CreateMaterial MeshID
				MatFactory.SetAmbient GetMat(MeshID), ar, ag, ab, 0
				MatFactory.SetDiffuse GetMat(MeshID), 0, 0, 0, da
				MatFactory.SetEmissive GetMat(MeshID), er, eg, eb, 0
				MatFactory.SetSpecular GetMat(MeshID), 0, 0, 0, 0
				MatFactory.SetPower GetMat(MeshID), 0
				End Sub

				Public Sub LookAtPlayer()
				Dim CollisionResult As TVCollisionResult
				Dim Destination As D3DVECTOR
				Dim Direction As D3DVECTOR
				Dim AngleY As Single

				Direction = VNormalize(VSubtract(Player.pos, TextMesh.GetPosition))

				If Direction.Z > 0 Then
				AngleY = Rad2Deg(Atn(Direction.X / Direction.Z))
				Else
				AngleY = Rad2Deg(Atn(Direction.X / Direction.Z)) + 180
				End If

				TextMesh.SetRotation 0, AngleY, 0
				End Sub

				*/
	}
	Mesh->SetLightingMode(cTV_LIGHTING_MANAGED);
	Mesh->SetShadowCast(true, true);
	if (bEnable == false)  Mesh->Enable(false);
	if (Texture && iTYPE != BILLBOARD)	Mesh->SetTexture(Texture, -1);
	if (!Mesh->GetMaterial(0))  
		Mesh->SetMaterial(clTV3D->iStdMaterial, -1);
	else // Correctif pour model de Blender...DOIT ETRE TEMPORAIRE ET CORRIGÉ, l'exporter de TVM semble ne pas récuprer les info de material du 3ds
		Mesh->SetMaterial(clTV3D->iStdMaterial, -1);
	


	if (iPHYSIC){
		if(iTYPE== SPHERE)	Collision = NewtonCreateSphere(World, Scale.x, 0, NULL);	
		if(iTYPE == CUBE)   Collision = NewtonCreateBox(World, Scale.x, Scale.y, Scale.z, 0, NULL);
		if(iTYPE == CUSTOM) BuildPhysicsMesh(Mesh); //Faire de koi ici pour changer le type de collision( faire un regroupement de primaire au lieu du custom sinon ca plante
													 // ex: avoir un PhysicBody custom a loader tout comme on load le mesh custom
	}
	
	if (iPHYSIC == PHYSICALL){
		pbody = NewtonCreateDynamicBody(World, Collision, Mesh->GetMatrix().m[0]);// (float*)//NewtonMX); // crash si on scale le mesh avant, a cause de la gestion des 3 dernier float de la matrix
		// On calcul l'inertie du corps, en passant par une petite formule
		cTV_3DVECTOR inertie;
		inertie.x = 0.7f * fMasse * (Scale.y * Scale.y + Scale.z * Scale.z) / 12;
		inertie.y = 0.7f * fMasse * (Scale.x * Scale.x + Scale.z * Scale.z) / 12;
		inertie.z = 0.7f * fMasse * (Scale.x * Scale.x + Scale.y * Scale.y) / 12;
		// On définit ensuite la masse et l'inertie pour ce corps
		NewtonBodySetMassMatrix(pbody, fMasse, inertie.x, inertie.y, inertie.z);
		// On règle enfin le Callback, qui sera nécessaire pour que le corps bouge
		NewtonBodySetForceAndTorqueCallback(pbody, ApplyForceAndTorqueCallback);
		NewtonBodySetTransformCallback(pbody, ApplyTransformationCallback);
		NewtonBodySetMaterialGroupID(pbody, iNewtonMat);
		NewtonBodySetUserData(pbody, this);
	}		
	if (iPHYSIC == PHYSICSTATIC){
		pbody = NewtonCreateDynamicBody(World, Collision, Mesh->GetMatrix().m[0]); // crash si on scale le mesh avant, a cause de la gestion des 3 dernier float de la matrix
		NewtonBodySetMaterialGroupID(pbody, iNewtonMat);
	}
}
CL3DObject::~CL3DObject(void)
{
	//  ETRANGE....    CEST CALLER UNE FOIS AU LOADING DES OBJECT!!!!
	if(Collision) NewtonDestroyCollision(Collision);  Collision = NULL;
	if(pbody) NewtonDestroyBody(pbody);   pbody = NULL;
	if(Mesh)  clTV3D->pTVScene->DestroyMesh(Mesh);  Mesh = NULL;
}
void CL3DObject::SetPosition(cTV_3DVECTOR Pos, cTV_3DVECTOR Rot)
{
		Mesh->SetPosition(Pos.x,Pos.y,Pos.z);
		Mesh->SetRotation(Rot.x, Rot.y, Rot.z);
		if (pbody)	NewtonBodySetMatrix(pbody, Mesh->GetMatrix().m[0]);
}
void CL3DObject::Render(void)
{
	if(Mesh->IsEnabled())
		Mesh->Render();
}
void CL3DObject::BuildPhysicsMesh(CTVMesh* mesh)
{
//  Build the physics mesh and colision tree	
float Temp;
int i, vec1, vec2, vec3, Group, color, NbrFaces, NbrVertex;

NbrFaces = Mesh->GetTriangleCount();
NbrVertex = Mesh->GetVertexCount();
sVERTEX *sVertex = NULL;
sVertex = new sVERTEX[NbrFaces * 3];
for (int j = 0; j<NbrFaces * 3; j++)
{
	sVertex[j].x = 0;    // Initialize all elements to zero.
	sVertex[j].y = 0;
	sVertex[j].z = 0;
}
Collision = NewtonCreateTreeCollision(World, 666);
NewtonTreeCollisionBeginBuild(Collision);
for (i = 0; i < NbrFaces * 3; i = i + 3)
{
	Mesh->GetTriangleInfo(i / 3, &vec1, &vec2, &vec3, &Group);
	Mesh->GetVertex(vec1, &sVertex[i].x, &sVertex[i].y, &sVertex[i].z, &Temp, &Temp, &Temp, &Temp, &Temp, &Temp, &Temp, &color);
	Mesh->GetVertex(vec2, &sVertex[i + 1].x, &sVertex[i + 1].y, &sVertex[i + 1].z, &Temp, &Temp, &Temp, &Temp, &Temp, &Temp, &Temp, &color);
	Mesh->GetVertex(vec3, &sVertex[i + 2].x, &sVertex[i + 2].y, &sVertex[i + 2].z, &Temp, &Temp, &Temp, &Temp, &Temp, &Temp, &Temp, &color);
	NewtonTreeCollisionAddFace(Collision, 3, &sVertex[i].x, sizeof(sVERTEX), 0);
}
// Finalize the collision tree build
NewtonTreeCollisionEndBuild(Collision, 1);
delete[] sVertex;  // When done, free memory pointed to by sVertex.
sVertex = NULL;     // Clear a to prevent using invalid memory reference.
}
void DebugShowGeometryCollision (void* userData, int vertexCount, const dFloat* faceVertec, int id)
{
CTVScreen2DImmediate* p2D = new CTVScreen2DImmediate();
int i = vertexCount - 1;
dVector p0 (faceVertec[i * 3 + 0], faceVertec[i * 3 + 1], faceVertec[i * 3 + 2]);
	for (int i = 0; i < vertexCount; i ++) 
	{
		dVector p1 (faceVertec[i * 3 + 0], faceVertec[i * 3 + 1], faceVertec[i * 3 + 2]);
		p2D->Draw_Line3D(p0.m_x, p0.m_y, p0.m_z, p1.m_x, p1.m_y, p1.m_z, WHITE_RGBA, -2); 
		p0 = p1;
	}

	delete(p2D); p2D = NULL;
}
void CL3DObject::NewtonDebug(void)
{
	if(pbody != NULL)
	{
	dMatrix matrix;
	NewtonBodyGetMatrix(pbody, &matrix[0][0]);
	NewtonCollisionForEachPolygonDo (NewtonBodyGetCollision(pbody), &matrix[0][0], DebugShowGeometryCollision, NULL);
	}
}
//Animation
void CL3DObject::Translate(cTV_3DVECTOR Force, float SpeedFactor)
{
	cTV_3DVECTOR VecRotation = Mesh->GetRotation();
	cTV_3DVECTOR VecPosition = Mesh->GetPosition();

	if (bAnimState == 1)
	{
		if ((iAnimNbrFrame == -1) || (iAnimNbrFrame >= iActualFrame))  // Si l'anim est infini ou pas terminée
		{
			VecPosition.x = VecPosition.x + (Force.x * SpeedFactor);
			VecPosition.y = VecPosition.y + (Force.y * SpeedFactor);
			VecPosition.z = VecPosition.z + (Force.z * SpeedFactor);

			SetPosition(VecPosition, VecRotation);
			iActualFrame++;
		}
		else{
			bAnimState = 0;
		}
	}
}
void CL3DObject::Rotate(cTV_3DVECTOR Force, float SpeedFactor)
{
	cTV_3DVECTOR VecRotation = Mesh->GetRotation();
	cTV_3DVECTOR VecPosition = Mesh->GetPosition();

	if (bAnimState == 1)
	{
		if ((iAnimNbrFrame == -1) || (iAnimNbrFrame >= iActualFrame))  // Si l'anim est infini(-1) ou pas terminée
		{
			VecRotation.x = VecRotation.x + (Force.x * SpeedFactor);// Force est la rotation en degré pour chaque axes
			VecRotation.y = VecRotation.y + (Force.y * SpeedFactor);
			VecRotation.z = VecRotation.z + (Force.z * SpeedFactor);
			if (VecRotation.x >= 360)  VecRotation.x = 0;
			if (VecRotation.y >= 360)  VecRotation.y = 0;
			if (VecRotation.z >= 360) 	VecRotation.z = 0;

			SetPosition(VecPosition, VecRotation);
			iActualFrame++;
		}
		else{
			bAnimState = 0;
		}
	}
}
void CL3DObject::RotateAround(float SpeedFactor, cTV_3DVECTOR *Center, float fRadius)
{
	float xRot, yRot;
	cTV_3DVECTOR rotate;
	cTV_3DVECTOR VecRotation = Mesh->GetRotation();
	cTV_3DVECTOR VecPosition = Mesh->GetPosition();


	xRot = (float)( 2.0f * M_PI * fTime - M_PI * 0.5f);
	yRot = 0;

	fTime = fTime + SpeedFactor;

	if (bAnimState == 1)
	{
		if ((iAnimNbrFrame == -1) || (iAnimNbrFrame >= iActualFrame))  // Si l'anim est infini(-1) ou pas terminée
		{
			rotate = clTV3D->pTVMaths->MoveAroundPoint(Center, fRadius, xRot, yRot);
			SetPosition(rotate, VecRotation);
			iActualFrame++;
		}
		else{
			bAnimState = 0;
		}
	}
}

void CL3DObject::ChangeColor( int Intensity, float SpeedFactor)
{
	if (bAnimState == 1)
	{
		if ((iAnimNbrFrame == -1) || (iAnimNbrFrame >= iActualFrame))  // Si l'anim est infini(-1) ou pas terminée
		{
			// A Implenter....
			iActualFrame++;
		}
		else{
			bAnimState = 0;
		}
	}
}

