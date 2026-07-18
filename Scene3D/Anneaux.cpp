#include "stdafx.h"
#include "Anneaux.h"
#include <GraphysX\Tools.h>
//#include "tiny_obj_loader.h"
#include <GraphysX\ResourceManager.h>

//  TODO:  Les Anneaux devrait être des entity3D au meme titre que le reste....

	int CLAnneaux::iMaxRingCount = 0;
	int CLAnneaux::iRingCount = 0;

	CLAnneaux::CLAnneaux(GraphysX::IGraphysX* graphysX, float particleSize)
	{
		m_graphysX = graphysX;
		std::cout << "Anneaux - Creating Empty Ring system... " << endl;
		iMaxRingCount = 0;
		iRingCount = 0;

		//Create and setup our particle system.
		IDParticleTex = GraphysX::ResourceManager::getTexture("particle.dds");
		m_Particles = GraphysX::CLTV3D::pTVScene.CreateParticleSystem("RingParticles");
		IDEmit = m_Particles->CreateEmitter(cTV_EMITTER_BILLBOARD, 16);
		m_Particles->SetEmitterPower(IDEmit, 0.15f, 2);
		m_Particles->SetEmitterSpeed(IDEmit, 100);
		m_Particles->SetEmitterPosition(IDEmit, &Vector3(0, 0, 0));
		m_Particles->SetEmitterShape(IDEmit, cTV_EMITTERSHAPE_POINT);
		m_Particles->SetBillboard(IDEmit, IDParticleTex,float( 2 * particleSize),float( 2 * particleSize));// 0.25, 0.25);
		m_Particles->SetEmitterDirection(IDEmit, true, &Vector3(1, 1, 0), &Vector3(0.5f, 0.5f, 0));
		m_Particles->SetParticleDefaultColor(IDEmit, &TVColor(1, 0, 0, 1));
		//m_Particles->SetEmitterGravity(IDEmit, true, &Vector3(0, -9.8, 0));
		m_Particles->SetEmitterEnable(IDEmit, false);
		m_Particles->SetEmitterLooping(IDEmit, false);

		pplShader = GraphysX::CLTV3D::pTVScene.CreateShader("pplring");  ///   DEVRAIT ETRE une ressource gerer dans le resource manager!!!!!!!!!!!!!!
		pplShader->CreateFromEffectFile("..\\StockRoom\\shaders\\ppl.shade");
		pplShader->SetEffectParamVector3("LPos", &m_graphysX->pActiveScene->getDefaultLightPos());
		pplShader->SetEffectParamFloat("BumpAmount", 0.025f);

		ringSound = GraphysX::ResourceManager::getSound("coin.wav", false);
		
	}
	CLAnneaux::CLAnneaux(GraphysX::IGraphysX* graphysX, std::string inputfile, int iColor, float fSize)
	{
		m_graphysX = graphysX;
		std::cout << "Anneaux - Loading .OBJ file... " << endl;
		NewtonWorld* World = m_graphysX->pActiveScene->GetWorld();
		
		GraphysX::Spline spline = Tools::OBJtoSpline(inputfile);

		for each(cTV_3DVECTOR vec in spline)
		{
			VecAnneaux.push_back(m_graphysX->pActiveScene->addPrimSphere("Ring", vec, fSize));
			VecAnneaux.back()->mesh->SetColor(iColor);
		}

		iMaxRingCount = VecAnneaux.size();
		iRingCount = iMaxRingCount;

		//Create and setup our particle system.
		IDParticleTex = GraphysX::ResourceManager::getTexture("ParticleTex");
		m_Particles = GraphysX::CLTV3D::pTVScene.CreateParticleSystem("RingParticles");
		IDEmit = m_Particles->CreateEmitter(cTV_EMITTER_BILLBOARD, 16);
		m_Particles->SetEmitterPower(IDEmit, 0.15f, 2);
		m_Particles->SetEmitterSpeed(IDEmit, 100);
		m_Particles->SetEmitterPosition(IDEmit, &Vector3(0, 0, 0));
		m_Particles->SetEmitterShape(IDEmit, cTV_EMITTERSHAPE_POINT);
		m_Particles->SetBillboard(IDEmit, IDParticleTex, 2 * fSize, 2 * fSize);// 0.25, 0.25);
		m_Particles->SetEmitterDirection(IDEmit, true, &Vector3(1, 1, 0), &Vector3(0.5f, 0.5f, 0));
		m_Particles->SetParticleDefaultColor(IDEmit, &TVColor(1, 0, 0, 1));
		//m_Particles->SetEmitterGravity(IDEmit, true, &Vector3(0, -9.8, 0));
		m_Particles->SetEmitterEnable(IDEmit, false);
		m_Particles->SetEmitterLooping(IDEmit, false);

		ringSound = GraphysX::ResourceManager::getSound("coin.wav", false);
	}
	CLAnneaux::~CLAnneaux(void)
	{
		VecAnneaux.clear();
	}
	GraphysX::CLMeshPrimitive* CLAnneaux::addRing(cTV_3DVECTOR pos, int iColor, float fSize)
	{
		VecAnneaux.push_back(m_graphysX->pActiveScene->addPrimSphere("Ring", pos, fSize, "ZRing.png"));
		
		CTVMesh* tvmesh = VecAnneaux.back()->mesh;
		tvmesh->SetMaterial(GraphysX::ResourceManager::getMaterial(eMATERIAL::DEF_MATERIAL));

		int IDNormal = GraphysX::ResourceManager::getTexture("..\\StockRoom\\ball_Normal.png");
		pplShader->SetEffectParamTexture("Normal", IDNormal);
		tvmesh->SetShader(pplShader);
		tvmesh->SetLightingMode(cTV_LIGHTING_BUMPMAPPING_TANGENTSPACE);
			
		tvmesh->SetSphereMapping(true, 0);    //  TODO: still used?
		iMaxRingCount++;
		iRingCount++;
		return(VecAnneaux.back());
	}
	int CLAnneaux::CheckContact(GraphysX::CLMeshPrimitive* BallZ)  // TODO  :   voir si les 2 itérations Check contact et Check proximity ne devrait pas etre uni dans CLAnneaux update simplement...
	{
		for each(GraphysX::CLMeshPrimitive* Obj in VecAnneaux)
		{
			cTV_3DVECTOR pos = BallZ->mesh->GetPosition();
			if (Tools::IsPositionInsideMesh(pos, Obj->mesh))  // if the Ball is inside a ring
			{
				m_Particles->SetEmitterPosition(IDEmit, &Obj->mesh->GetPosition());
				m_Particles->SetEmitterEnable(IDEmit, true);
				m_Particles->ResetAll();
				m_graphysX->clDisplayer.addScore();
				//m_graphysX->clMeshFactory.DestroyMesh(Obj);
				//Obj->Mesh->Enable(false);  //  BUG: meme avec le destroymesh, l'anneau reste visible....
				VecAnneaux.erase(std::remove(VecAnneaux.begin(), VecAnneaux.end(), Obj), VecAnneaux.end());
				//clSceneStaff->clSound->PlaySound(2);
				
				iRingCount = VecAnneaux.size();
				cout << iRingCount;
				break;
			}
		}
		return (VecAnneaux.size());
	}
	GraphysX::CLMeshPrimitive* CLAnneaux::CheckProximity(cTV_3DVECTOR playerPos, cTV_3DVECTOR playerScale)
	{
		const float proxFactor = 0.5f;
		for each(GraphysX::CLMeshPrimitive* ring in VecAnneaux)
		{
			ring->mesh->RotateY(0.1f * GraphysX::CLTV3D::getTimeElapsed());  // TODO :  ne devrait pas etre ici....    mais veut éviter une autre iteration entre tous les ring...
			if (playerScale.x / proxFactor  >   GraphysX::CLTV3D::pTVMaths.TVVec3Distance(&playerPos, &ring->mesh->GetPosition()))  // if the Ball is close enough to a ring
			{
				return(ring);  // proximity contact detected
			}
		}
		return (nullptr);  // no proximity contact
	}
	void CLAnneaux::deleteRing(GraphysX::CLMeshPrimitive* ring)
	{
		m_Particles->SetEmitterPosition(IDEmit, &ring->mesh->GetPosition());
		m_Particles->SetEmitterEnable(IDEmit, true);
		m_Particles->ResetAll();
		m_graphysX->clDisplayer.addScore();
		GraphysX::CLSound::PlaySound(ringSound);

		auto it = std::find(VecAnneaux.begin(), VecAnneaux.end(), ring);
		if (it != VecAnneaux.end()) {
			std::swap(*it, VecAnneaux.back());
			VecAnneaux.pop_back();
		}
						
		iRingCount = VecAnneaux.size();
		cout << iRingCount;
	}
	void CLAnneaux::Update()
	{
		m_Particles->Update();
		m_Particles->Render();   //Render the particles.
	}

