#pragma once
#include "StdAfx.h"
#include "Tools.h"
#include "Scene.h"
#include "tiny_obj_loader.h"

#include "HighScoreForm.h"


class CLAnneaux
{
private:
	unsigned int nbrAnneaux;
	int iColor;
	float fSize;
	int iPtsValue;
	bool bGameOver;
	NewtonWorld* World;
	vecPosition3D Spline;
	vector<CL3DObject*> VecAnneaux;
	CLSceneStaff* clSceneStaff;

	CTVParticleSystem  *m_Particles; //Particle functionality.
	int   IDParticleTex;             //This is an ID for our particle texture.
	int   IDEmit;  //This is an ID for our particle emitter - TV3D particle systems can have multiple emitters.

	
public:
	CLAnneaux(NewtonWorld* world, int IDParticleTex, int Color) : World(world), fSize(1), iPtsValue(1), iColor(Color), bGameOver(false)
	{
		clSceneStaff = new CLSceneStaff();
		string inputfile = "..\\BallZ2015\\Media\\Suzanne1.obj";
		vector<tinyobj::shape_t> shapes;
		vector<tinyobj::material_t> materials;

		string err;
		bool ret = tinyobj::LoadObj(shapes, materials, err, inputfile.c_str());
		if (!err.empty()) { cerr << err << endl; }// `err` may contain warning message.
		if (!ret) { exit(1);}

		cout << "# of shapes    : " << shapes.size() << endl;
		cout << "# of materials : " << materials.size() << endl;

		for (size_t i = 0; i < shapes.size(); i++) {
			printf("shape[%ld].name = %s\n", i, shapes[i].name.c_str());
			printf("Size of shape[%ld].indices: %ld\n", i, shapes[i].mesh.indices.size());
			printf("Size of shape[%ld].material_ids: %ld\n", i, shapes[i].mesh.material_ids.size());
			assert((shapes[i].mesh.indices.size() % 3) == 0);
			for (size_t f = 0; f < shapes[i].mesh.indices.size() / 3; f++) {
				printf("  idx[%ld] = %d, %d, %d. mat_id = %d\n", f, shapes[i].mesh.indices[3 * f + 0], shapes[i].mesh.indices[3 * f + 1], shapes[i].mesh.indices[3 * f + 2], shapes[i].mesh.material_ids[f]);
			}

			printf("shape[%ld].vertices: %ld\n", i, shapes[i].mesh.positions.size());
			assert((shapes[i].mesh.positions.size() % 3) == 0);
			for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
				printf("  v[%ld] = (%f, %f, %f)\n", v,
					shapes[i].mesh.positions[3 * v + 0],
					shapes[i].mesh.positions[3 * v + 1],
					shapes[i].mesh.positions[3 * v + 2]);
			}
			// SECTION INUTILE - ARRANGER POUR UTILISER mesh.positions!!!
			nbrAnneaux = shapes[i].VerticesPos.size();
			for (size_t v = 0; v < nbrAnneaux; v++) {
				printf("  v[%ld] = (%f, %f, %f)\n", v,
					shapes[i].VerticesPos[v].x,
					shapes[i].VerticesPos[v].y,
					shapes[i].VerticesPos[v].z);

				VecAnneaux.push_back(new CL3DObject(SPHERE, "Ring", NULL, NULL, 
					cTV_3DVECTOR(shapes[i].VerticesPos[v].x,shapes[i].VerticesPos[v].y,shapes[i].VerticesPos[v].z), 
					cTV_3DVECTOR(0.05f, 0.05f, 0.05f), true, 0, PHYSICSTATIC, NULL, World));
				VecAnneaux.back()->Mesh->SetColor(iColor, true);
			}
		}
		/*
		Spline = XmlSplineRead("Media\\Anneaux.xml");	// { cTV_3DVECTOR(0, 0, 0), cTV_3DVECTOR(10, 0, 0) }; 		
		for each(cTV_3DVECTOR vec in Spline)
		{
			VecAnneaux.push_back(new CL3DObject(SPHERE, "Ring", NULL, NULL,vec,cTV_3DVECTOR(0.05f, 0.05f, 0.05f),true, 0, PHYSICSTATIC, NULL, World));
			VecAnneaux.back()->Mesh->SetColor(iColor, true);
		}
		*/
		//Create and setup our particle system.
		m_Particles = clSceneStaff->clTV3D->pTVScene->CreateParticleSystem("RingParticles");
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
			//Check for collision between collision meshes, returns number of contact points
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
				clSceneStaff->clSound->PlaySound(2);
				clSceneStaff->clDisplayer->iScore++;
				NewtonDestroyCollision((*it)->Collision);
				delete (*it);
				it = VecAnneaux.erase(it);
				cout << VecAnneaux.size();
			}
			else {
				//m_Particles->SetEmitterEnable(IDEmit, false);
				it++;
			}
		}
		//Update the particle system.
		m_Particles->Update();
		
		if ((VecAnneaux.size() == 10) && (bGameOver == false))
		{
			cout << "VICTORY";
			bGameOver = true;
			BallZ2015::HighScoreForm^ ScoreSheet = gcnew BallZ2015::HighScoreForm(clSceneStaff->clDisplayer->iTime);
			ScoreSheet->Show();
		}
			
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