#pragma once
#include "DynamicMesh.h"

namespace GraphysX
{
	class Bullet : public CLDynamicMesh
	{
	public:
		Bullet(NewtonWorld* world, STEntityParam param, cTV_3DVECTOR dir, int lifetime);
		~Bullet();

		void update() override;

		void ForceNTorque() override;
		//void Transform();

		int getBulletID() { return bulletID; }


		int Lifetime = 0;
		float fActualSpeed = 0;
		cTV_3DVECTOR direction;

		static int getNbrBullets() { return NbrBullets; }
	private:

		CLMeshPrimitive* bulletMesh = nullptr;
		static int NbrBullets;
		bool shoot = true;
		int bulletID = 0;
	};

}