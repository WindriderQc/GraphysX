#include "stdafx.h"
#include "Human.h"
#include "Tools.h"
#include "TV3DMoteur.h"
#include "MeshBillboard.h"

namespace GraphysX
{
	Human::Human(NewtonWorld* world, STEntityParam param, int tex) : CLDynamicMesh(world, param)
	{
		NewtonBodySetUserData(pbody, this);
		setRandomDirection();
		if (tex)
		{
			billboard = new CLMeshBillboard(STEntityParam(eObjet::BILLBOARD,  eGeometry::NON_PRIMITIVE,"", param.pos, { 2, 2, 1 }), tex);
			billboard->loadMesh();  //  TODO : est-ce nécessaire??  ou bien si le constructeur de DynamicMesh va caller la fonction override anyway??
		}
	}
	Human::~Human()
	{
		if(billboard) delete(billboard);
	}
	void Human::setRandomDirection()
	{
		direction = Vector3(Tools::random_float(-1.0f, 1.0f), 0, Tools::random_float(-1.0f, 1.0f));
		if (direction.Length() == 0) direction = Vector3(1.0f, 0, 0);
		CLTV3D::pTVMaths.TVVec3Normalize(&direction, &direction);
	}
	void Human::rotateDirection()
	{
		CLTV3D::pTVMaths.TVVec3Rotate(&direction, &direction, Tools::random_float(-40.0f, 40.0f), 0, 0);
	}
	void Human::update()
	{
		if (frameCount == 20)
		{
			rotateDirection();
			frameCount = 0;
		}
		else{
			frameCount++;
		}
		
		if (billboard){
			cTV_3DVECTOR p = mesh->GetPosition();
			billboard->mesh->SetPosition(p.x, p.y - stParam.scale.x/2, p.z);
		}
			
		//stParam.pos += direction * stParam.speed;
		//clPrim->mesh->SetPosition(position.x, position.y, position.z);
		//CLMeshPrimitive->SetPosNRot(position, Vector3(0, 0, 0));
	}
	void Human::ForceNTorque()
	{
		addGravity();
		addForce(CLTV3D::pTVMaths.VScale(&direction, stParam.speed));
	}
}