#include "stdafx.h"
#include "SceneBuilder.h"
#include "Wrapper3D.h"


CSceneBuilder::CSceneBuilder()
{
	Scene3D = gcnew CScene3D();
	Scene3D->ObjList = gcnew List<Object3D^>();
}
CSceneBuilder::~CSceneBuilder()

{
}
void CSceneBuilder::Serialize(String^ Path, Object^ Obj, Engine3DWrapper^ Tools3D){
	
	// RE ECRIT LE HEADER DU XML
	Scene3D->Actions = "";
	size_t Nbr = sizeof(eObjet_names) / sizeof(eObjet_names[0]);  // divise l'array totale par la longueur de 1 élément
	for (UINT i = 0; i < Nbr; i++)
	{
		String^ name = gcnew String(eObjet_names[i]);
		Scene3D->Actions = Scene3D->Actions + "  " + i + "=" + name;
		delete(name);
	}

	// Update all Positions
	for each(Object3D^ Obj in Scene3D->ObjList){
		char cName[50] = { 0 };
		if (Obj->Name->Length < sizeof(cName)) // make sure it fits & allow space for null terminator
			sprintf(cName, "%s", Obj->Name);
		else{
			System::Windows::Forms::MessageBox::Show(L"SceneBuilder -Serializer: " + Obj->Name, L"Nom trop long (50 char max)",
				System::Windows::Forms::MessageBoxButtons::OK);
		}

		CLMesh* nativemesh = Tools3D->m_MeshFactory->Find3DObjectWithName(cName);
		Obj->Pos->x = nativemesh->Mesh->GetPosition().x;
		Obj->Pos->y = nativemesh->Mesh->GetPosition().y;
		Obj->Pos->z = nativemesh->Mesh->GetPosition().z;
	}

	// Write in XML
	StreamWriter^ xmlFile = File::CreateText(Path);
	XmlSerializer^ serializer = gcnew XmlSerializer(Obj->GetType());
	serializer->Serialize(xmlFile, Obj);
	xmlFile->Close();
}
void CSceneBuilder::Deserialized(String^ Path)
{
	XmlSerializer^ serializer = gcnew XmlSerializer(Scene3D->GetType());
	StreamReader^ reader = File::OpenText(Path);
	Scene3D = (CScene3D^)serializer->Deserialize(reader);
	reader->Close();
}
void CSceneBuilder::Test()
{
	Scene3D->ObjList->Add(gcnew Object3D());
}
void CSceneBuilder::BuildScene(Engine3DWrapper^ Tools3D, CScene3D^ Scene2Build )
{
	for each(Object3D^ Obj in Scene2Build->ObjList)
	{
		 
		int index = Scene2Build->ObjList->IndexOf(Obj); // Pour rajouter l'indez au nom mais...  
		//ca fera chier les recherche par nom...  devra controler que la creation ne fait jamais un doublon de nom!!

		char cName[50] = { 0 };
		if (Obj->Name->Length < sizeof(cName)) // make sure it fits & allow space for null terminator
			sprintf(cName, "%s", Obj->Name);
		else{
			System::Windows::Forms::MessageBox::Show(L"SceneBuilder - " + Obj->Name, L"Nom trop long (50 char max)",
				System::Windows::Forms::MessageBoxButtons::OK);
		}

		char cTexName[50] = { 0 };
		if (Obj->TextureName->Length < sizeof(cTexName)) // make sure it fits & allow space for null terminator
			sprintf(cTexName, "%s", Obj->TextureName);
		else{
			System::Windows::Forms::MessageBox::Show(L"SceneBuilder - " + Obj->TextureName, L"Texture Nom trop long (50 char max)",
				System::Windows::Forms::MessageBoxButtons::OK);
		}

		char cPathToMesh[100] = { 0 };
		if (Obj->PathToMesh->Length < sizeof(cPathToMesh)) // make sure it fits & allow space for null terminator
			sprintf(cPathToMesh, "%s", Obj->PathToMesh);
		else{
			System::Windows::Forms::MessageBox::Show(L"SceneBuilder - " + Obj->PathToMesh, L"PathToMesh trop long (100 char max)",
				System::Windows::Forms::MessageBoxButtons::OK);
		}

		if (Obj->iNewtonMat == -1)	
			Obj->iNewtonMat = Tools3D->m_Realisateur->GetActiveScene()->GetDefaultPhysMaterial();

		//   !!!!   remplacer clBallZScene par un ActiveScene venant de Area!!!!!!!


		switch(Obj->Type)
		{

		case CUBE:
			Tools3D->m_MeshFactory->CreateCube(Tools3D->m_TV3D->pTVTexturefactory->GetTextureByName(cTexName),
				cTV_3DVECTOR(Obj->Pos->x, Obj->Pos->y, Obj->Pos->z), cTV_3DVECTOR(Obj->Scale->x, Obj->Scale->y, Obj->Scale->z), cName, true);
			break;
		case SPHERE:
			Tools3D->m_MeshFactory->CreateSphere(Tools3D->m_TV3D->pTVTexturefactory->GetTextureByName(cTexName),
				cTV_3DVECTOR(Obj->Pos->x, Obj->Pos->y, Obj->Pos->z), cTV_3DVECTOR(Obj->Scale->x, Obj->Scale->y, Obj->Scale->z), cName, true);
			break;
		case CYLINDER:
			Tools3D->m_MeshFactory->CreateCylinder(Tools3D->m_TV3D->pTVTexturefactory->GetTextureByName(cTexName),
				cTV_3DVECTOR(Obj->Pos->x, Obj->Pos->y, Obj->Pos->z), cTV_3DVECTOR(Obj->Scale->x, Obj->Scale->y, Obj->Scale->z), cName, true);
			break;
		case CONE:
			Tools3D->m_MeshFactory->CreateCone(Tools3D->m_TV3D->pTVTexturefactory->GetTextureByName(cTexName),
				cTV_3DVECTOR(Obj->Pos->x, Obj->Pos->y, Obj->Pos->z), cTV_3DVECTOR(Obj->Scale->x, Obj->Scale->y, Obj->Scale->z), cName, true);
			break;
		case BILLBOARD:
			Tools3D->m_MeshFactory->CreateBillboard(Tools3D->m_TV3D->pTVTexturefactory->GetTextureByName(cTexName),
				cTV_3DVECTOR(Obj->Pos->x, Obj->Pos->y, Obj->Pos->z), cTV_3DVECTOR(Obj->Scale->x, Obj->Scale->y, Obj->Scale->z), cName, true);
			break;
		case FLOOR:
			Tools3D->m_MeshFactory->CreateFloor(Tools3D->m_TV3D->pTVTexturefactory->GetTextureByName(cTexName),
				cTV_3DVECTOR(Obj->Pos->x, Obj->Pos->y, Obj->Pos->z), cTV_3DVECTOR(Obj->Scale->x, Obj->Scale->y, Obj->Scale->z), cName, true);
			break;
		case CUSTOM:
			Tools3D->m_MeshFactory->CreateCustom(cPathToMesh, Tools3D->m_TV3D->pTVTexturefactory->GetTextureByName(cTexName),
				cTV_3DVECTOR(Obj->Pos->x, Obj->Pos->y, Obj->Pos->z), cTV_3DVECTOR(Obj->Scale->x, Obj->Scale->y, Obj->Scale->z), cName, true);
			break;
		case XMESH:
			Tools3D->m_MeshFactory->CreateXMesh(cPathToMesh,
				cTV_3DVECTOR(Obj->Pos->x, Obj->Pos->y, Obj->Pos->z), cTV_3DVECTOR(Obj->Scale->x, Obj->Scale->y, Obj->Scale->z), cName, true);
				break;
		case DUPLICATE:
			Tools3D->m_MeshFactory->CreateDuplicate(cPathToMesh, cTV_3DVECTOR(Obj->Pos->x, Obj->Pos->y, Obj->Pos->z),
										cTV_3DVECTOR(Obj->Scale->x, Obj->Scale->y, Obj->Scale->z), cName, true);
			break;
		case PHYSICCUBE:
			Tools3D->m_MeshFactory->CreatePhysicMeshCube(Tools3D->m_TV3D->pTVTexturefactory->GetTextureByName(cTexName),
				cTV_3DVECTOR(Obj->Pos->x, Obj->Pos->y, Obj->Pos->z), cTV_3DVECTOR(Obj->Scale->x, Obj->Scale->y, Obj->Scale->z), cName, true, 
				Obj->masse, Tools3D->m_Realisateur->GetActiveScene()->iPhysicMaterials[Obj->iNewtonMat], Tools3D->m_Realisateur->GetActiveScene()->GetWorld(), Obj->MeshControlled);
			break;
		case PHYSICSPHERE:
			Tools3D->m_MeshFactory->CreatePhysicMeshSphere(Tools3D->m_TV3D->pTVTexturefactory->GetTextureByName(cTexName),
				cTV_3DVECTOR(Obj->Pos->x, Obj->Pos->y, Obj->Pos->z), cTV_3DVECTOR(Obj->Scale->x, Obj->Scale->y, Obj->Scale->z), cName, true,
				Obj->masse, Tools3D->m_Realisateur->GetActiveScene()->iPhysicMaterials[Obj->iNewtonMat], Tools3D->m_Realisateur->GetActiveScene()->GetWorld(), Obj->MeshControlled);
			break;
		case PHYSICCYLINDER:
			Tools3D->m_MeshFactory->CreatePhysicMeshCylinder(Tools3D->m_TV3D->pTVTexturefactory->GetTextureByName(cTexName),
				cTV_3DVECTOR(Obj->Pos->x, Obj->Pos->y, Obj->Pos->z), cTV_3DVECTOR(Obj->Scale->x, Obj->Scale->y, Obj->Scale->z), cName, true,
				Obj->masse, Tools3D->m_Realisateur->GetActiveScene()->iPhysicMaterials[Obj->iNewtonMat], Tools3D->m_Realisateur->GetActiveScene()->GetWorld(), Obj->MeshControlled);
			break;
		case PHYSICCONE:
			Tools3D->m_MeshFactory->CreatePhysicMeshCone(Tools3D->m_TV3D->pTVTexturefactory->GetTextureByName(cTexName),
				cTV_3DVECTOR(Obj->Pos->x, Obj->Pos->y, Obj->Pos->z), cTV_3DVECTOR(Obj->Scale->x, Obj->Scale->y, Obj->Scale->z), cName, true,
				Obj->masse, Tools3D->m_Realisateur->GetActiveScene()->iPhysicMaterials[Obj->iNewtonMat], Tools3D->m_Realisateur->GetActiveScene()->GetWorld(), Obj->MeshControlled);
			break;
		case PHYSICCUSTOM:
			Tools3D->m_MeshFactory->CreatePhysicMeshCustom(cPathToMesh, Tools3D->m_TV3D->pTVTexturefactory->GetTextureByName(cTexName),
				cTV_3DVECTOR(Obj->Pos->x, Obj->Pos->y, Obj->Pos->z), cTV_3DVECTOR(Obj->Scale->x, Obj->Scale->y, Obj->Scale->z), cName, true,
				Obj->masse, Tools3D->m_Realisateur->GetActiveScene()->iPhysicMaterials[Obj->iNewtonMat], Tools3D->m_Realisateur->GetActiveScene()->GetWorld(), Obj->MeshControlled);
			break;
		case PHYSICXMESH:
			Tools3D->m_MeshFactory->CreatePhysicXMesh(cPathToMesh, 	
				cTV_3DVECTOR(Obj->Pos->x, Obj->Pos->y, Obj->Pos->z), cTV_3DVECTOR(Obj->Scale->x, Obj->Scale->y, Obj->Scale->z), cName, true,
				Obj->masse, Tools3D->m_Realisateur->GetActiveScene()->iPhysicMaterials[Obj->iNewtonMat], Tools3D->m_Realisateur->GetActiveScene()->GetWorld(), Obj->MeshControlled);
			break;
		case PHYSICXSTATIC:
			Tools3D->m_MeshFactory->CreatePhysicXStatic(cPathToMesh,
				cTV_3DVECTOR(Obj->Pos->x, Obj->Pos->y, Obj->Pos->z), cTV_3DVECTOR(Obj->Scale->x, Obj->Scale->y, Obj->Scale->z), cName, true,
				Obj->masse, Tools3D->m_Realisateur->GetActiveScene()->iPhysicMaterials[Obj->iNewtonMat], Tools3D->m_Realisateur->GetActiveScene()->GetWorld(), Obj->MeshControlled);
			break;
		default:
			System::Windows::Forms::MessageBox::Show(L"SceneBuilder - " + Obj->Type, L"Object Type non reconnu - Must add to BuildScene()",
				System::Windows::Forms::MessageBoxButtons::OK);
			break;
		}
	}
}
void CSceneBuilder::ClearScene3D(Engine3DWrapper^ Tools3D)
{
	for each(CLMesh* ObjMesh in Tools3D->m_MeshFactory->vecMesh){
		delete(ObjMesh); ObjMesh = NULL;
	}
	Tools3D->m_MeshFactory->vecMesh.clear();

	for each(Object3D^ Obj in Scene3D->ObjList){
		delete(Obj);
	}
	Scene3D->ObjList->Clear();
}