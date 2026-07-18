#pragma once
#include "MeshPrimitive.h"


namespace Physics
{
		class ZoneTrigger
		{
		public:
			ZoneTrigger();
			~ZoneTrigger();
			int Init(GraphysX::CLMeshPrimitive* CLMeshPrimitive, bool* trg = nullptr); // , MeshController* ctrler);
			bool CheckZone(cTV_3DVECTOR pos);
			void setTrigger(bool stated);
			static int s_NbrTrigger;
			static std::vector<ZoneTrigger*> listTrigger;
		private:
			GraphysX::CLMeshPrimitive* zoneMesh = nullptr;
			cTV_3DVECTOR* min = &cTV_3DVECTOR(0, 0, 0);
			cTV_3DVECTOR* max = &cTV_3DVECTOR(0, 0, 0);
			bool internalTrg = 0;
			bool* pTrg = &internalTrg;  // Set to internal trigger in case an external trigger is not set
		};

		void CheckAllZoneTriggers(cTV_3DVECTOR pos);
		void ClearTriggers();
}