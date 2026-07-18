#pragma once
#include "StdAfx.h"
#include "Tools.h"
#include "Scene.h"


class CLAnneaux : private CLSceneTools
{
private:
	int iColor;
	float fSize;
	int iPtsValue;
	NewtonWorld* World;
	vecPosition3D Spline;
	vector<CL3DObject*> VecAnneaux;

	CTVParticleSystem  *m_Particles; //Particle functionality.
	int   IDParticleTex;             //This is an ID for our particle texture.
	int   IDEmit;                    //This is an ID for our particle emitter - TV3D particle systems can have multiple emitters.

public:
	CLAnneaux(NewtonWorld* world, int IDParticleTex, int Color) : World(world), fSize(1), iPtsValue(1), iColor(Color)
	{
		Spline = XmlSplineRead("Media\\Anneaux.xml");	// { cTV_3DVECTOR(0, 0, 0), cTV_3DVECTOR(10, 0, 0) }; 		
		for each(cTV_3DVECTOR vec in Spline)
		{
			VecAnneaux.push_back(new CL3DObject(SPHERE, "Ring", NULL, NULL,vec,cTV_3DVECTOR(0.05f, 0.05f, 0.05f),true, 0, PHYSICSTATIC, NULL, World));
			VecAnneaux.back()->Mesh->SetColor(iColor, true);
		}

		//Create and setup our particle system.
		m_Particles = clTV3D->pTVScene->CreateParticleSystem("RingParticles");
		IDEmit = m_Particles->CreateEmitter(cTV_EMITTER_BILLBOARD, 16);
		m_Particles->SetEmitterPower(IDEmit, 0.15f, 1);
		m_Particles->SetEmitterSpeed(IDEmit, 100);
		m_Particles->SetEmitterPosition(IDEmit, &Vector3(0, 0, 0));
		m_Particles->SetEmitterShape(IDEmit, cTV_EMITTERSHAPE_POINT);
		m_Particles->SetBillboard(IDEmit, IDParticleTex, 0.25, 0.25);
		m_Particles->SetEmitterDirection(IDEmit, true, &Vector3(1, 1, 0), &Vector3(0.5f, 0.5f, 0));
		m_Particles->SetParticleDefaultColor(IDEmit, &TVColor(1, 0, 0, 1));
		//m_Particles->SetEmitterGravity(IDEmit, true, &Vector3(0, -9.8, 0));
		m_Particles->SetEmitterEnable(IDEmit,false);
		m_Particles->SetEmitterLooping(IDEmit,false);

	};
	~CLAnneaux(void)
	{
		for each(CL3DObject *Obj in VecAnneaux)
		{
			NewtonDestroyCollision(Obj->Collision); Obj->Collision = NULL;
			NewtonDestroyBody(Obj->pbody); Obj->pbody = NULL;
			delete(Obj); Obj = NULL;
		}
	};
	void Update(NewtonCollision* BallZCollision, cTV_3DMATRIX mat2)
	{
		const int nContacts = 2;
		float contacts[3 * nContacts];
		float normals[3 * nContacts];
		float penetration[ nContacts ];
		long long attributeA[nContacts];
		long long attributeB[nContacts];
		int threadIndex = 0;
		vector<CL3DObject*>::iterator it;
		cTV_3DMATRIX mat1;

		for (it = VecAnneaux.begin(); it != VecAnneaux.end();)
		{
			//Check for collision between collision meshes,
			// returns number of contact points
			mat1 = (*it)->Mesh->GetMatrix();
			int nHits = NewtonCollisionCollide(World, nContacts, (*it)->Collision,(float*)&mat1, 
												BallZCollision, (float*)&mat2, contacts, normals, 
												penetration, attributeA, attributeB, threadIndex);
			if (nHits > 0) //Collision detected if nHits > 0
			{
				cTV_3DVECTOR vec = (*it)->Mesh->GetPosition();
				m_Particles->SetEmitterPosition(IDEmit, &vec);
				m_Particles->SetEmitterEnable(IDEmit, true);
				m_Particles->ResetAll();
				clSound->PlaySound(2);
				clDisplayer->iScore = clDisplayer->iScore + 1;

				NewtonDestroyCollision((*it)->Collision);
				(*it)->Mesh->Enable(FALSE);  // mmm  ne devrait pas etre nécessaire puisque le mesh devrait etre supprimé par la prochaine ligne...  mais sans le disable, l'ombrage de l'anneau reste.... :S
				//DeleteMesh((*it)->Mesh);
				delete (*it);
				it = VecAnneaux.erase(it);
			}
			else {
				//m_Particles->SetEmitterEnable(IDEmit, false);
				it++;
			}
		}
		//Update the particle system.
		m_Particles->Update();
	};
	void Render()
	{
		for each(CL3DObject *Obj in VecAnneaux)
		{
			Obj->Render();
		}
		m_Particles->Render();   //Render the particles.
	};	
};