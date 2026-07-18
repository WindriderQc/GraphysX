#pragma once
//#include <SDL\SDL.h>
//#include <SDL\SDL_syswm.h>
#ifdef GFX_API_EXPORT
	#define GFX_API __declspec(dllexport)
#else
	#define GFX_API __declspec(dllimport)
#endif

#include "TV3DMoteur.h"
#include "test.h"

namespace Gfx
{
	class GFX_API IGfx
		{
		public:
			 IGfx();
			 ~IGfx();
			// void init(std::string PathString, int iWinWidth = 1280, int iWinHeight = 720);
			
			//HWND createSDLwindow(int width = 1280, int height = 720);
			 float getTimeElapsed();
		private:
			//CLTV3D clTV3D;
			test t;

		};
}