#pragma once
#include "m_3D.h"  // (m_Vector3)
#include "Wrapper3D.h" // (Engine3DWrapper)

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Xml::Serialization;
using namespace System::IO;                 // File System Methods


[XmlRoot("Object3D")]
public ref class Object3D
{
public:
	[XmlAttribute]
	int Type;
	[XmlAttribute]
	String^ Name;
	
	m_Vector3^ Pos;
	m_Vector3^ Scale;
	[XmlAttribute("Enabled")]
	bool bEnable;
	[XmlAttribute]
	float masse;
	[XmlAttribute]
	bool MeshControlled;
	[XmlAttribute("NewtonMat")]
	int iNewtonMat;
	String^ PathToMesh;
	String^ TextureName;

	Object3D() {
		Type = PHYSICCUBE;
		Name = "CubeTestPhys";
		PathToMesh = "";
		TextureName = "TwoWay";
		Pos = gcnew m_Vector3(0, 0, 0);
		Scale = gcnew m_Vector3(10, 10, 10);
		bEnable = true;
		masse = NULL;
		iNewtonMat = -1;
		MeshControlled = false;
	}
};
public ref class CScene3D
{
public:
	CScene3D(){ /// PKOI EST CALLÉ 2 FOIS??????????
		
		size_t Nbr = sizeof(eObjet_names) / sizeof(eObjet_names[0]);  // divise l'array totale par la longueur de 1 élément
		for (UINT i = 0; i < Nbr; i++)
		{
			String^ name = gcnew String(eObjet_names[i]);
			Actions = Actions + "  " + i +"=" + name;
			delete(name);
		}

	};
	String^ Actions;
	List<Object3D^>^ ObjList;
}; 

ref class CSceneBuilder
{
public:
	CSceneBuilder();
	~CSceneBuilder();
	
	CScene3D^ Scene3D;

	void Serialize(String^ Path, Object^ Obj, Engine3DWrapper^ Tools3D);
	void Deserialized(String^ Path);

	void BuildScene(Engine3DWrapper^ Tools3D, CScene3D^ Scene2Build);
	void ClearScene3D(Engine3DWrapper^ Tools3D);
	void Test();

};