#pragma once
#include "Entity3D.h"
#include "Scene.h"

namespace GraphysX
{
	class GRAPHYSX_API CLSpline3D :
		public CLEntity3D
	{
	public:
		CLSpline3D(CLScene* scene, Spline spline);
		virtual ~CLSpline3D();
		virtual void update();

		virtual void setVisible(bool b);
		virtual void addNode(cTV_3DVECTOR pos);
		virtual void setSpline(Spline spline);
		virtual Spline getSpline();
		virtual void clear();

	private:
		CLScene* clScene;
		Spline m_spline;
		bool isVisible = true;
	};

 }