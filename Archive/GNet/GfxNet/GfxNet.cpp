// This is the main DLL file.
#include "stdafx.h"
#include "GfxNet.h"

//#pragma comment(lib, "..\\Deps\\Lib\\Gfx")


namespace GfxNet
{
	GfxTools::GfxTools()
	{
		m_nativeClass = new Gfx::IGfx();
		
	}
	GfxTools::~GfxTools()
	{
		this->!GfxTools();
	}
	GfxTools::!GfxTools()
	{
		delete m_nativeClass;  
	}
	void GfxTools::window(HWND w)
	{
		Gfx::CLTV3D::init(w);
	

		//m_nativeClass->createSDLwindow();
	}
	System::String^ GfxTools::test()
	{
		return ("GfxNet ok");
	}
	float GfxTools::time()
	{
		//std::cout << "test time" << std::endl;
		return(m_nativeClass->getTimeElapsed());
	}
	void GfxTools::render()
	{
		Gfx::CLTV3D::renderBegin();
		Gfx::CLTV3D::pTVScene.RenderAll(true);
		Gfx::CLTV3D::renderEnd();
	}

}