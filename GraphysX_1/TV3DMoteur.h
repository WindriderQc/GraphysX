#pragma once
#include "GraphysXdef.h"
#include "TV3D65\TVall.h" // TrueVision3D Header Files :  "CTVInternalObjects.h" -> DX headers needs to be include in order to use this.

namespace GraphysX
{
	class GRAPHYSX_API CLTV3D
	{
	private:
		 static HWND w3DWindowHandle;
		 static float fTimeElapsed;

	public:
		 static int init(HWND window, std::string AppPath);
		 static void kill();
		 static float getTimeElapsed();  // Actualisé à chaque loop dans renderEnd(). TODO : Whatchout au multi render par frame ( ex: water)
		 static void UpdateMeshToDestination(CTVMesh* Mesh, cTV_3DVECTOR dest, CTVLandscape* Land, float fHeightOffset, float fTimeElapsed); //TODO: pkoi pas dans l'interface comme le reste?
		 static void renderBegin(bool bZBufferOnly = false);
		 static void renderEnd();
		 static LPDIRECT3DDEVICE9 Get3DDevice(void);

		 static float getDistance3D(cTV_3DVECTOR &v1, cTV_3DVECTOR &v2);
		 static cTV_3DVECTOR getMiddlePoint(cTV_3DVECTOR* v1, cTV_3DVECTOR* v2);
		 static void DrawTV3DLine(cTV_3DVECTOR VecA, cTV_3DVECTOR VecB, int Color);
		 static void DrawMeshAxis(CTVMesh* mesh, int iAxisLength);
		 static void DrawTV3DAxis(void);
		 static void DrawWireframe(CTVMesh* Mesh);
		 static void DrawSpline(Spline spline, int RGBAColor);
		 static void ScreenShot(char* filename, cCONST_TV_IMAGEFORMAT format);
		 static void GetUserInput(void);
		
		 static CTVEngine pTV;
		 static CTVScene pTVScene;
		 static CTVMathLibrary pTVMaths;
		 static CTVTextureFactory pTVTexturefactory;
		 static CTVMaterialFactory pTVMaterialfactory;
		 static CTVScreen2DImmediate pTV2DImmediate;
		 static CTVInternalObjects pTVIntObj;  // Pointer on TV3D Internal objects
		 static CTVLightEngine pTVLightEngine;
		 static CTVGlobals pTVGlobals;
		 static CTVInputEngine pTVInput;
		 static CTVAtmosphere pTVAtmos;

		 //  TODO : a garder??   est-ce qu'on utilise tjrs le clavier et souris de TV3D?
		 static struct STKeyboard {
			cCONST_TV_KEY Pressedkey;
			cCONST_TV_KEY Releasedkey;
			byte KEY_PRESSED[256];
		} stKeyboard;
		 static struct STMouse
		{
			int PosX = NULL;
			int DeltaPosX = NULL;
			int PosY = NULL;
			int DeltaPosY = NULL;
			bool B1 = false;
			bool B2 = false;
			bool B3 = false;
			bool B4 = false;
			int Wheel = NULL;		 //  sera eventuellement pour le controle de la molette
		} stMouse;
		/* static struct STGamePad
		{
			long LeftX = NULL;
			long LeftY = NULL;
			long RightX = NULL;
			long RightY = NULL;
			bool b0 = false;
			bool b1 = false;
			bool b2 = false;
			bool b3 = false;
		} stGamePad; */
     	/*
			cTV_JOYSTATE joyState;  //  TODO: Pourrait etre mis public et remplacer la structure STGAMEPAD
			CTVGameControllers* pTVGamePads;
			bool bGamePadConnected;
		*/		
	};
}