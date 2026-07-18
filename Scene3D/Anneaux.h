#pragma once
#include <GraphysX\Itv.h>


using namespace std;
using namespace System::Collections::Generic;


class CLAnneaux
{
private:
	GraphysX::Itv* m_tv;
	unsigned int nbrAnneaux;
	int iPtsValue = 1;
		
	vector<GraphysX::CLMeshPrimitive*> VecAnneaux;

	CTVParticleSystem  *m_Particles; //Particle functionality.
	int   IDParticleTex;             //This is an ID for our particle texture
	int   IDEmit;					 //This is an ID for our particle emitter .
	CTVShader* pplShader;
	static int iMaxRingCount;
	static int iRingCount;

	FMOD::Sound* ringSound;
public:
	CLAnneaux(GraphysX::IGraphysX* m_graphysX, float particleSize);
	CLAnneaux(GraphysX::IGraphysX* m_graphysX, std::string inputfile, int iColor = RGBA_YELLOW, float fSize = 1);
	~CLAnneaux(void);
	GraphysX::CLMeshPrimitive* addRing(cTV_3DVECTOR pos, int iColor = RGBA_YELLOW, float fSize = 1);
	int CheckContact(GraphysX::CLMeshPrimitive* BallZ);   // In Zone detection
	GraphysX::CLMeshPrimitive* CheckProximity(cTV_3DVECTOR playerPos, cTV_3DVECTOR playerScale); // proximity detection - faster process, less precise
	void Update();
	static int getScore() { return iMaxRingCount - iRingCount; }
	static int getMaxRing() { return iMaxRingCount; }
	void deleteRing(GraphysX::CLMeshPrimitive* ring);

	bool isGameOver = false;
};
