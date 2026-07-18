#pragma once
#include <GraphysX\Tools.h>
#include <GraphysX\IGraphysX.h>  //  TODO:   a surement pas besoin de GraphysX au complet
#include <GraphysX\Spline3D.h>

#define SAMPLING_FRAMES 100

public ref class LapChecker {
public:  ////   TODO:   THAT SHOULD BE OPTIMIZE i think....    send reference to vector should be faster?   et ptete creer une fonction init au lieu du construtor comme ca pas besoin de gcnew dans Gameplayscreen
	LapChecker(GraphysX::IGraphysX* graphysX, cTV_3DVECTOR FinishPos1, cTV_3DVECTOR FinishPos2, cTV_3DVECTOR HalfPos1, cTV_3DVECTOR HalfPos2){
		m_graphysX = graphysX;
		FinishMin = new cTV_3DVECTOR(0, 0, 0);
		FinishMax = new cTV_3DVECTOR(0, 0, 0);
		HalfMin = new cTV_3DVECTOR(0, 0, 0);
		HalfMax = new cTV_3DVECTOR(0, 0, 0);
		float fLength;
		cTV_3DVECTOR pos;

		//FinishZone
		fLength = GraphysX::CLTV3D::getDistance3D(FinishPos1, FinishPos2);
		pos = GraphysX::CLTV3D::getMiddlePoint(&FinishPos1, &FinishPos2);
		clFinishZone = m_graphysX->pActiveScene->addPrimCube("FinishZone", pos, { 0.1f, 10.0f, fLength });
		clFinishZone->stParam.enable = false;
		clFinishZone->mesh->Enable(false);
		clFinishZone->mesh->LookAtPoint(&FinishPos2);
		clFinishZone->mesh->GetBoundingBox(FinishMin, FinishMax);
		//HalfLapZone
		fLength = GraphysX::CLTV3D::getDistance3D(HalfPos1, HalfPos2);
		pos = GraphysX::CLTV3D::getMiddlePoint(&HalfPos1, &HalfPos2);
		clHalfLapZone = m_graphysX->pActiveScene->addPrimCube("HalfLapZone", pos, { 0.1f, 10.0f, fLength });
		clHalfLapZone->stParam.enable = false;
		clHalfLapZone->mesh->Enable(false);
		clHalfLapZone->mesh->LookAtPoint(&HalfPos2);
		clHalfLapZone->mesh->GetBoundingBox(HalfMin, HalfMax);

		clSplineGhost = m_graphysX->pActiveScene->add3DSpline(GraphysX::Spline(), true);
		clSplineGhost->setVisible(false);
		clSplineLastLap = m_graphysX->pActiveScene->add3DSpline(GraphysX::Spline(), true);

	};
	~LapChecker(){
		delete(FinishMin);
		delete(FinishMax);
		delete(HalfMin);
		delete(HalfMax);
	};
	bool IsLapCompleted(cTV_3DVECTOR BallPos)
	{
		static int iSampling = SAMPLING_FRAMES;
		iSampling--;   // TODO : jpense c'Est bad...  si plusieurs LapCXhecker....  le sampling fait augmenter a chaque nouveau checker
		if (!iSampling)
		{
			clSplineGhost->addNode(BallPos);
			iSampling = SAMPLING_FRAMES;
		}
		
		bool finish = false;
		if (bHalfLapDone){
			finish = Tools::IsPositionInsideBoundingBox(BallPos, *FinishMin, *FinishMax);
			if (finish) {
				bHalfLapDone = false;
				clSplineLastLap->setSpline(clSplineGhost->getSpline());
				clSplineGhost->clear();
			}
		}
		else bHalfLapDone = Tools::IsPositionInsideBoundingBox(BallPos, *HalfMin, *HalfMax);

		return(finish);
	}
	private:
		GraphysX::IGraphysX* m_graphysX;
		GraphysX::CLMeshPrimitive* clFinishZone;
		GraphysX::CLMeshPrimitive* clHalfLapZone;
		cTV_3DVECTOR* FinishMin, *FinishMax, *HalfMin, *HalfMax;
		cTV_3DVECTOR* FinishPos1, *FinishPos2, *HalfPos1, *HalfPos2;
		bool bHalfLapDone = false;

		GraphysX::CLSpline3D* clSplineGhost;
		GraphysX::CLSpline3D* clSplineLastLap;
	};
