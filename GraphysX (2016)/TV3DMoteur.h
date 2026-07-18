#pragma once
#include "TV3D65\TVall.h" // TrueVision3D Header Files :  "CTVInternalObjects.h" -> DX headers needs to be include in order to use this.


namespace GraphysX
{
	struct STKeyboard
	{
		cCONST_TV_KEY Pressedkey;
		cCONST_TV_KEY Releasedkey;
		byte KEY_PRESSED[256];
	};
	struct STMouse
	{
		int PosX;
		int DeltaPosX;
		int PosY;
		int DeltaPosY;
		bool B1;
		bool B2;
		bool B3;
		bool B4;
		int Wheel;		 //  sera eventuellement pour le controle de la molette
	};
	struct STGamePad
	{
		long LeftX;
		long LeftY;
		long RightX;
		long RightY;
		bool b0;
		bool b1;
		bool b2;
		bool b3;
	};

	class CLTV3D
	{
	public:
		int init(HWND window, std::string AppPath);
		void kill();
		void UpdateMeshToDestination(CTVMesh* Mesh, cTV_3DVECTOR dest, CTVLandscape* Land, float fHeightOffset, float fTimeElapsed);
		
		GRAPHYSX_API static Spline CreateTrianglePile(int NbrEtage, cTV_3DVECTOR pos, float BoxSize);
		GRAPHYSX_API static Spline OBJtoSpline(std::string filePath);
		GRAPHYSX_API static float getDistance3D(cTV_3DVECTOR &v1, cTV_3DVECTOR &v2);
		GRAPHYSX_API static cTV_3DVECTOR getMiddlePoint(cTV_3DVECTOR* v1, cTV_3DVECTOR* v2);


		
		GRAPHYSX_API void renderBegin(bool bZBufferOnly = false);
		GRAPHYSX_API void renderEnd();
		GRAPHYSX_API static void DrawTV3DLine(cTV_3DVECTOR VecA, cTV_3DVECTOR VecB, int Color);
		GRAPHYSX_API static void DrawMeshAxis(CTVMesh* mesh, int iAxisLength);
		GRAPHYSX_API static void DrawTV3DAxis(void);
		GRAPHYSX_API static void DrawWireframe(CTVMesh* Mesh);
		GRAPHYSX_API static void DrawSpline(Spline spline, int RGBAColor);
		GRAPHYSX_API static void ScreenShot(char* filename, cCONST_TV_IMAGEFORMAT format);
		GRAPHYSX_API static LPDIRECT3DDEVICE9 Get3DDevice(void);
		GRAPHYSX_API static void GetUserInput(void);
		
		GRAPHYSX_API static CTVEngine pTV;
		GRAPHYSX_API static CTVScene pTVScene;
		GRAPHYSX_API static CTVMathLibrary pTVMaths;
		GRAPHYSX_API static CTVTextureFactory pTVTexturefactory;
		GRAPHYSX_API static CTVMaterialFactory pTVMaterialfactory;
		GRAPHYSX_API static CTVScreen2DImmediate pTV2DImmediate;
		GRAPHYSX_API static CTVInternalObjects pTVIntObj;  // Pointer on TV3D Internal objects
		GRAPHYSX_API static CTVLightEngine pTVLightEngine;
		GRAPHYSX_API static CTVGlobals pTVGlobals;
		GRAPHYSX_API static CTVInputEngine pTVInput;
		GRAPHYSX_API static CTVAtmosphere pTVAtmos;

		GRAPHYSX_API static STKeyboard stKeyboard;
		GRAPHYSX_API static STMouse stMouse;
		GRAPHYSX_API static STGamePad stGamePad;

	private:
			HWND w3DWindowHandle = nullptr;

			/*
			cTV_JOYSTATE joyState;  //  TODO: Pourrait etre mis public et remplacer la structure STGAMEPAD
			CTVGameControllers* pTVGamePads;
			bool bGamePadConnected;
			*/
	};
}