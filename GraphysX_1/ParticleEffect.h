#pragma once

class CTVParticleSystem;

namespace ParticleEffects
{
	class ParticleEffect
		{
		public:
			ParticleEffect(CTVParticleSystem* partsys, bool Enable, bool Looping);
			ParticleEffect(CTVParticleSystem* partsys, cCONST_TV_EMITTERTYPE typeEmitter, cCONST_TV_EMITTERSHAPE shapeEmitter, cTV_3DVECTOR* pos, int MaxSprite, bool Enable);
			~ParticleEffect();
			int Init(int IDPartTexture, float emitterSpeed, float emitPower, float partLifetime, cTV_COLOR* defaultColor, bool Looping);
			void kill();
			static int s_NbrSystem;

			CTVParticleSystem* PartSys;
			static std::vector<ParticleEffect*> vPartSys;
		private:
			int IDEmit;

		};

		void RenderAllParticleSystems();
		void ClearParticleSystems();
}