/*
CLAnneaux::CLAnneaux(IGraphysX* m_graphysX, std::string inputfile, int iColor, float fSize)
{
std::cout << "Anneaux - Loading .OBJ file... " << endl;
NewtonWorld* World = m_graphysX->pActiveScene->GetWorld();
vector<tinyobj::shape_t> shapes;
vector<tinyobj::material_t> materials;

string err;
bool ret = tinyobj::LoadObj(shapes, materials, err, inputfile.c_str());
if (!err.empty()) { cerr << err << endl; }// `err` may contain warning message.
if (!ret)  exit(1);
std::cout << "# of shapes    : " << shapes.size() << endl;
std::cout << "# of materials : " << materials.size() << endl;

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

	VecAnneaux.push_back(m_graphysX->clMeshFactory.CreatePhysicMeshSphere(NULL, cTV_3DVECTOR(shapes[i].VerticesPos[v].x, shapes[i].VerticesPos[v].y, shapes[i].VerticesPos[v].z),
		cTV_3DVECTOR(fSize, fSize, fSize), "Ring", true, 0, m_graphysX->clMeshFactory.iPhysicMaterials[(int)ePHYSMAT::WOOD], World, false));
	VecAnneaux.back()->Mesh->SetColor(iColor, true);
}
		}

		//Create and setup our particle system.
		IDParticleTex = GraphysX::CLTV3D::pTVTexturefactory.GetTextureByName("ParticleTex");
		m_Particles = GraphysX::CLTV3D::pTVScene.CreateParticleSystem("RingParticles");
		IDEmit = m_Particles->CreateEmitter(cTV_EMITTER_BILLBOARD, 16);
		m_Particles->SetEmitterPower(IDEmit, 0.15f, 2);
		m_Particles->SetEmitterSpeed(IDEmit, 100);
		m_Particles->SetEmitterPosition(IDEmit, &Vector3(0, 0, 0));
		m_Particles->SetEmitterShape(IDEmit, cTV_EMITTERSHAPE_POINT);
		m_Particles->SetBillboard(IDEmit, IDParticleTex, 2 * fSize, 2 * fSize);// 0.25, 0.25);
		m_Particles->SetEmitterDirection(IDEmit, true, &Vector3(1, 1, 0), &Vector3(0.5f, 0.5f, 0));
		m_Particles->SetParticleDefaultColor(IDEmit, &TVColor(1, 0, 0, 1));
		//m_Particles->SetEmitterGravity(IDEmit, true, &Vector3(0, -9.8, 0));
		m_Particles->SetEmitterEnable(IDEmit, false);
		m_Particles->SetEmitterLooping(IDEmit, false);
	}
*/