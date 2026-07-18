#include "stdafx.h"
#include "Bullet.h"
#include "ResourceManager.h"

namespace GraphysX
{
	int Bullet::NbrBullets = 0;


	Bullet::Bullet(NewtonWorld* world, STMeshParam param, cTV_3DVECTOR dir, int lifetime, float masse, ePHYSMAT physmat)
		: CLDynamicMesh(world, param, physmat, masse)
	{
		direction = dir;
		Lifetime = lifetime;
		fActualSpeed = param.speed;
		NbrBullets++;
		bulletID = NbrBullets;
	}
	Bullet::~Bullet() {
		NbrBullets--;
	}
	void Bullet::ForceNTorque()
	{
		addGravity();
		if (shoot)
		{
			cTV_3DVECTOR force;
			force.x = direction.x * fActualSpeed;   //  TODO: modifier pour que la vitesse diminue selon le temps....
			force.y = 0;
			force.z = direction.z * fActualSpeed;
			addForce(force);
			shoot = false;
		}
	}
	
	void Bullet::update()
	{
		//TODO: AJOUTER LE CALCUL de friction pour diminier progressivement la vitesse.

		Lifetime--;
		if (Lifetime == 0)  {
			kill();
			std::cout << "Bullet " << bulletID << " is dead" << std::endl;
		}
				
		//position += direction * fSpeed;
		//bulletMesh->Mesh->SetPosition(position.x, position.y, position.z);
	}
}

