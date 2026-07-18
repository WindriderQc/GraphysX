#include "stdafx.h"
#include "ZoneTrigger.h"
#include "MeshPrimitive.h"
#include "Tools.h"

namespace Physics
{
		int ZoneTrigger::s_NbrTrigger = 0;
		std::vector<ZoneTrigger*> ZoneTrigger::listTrigger;

		ZoneTrigger::ZoneTrigger()
		{
		}
		ZoneTrigger::~ZoneTrigger()
		{
		}

		int ZoneTrigger::Init(GraphysX::CLMeshPrimitive* clMeshPrimitive, bool* trg)
		{
			zoneMesh = clMeshPrimitive;
			if (trg) pTrg = trg;
			listTrigger.push_back(this);
			s_NbrTrigger++;
			return 1;
		}
		bool ZoneTrigger::CheckZone(cTV_3DVECTOR pos)
		{
			zoneMesh->mesh->GetBoundingBox(min, max);
			if (Tools::IsPositionInsideBoundingBox(pos, *min, *max))
			{
				*pTrg = true;
			}
			else{
				*pTrg = false;
			}
			return *pTrg;
		}
		void ZoneTrigger::setTrigger(bool state)
		{
			*pTrg = state;
		}

		void CheckAllZoneTriggers(cTV_3DVECTOR pos)
		{
			for each(ZoneTrigger* Trg in ZoneTrigger::listTrigger)
				Trg->CheckZone(pos);
		}
		void ClearTriggers()
		{
			for each(ZoneTrigger* Trg in ZoneTrigger::listTrigger)
				delete(Trg);
			ZoneTrigger::listTrigger.clear();
			ZoneTrigger::s_NbrTrigger = 0;
		}
}