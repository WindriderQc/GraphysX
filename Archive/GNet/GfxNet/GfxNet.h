// GfxNet.h
#pragma once
#include "..\Gfx\IGfx.h"

using namespace System;

namespace GfxNet {

	public ref class GfxTools
	{
	public:
		GfxTools();
		~GfxTools();

	protected:
		!GfxTools();

	public:
		System::String^ test();
		void window(HWND w);
		void render();
		float time();

	
	
private:	
		Gfx::IGfx* m_nativeClass = nullptr;

	
	};
}
