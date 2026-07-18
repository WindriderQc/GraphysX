#pragma once
#include "Entity3D.h"
#include "GraphysXdef.h"
#include "TV3D65\TVall.h" 


namespace GraphysX
{
	class CLScene;

	class CLSpline3D :
		public CLEntity3D
	{
	public:
		GRAPHYSX_API CLSpline3D(CLScene* scene, Spline spline);
		GRAPHYSX_API virtual ~CLSpline3D();
		GRAPHYSX_API virtual void update();

		GRAPHYSX_API virtual void setVisible(bool b);
		GRAPHYSX_API virtual void addNode(cTV_3DVECTOR pos);
		GRAPHYSX_API virtual void setSpline(Spline spline);
		GRAPHYSX_API virtual Spline getSpline();
		GRAPHYSX_API virtual void clear();

	private:
		CLScene* clScene;
		Spline m_spline;
		bool isVisible = true;
	};

 }