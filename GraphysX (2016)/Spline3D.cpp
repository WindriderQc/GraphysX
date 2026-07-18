#include "stdafx.h"
#include "Spline3D.h"

namespace GraphysX
{
	CLSpline3D::CLSpline3D(CLScene* scene, Spline spline) : clScene(scene), m_spline(spline)
	{
		clScene->vEntity.push_back(this);
	}


	CLSpline3D::~CLSpline3D()
	{
	/*	auto it = std::find(clScene->vEntity.begin(), clScene->vEntity.end(), this);
		if (it != clScene->vEntity.end()) {
			std::swap(*it, clScene->vEntity.back());
			clScene->vEntity.pop_back();
		}*/
	}

	void CLSpline3D::setVisible(bool b)
	{
		isVisible = b;
	}
	void CLSpline3D::clear()
	{
		m_spline.clear();
	}
	void CLSpline3D::addNode(cTV_3DVECTOR pos)
	{
		m_spline.push_back(pos);
	}
	void CLSpline3D::setSpline(Spline spline)
	{
		m_spline = spline;
	}
	Spline CLSpline3D::getSpline()
	{
		return(m_spline);
	}
	void CLSpline3D::update()
	{
		bool first = true; 
		cTV_3DVECTOR prev;
		
		if (isVisible)
		{
			  
			for each (cTV_3DVECTOR vec in m_spline)
			{
				if (first)
				{
					prev =  m_spline[0];
					first = false;
				}
				clScene->vecLine3D.push_back(sLine3D(prev, vec, RGBA_YELLOW));
				prev = vec;
			}

		}
	}
}