#include "stdafx.h"
#include "ParticleEffect.h"
#include <TV3D65\CTVParticleSystem.h>

namespace ParticleEffects
{
		int ParticleEffect::s_NbrSystem = 0;
		std::vector<ParticleEffect*> ParticleEffect::vPartSys;

		ParticleEffect::ParticleEffect(CTVParticleSystem* partsys, bool Enable, bool Looping)
		{
			PartSys = partsys;
			IDEmit = PartSys->CreateEmitter();
			PartSys->SetEmitterShape(IDEmit);
			PartSys->SetEmitterEnable(IDEmit, Enable);
			PartSys->SetEmitterLooping(IDEmit, Looping);
			s_NbrSystem++;
		}

		ParticleEffect::ParticleEffect(CTVParticleSystem* partsys, cCONST_TV_EMITTERTYPE typeEmitter, cCONST_TV_EMITTERSHAPE shapeEmitter, cTV_3DVECTOR* pos, int MaxSprite, bool Enable)
		{
			PartSys = partsys;
			IDEmit = PartSys->CreateEmitter(typeEmitter, MaxSprite);
			PartSys->SetEmitterShape(IDEmit, shapeEmitter);
			PartSys->SetEmitterEnable(IDEmit, Enable);
			PartSys->SetEmitterPosition(IDEmit, pos);
			s_NbrSystem++;
		}
		ParticleEffect::~ParticleEffect()
		{
		}
		int ParticleEffect::Init(int IDPartTexture, float emitterSpeed, float emitPower, float partLifetime, cTV_COLOR* defaultColor, bool Looping)
		{
			PartSys->SetEmitterLooping(IDEmit, Looping);
			PartSys->SetEmitterPower(IDEmit, emitPower, partLifetime);
			PartSys->SetEmitterSpeed(IDEmit, emitterSpeed);

			PartSys->SetBillboard(IDEmit, IDPartTexture, 2, 2);
			PartSys->SetEmitterDirection(IDEmit, true, &Vector3(1, 1, 0), &Vector3(0.25f, 0.5f, 0));
			PartSys->SetParticleDefaultColor(IDEmit, defaultColor);// &TVColor(1, 0, 1, 1));
			//m_Particles->SetEmitterGravity(IDEmit, true, &Vector3(0, -9.8, 0));
			vPartSys.push_back(this);
			return true;
		}
		void ParticleEffect::kill()
		{
			PartSys->Destroy();
		}

		void RenderAllParticleSystems()
		{
			for each(ParticleEffect* obj in ParticleEffect::vPartSys)
				obj->PartSys->Render();
		}
		void ClearParticleSystems()
		{
			for each(ParticleEffect* obj in ParticleEffect::vPartSys)
			{
				obj->PartSys->Enable(false);
			}
		}
}