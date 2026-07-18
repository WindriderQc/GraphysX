#include "stdafx.h"
#include "TV3DMoteur.h"

#include "tiny_obj_loader.h"


namespace GraphysX
{ 
	CTVEngine CLTV3D::pTV;
	CTVScene CLTV3D::pTVScene;
	CTVMathLibrary CLTV3D::pTVMaths;
	CTVMaterialFactory CLTV3D::pTVMaterialfactory;
	CTVTextureFactory CLTV3D::pTVTexturefactory;
	CTVScreen2DImmediate CLTV3D::pTV2DImmediate;
	CTVLightEngine CLTV3D::pTVLightEngine;
	CTVGlobals CLTV3D::pTVGlobals;
	CTVInputEngine CLTV3D::pTVInput;
	CTVInternalObjects CLTV3D::pTVIntObj;
	CTVAtmosphere CLTV3D::pTVAtmos;

	STKeyboard CLTV3D::stKeyboard;
	STMouse CLTV3D::stMouse;
	STGamePad CLTV3D::stGamePad;

	int CLTV3D::init(HWND window, std::string AppPath)
	{
		if (w3DWindowHandle == nullptr)
		{
			w3DWindowHandle = window;
			std::cout << "TV3D Engine Initialization." << std::endl;
			//srand(UINT(time(NULL)));  // Called once so we can generate random number after... 
		
			// Create the pTV Interface first:
			pTV.SetSearchDirectory(AppPath.c_str());
			pTV.SetDebugMode(true, true); // Set the debug file/options. Do this before the 3D init so it can log any errors found during init.
			
			std::string debugPath = AppPath + "\\debugfile.txt";
			pTV.SetDebugFile(debugPath.c_str());
			pTV.EnableProfiler(true, true); // silent profiler only shows in debug file
			pTV.AllowMultithreading(true);
			pTV.Init3DWindowed(w3DWindowHandle, true); // Init the engine
			pTV.SetAntialiasing(true, cTV_MULTISAMPLE_2_SAMPLES);
			pTV.GetViewport()->SetAutoResize(true);// engine will analyse the window size and update the internal viewport when needed
			pTV.SetAngleSystem(cTV_ANGLE_DEGREE);
			pTV.ShowWinCursor(true);
			pTV.SetWatermarkParameters(cTV_WATERMARK_BOTTOMRIGHT, 0.6f); // Watermark maximum possible transparency is 0.6

			pTV.DisplayFPS(true, 0); // 0 = White text


			pTVInput.Initialize(true, true);
			for (int i = 0; i<256; i++)
			{
				stKeyboard.KEY_PRESSED[i] = NULL;
			}
			stMouse.B1 = NULL;
			stMouse.B2 = NULL;
			stMouse.DeltaPosX = NULL;
			stMouse.DeltaPosY = NULL;
			/*
			stGamePad.b0 = NULL;
			stGamePad.b1 = NULL;
			stGamePad.b2 = NULL;
			stGamePad.b3 = NULL;
			stGamePad.LeftX = NULL;
			stGamePad.LeftY = NULL;
			stGamePad.RightX = NULL;
			stGamePad.RightY = NULL;
			bGamePadConnected = false;
			pTVGamePads = new CTVGameControllers();
			if (pTVGamePads->Count() != NULL)
			bGamePadConnected = true;
			if(bGamePadConnected)
			{
			bool test = pTVGamePads->Item(0)->Initialize();
			test = 0;
			test = pTVGamePads->Item(0)->IsConnected();
			std::string sTest = pTVGamePads->Item(0)->GetProductName();
			}else{
			int i = 0;
			MSGBOX("GamePad not found","Input Init");
			}
			*/

			pTV.AddToLog("TV3DMoteur init completed.");
			pTV.SetVSync(false);  // drastique drop du FPS lorsque mis a true!!!
				
			return 1;
		} 
		std::cout << "CLTV3D already initialized." << std::endl;
		return -1;
	}
	LPDIRECT3DDEVICE9 CLTV3D::Get3DDevice(void)
	{
		return(pTVIntObj.GetDevice3D()); 
	}
	void CLTV3D::kill(void)
	{
		pTV.ReleaseAll();
	}
	void CLTV3D::GetUserInput(void)
	{
		////////////////////////////////////////
		//  Mouse Section  //
		////////////////////////////////////////
		pTVInput.GetAbsMouseState(&stMouse.PosX, &stMouse.PosY, &stMouse.B1, &stMouse.B2, &stMouse.B3, &stMouse.B4, &stMouse.Wheel);//A implementer : ctrl de la molette
		pTVInput.GetMouseState(&stMouse.DeltaPosX, &stMouse.DeltaPosY); 

		////////////////////////////////////////
		//  Keyboard Section  //
		////////////////////////////////////////
		//  non-repeating commands (commands that should not continuously fire)
		stKeyboard.Pressedkey = cTV_FORCE_DWORD;  // Reset les flags.
		stKeyboard.Releasedkey = cTV_FORCE_DWORD;
		int BUFFER_COUNT = 0;
		cTV_KEYDATA KEY_BUFFER[256];
		pTVInput.GetKeyBuffer(KEY_BUFFER, &BUFFER_COUNT);
		
		for (int i = 0; i < BUFFER_COUNT; i++)  
		{
			// Set Pressed Key
			if (KEY_BUFFER[i].Pressed != 0)
			{
				stKeyboard.Pressedkey = KEY_BUFFER[i].Key;
			}
			// Set released Key
			if (KEY_BUFFER[i].Released != 0)
			{
				stKeyboard.Releasedkey = KEY_BUFFER[i].Key;
			}
		}
		// repeating commands  (commands that should continuously fire)
		pTVInput.GetKeyPressedArray(stKeyboard.KEY_PRESSED);  



		////////////////////////////////////////
		//  GamePad Section  //
		////////////////////////////////////////
		/*
		//Side not for the left and right analog sticks: All values are 5000 greater or less than. This is the analogs range of motion
		//from 0 to 5000 and 0 to -5000. You can make it so if you have like 2500 if you want different speeds depending on how far you
		//have the stick pushed but that’s up to you to decide.
		//Side not for the D-Pad or Directional Pad all values are 9000 and to do the next direction it would be 9000 + 9000. For any diagonal
		//it would be 1/2. As you can see above in the code.
		//D-Pad Values up = 0 down = 18000 left = 27000 right = 9000 up right = 4500 right down 13500 down left 22500 up left = 31500 no press = -1

		if(bGamePadConnected)
		{
		// Game pad 1 (PSX/PC DUal Stick 12 button paddles)
		pTVGamePads->Item(0)->Poll();
		joyState = pTVGamePads->Item(0)->GetControllerState();

		if (pTVGamePads->Item(0)->GetButtonState(0)) // this is 0 - 11 for button index numbers
		stGamePad.b0 = 1;
		else
		stGamePad.b0 = 0;
		if (pTVGamePads->Item(0)->GetButtonState(1)) // this is 0 - 11 for button index numbers
		stGamePad.b1 = 1;
		else
		stGamePad.b1 = 0;

		if (pTVGamePads->Item(0)->GetButtonState(2)) // this is 0 - 11 for button index numbers
		stGamePad.b2 = 1;
		else
		stGamePad.b2 = 0;

		if (pTVGamePads->Item(0)->GetButtonState(3)) // this is 0 - 11 for button index numbers
		stGamePad.b3 = 1;
		else
		stGamePad.b3 = 0;

		// test avec 4500/5500 pour éviter de la fluctuation selon la calibration de la manette ou mouvement trop petit
		// Left Analog Stick  // Y - > Up = 0, Down = 10000
		//                    // X - > Left=0, Right = 10000
		if((4800 < joyState.X) && (joyState.X  < 5200))
		stGamePad.LeftX = NULL;
		else
		stGamePad.LeftX = (joyState.X -5000) / 5 ; // -5000 et divise par 50 pour avoir de -100 à 100 dans stGamePad,

		if((4800 < joyState.Y) && (joyState.Y < 5200))
		stGamePad.LeftY = NULL;
		else
		stGamePad.LeftY = (joyState.Y -5000) / 5 * -1; // * -1 pour inversé le sens

		//Right Analog Stick  // RotationZ - > Left = 0, Right = 1000
		//                    // Z - > Up, Down
		if((4000 < joyState.rotationz) && (joyState.rotationz  < 6000))
		stGamePad.RightY = NULL;
		else
		stGamePad.RightY = (joyState.rotationz -5000) / 50* -1; // * -1 pour inversé le sens
		if((4000 < joyState.Z) && (joyState.Z < 6000))
		stGamePad.RightX = NULL;
		else
		stGamePad.RightX = (joyState.Z -5000) / 50;
		}
		*/
	}
	void CLTV3D::renderBegin(bool bZBufferOnly)  {
		pTV.Clear(bZBufferOnly);
	}
	void CLTV3D::renderEnd()  {
		pTV.RenderToScreen();
	}
	void CLTV3D::ScreenShot(char* filename, cCONST_TV_IMAGEFORMAT format){
		pTV.Screenshot(filename, format);
	}
	void CLTV3D::DrawTV3DLine(cTV_3DVECTOR VecA, cTV_3DVECTOR VecB, int Color){
		pTV2DImmediate.Draw_Line3D(VecA.x, VecA.y, VecA.z, VecB.x, VecB.y, VecB.z, Color, -2);
	}
	void CLTV3D::DrawMeshAxis(CTVMesh* mesh, int iAxisLength){
		cTV_3DVECTOR VecPos;
		VecPos = mesh->GetPosition();
		pTV2DImmediate.Draw_Line3D(VecPos.x, VecPos.y, VecPos.z, VecPos.x + iAxisLength, VecPos.y, VecPos.z, RGBA_WHITE, -2);  // X Axis
		pTV2DImmediate.Draw_Line3D(VecPos.x, VecPos.y, VecPos.z, VecPos.x, VecPos.y + iAxisLength, VecPos.z, RGBA_GREEN, -2);	// Y Axis
		pTV2DImmediate.Draw_Line3D(VecPos.x, VecPos.y, VecPos.z, VecPos.x, VecPos.y, VecPos.z + iAxisLength, RGBA_RED, -2);	// Z Axis 
	}
	void CLTV3D::DrawTV3DAxis(void){
		pTV2DImmediate.Draw_Line3D(0, 0, 0, 10000, 0, 0, RGBA_WHITE, -2);  // X Axis   
		pTV2DImmediate.Draw_Line3D(0, 0, 0, 0, 10000, 0, RGBA_GREEN, -2); // Y Axis   
		pTV2DImmediate.Draw_Line3D(0, 0, 0, 0, 0, 10000, RGBA_RED, -2);  // Z Axis  
	}
	void CLTV3D::DrawWireframe(CTVMesh* Mesh)
	{
		int i, vec1, vec2, vec3, Group, color,NbrFaces;
		float Temp;
		sVERTEX sVertex[3];
		NbrFaces = Mesh->GetTriangleCount();
		for (i = 0; i < NbrFaces * 3; i = i + 3)
		{
			Mesh->GetTriangleInfo(i / 3, &vec1, &vec2, &vec3, &Group);
			Mesh->GetVertex(vec1, &sVertex[0].x, &sVertex[0].y, &sVertex[0].z, &Temp, &Temp, &Temp, &Temp, &Temp, &Temp, &Temp, &color);
			Mesh->GetVertex(vec2, &sVertex[1].x, &sVertex[1].y, &sVertex[1].z, &Temp, &Temp, &Temp, &Temp, &Temp, &Temp, &Temp, &color);
			Mesh->GetVertex(vec3, &sVertex[2].x, &sVertex[2].y, &sVertex[2].z, &Temp, &Temp, &Temp, &Temp, &Temp, &Temp, &Temp, &color);
			pTV2DImmediate.Draw_Line3D(sVertex[0].x, sVertex[0].y, sVertex[0].z, sVertex[1].x, sVertex[1].y, sVertex[1].z, RGBA_WHITE);
			pTV2DImmediate.Draw_Line3D(sVertex[1].x, sVertex[1].y, sVertex[1].z, sVertex[2].x, sVertex[2].y, sVertex[2].z, RGBA_WHITE);
			pTV2DImmediate.Draw_Line3D(sVertex[2].x, sVertex[2].y, sVertex[2].z, sVertex[0].x, sVertex[0].y, sVertex[0].z, RGBA_WHITE);
		}
	}
	void CLTV3D::DrawSpline(Spline spline, int RGBAColor){
		cTV_3DVECTOR PrevPoint = spline[0];
		for each(cTV_3DVECTOR vec in spline)
		{
			pTV2DImmediate.Draw_Line3D(PrevPoint.x, PrevPoint.y, PrevPoint.z, vec.x, vec.y, vec.z, RGBAColor);
			PrevPoint = vec;
		}
	}
	Spline CLTV3D::CreateTrianglePile(int NbrEtage, cTV_3DVECTOR pos, float BoxSize)
	{
		int iEtage, iLigne;
		Spline TrianglePos;
		cTV_3DVECTOR LineStart;
		//  determiné le nombre de boite dans la pile par NbrÉtage^2 je crois.  et établir un vecteur de CTV_3DVECTOR avec ce 
		//  nombre pour contruire la liste finale de position de la boite.
		// int iNbrBox = (NbrEtage*(NbrEtage + 1)) / 2;
		for (iEtage = 0; iEtage < NbrEtage; iEtage++){
			LineStart = cTV_3DVECTOR(pos.x - (BoxSize / 2)*iEtage, pos.y + (NbrEtage - iEtage)*BoxSize, pos.z);
			for (iLigne = 0; iLigne <= iEtage; iLigne++){
				TrianglePos.push_back(cTV_3DVECTOR(LineStart.x + (iLigne)*BoxSize, LineStart.y, LineStart.z));
			}
		}
		return(TrianglePos);
	}
	void CLTV3D::UpdateMeshToDestination(CTVMesh* Mesh, cTV_3DVECTOR dest, CTVLandscape* Land, float fHeightOffset, float fTimeElapsed)
	{
		// We need a position for the Mesh
		cTV_3DVECTOR MeshPosition = Mesh->GetPosition();
		// We need a direction for the Mesh to make it point in the right direction.
		cTV_3DVECTOR MeshDirection;
		// We need angles for the Mesh
		float MeshAngleY;

		pTVMaths.TVVec3Subtract(&MeshDirection, &dest, &MeshPosition); //MeshDestination
		pTVMaths.TVVec3Normalize(&MeshDirection, &MeshDirection);
		// Update the Mesh's angle
		if (MeshDirection.z > 0)
			MeshAngleY = pTVMaths.Rad2Deg(atan(MeshDirection.x / MeshDirection.z));
		else
			MeshAngleY = pTVMaths.Rad2Deg(atan(MeshDirection.x / MeshDirection.z)) + 180;

		// Set the  mesh rotation
		Mesh->SetRotation(0.0f, MeshAngleY, 0.0f);

		// Check if Mesh has reached destination, if not, update the Mesh
		// position by adding a scale of the vector destination.
		if ((pTVMaths.GetDistance3D(MeshPosition.x, 0, MeshPosition.z, dest.x, 0, dest.z)) > 2)
		{
			// Update all the Mesh's position
			cTV_3DVECTOR dV2;
			pTVMaths.TVVec3Scale(&dV2, &MeshDirection, (fTimeElapsed * 0.1f));
			pTVMaths.TVVec3Add(&MeshPosition, &MeshPosition, &dV2);
			MeshPosition.y = Land->GetHeight(MeshPosition.x, MeshPosition.z) + fHeightOffset;
			// Update the  mesh position
			Mesh->SetPosition(MeshPosition.x, MeshPosition.y, MeshPosition.z);
		}
	};
	float CLTV3D::getDistance3D(cTV_3DVECTOR &v1, cTV_3DVECTOR &v2)
	{
		return(CLTV3D::pTVMaths.GetDistance3D(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z));
	}
	cTV_3DVECTOR CLTV3D::getMiddlePoint(cTV_3DVECTOR* v1, cTV_3DVECTOR* v2)
	{
		cTV_3DVECTOR pos;
		float fLength = CLTV3D::pTVMaths.TVVec3Distance(v1, v2);
		CLTV3D::pTVMaths.TVVec3Subtract(&pos, v2, v1);
		CLTV3D::pTVMaths.TVVec3MultiplyAdd(&pos, &pos, 0.5f, v1);
		return pos;
	}
	Spline CLTV3D::OBJtoSpline(std::string filePath)
	{
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		std::string err;
		bool ret = tinyobj::LoadObj(shapes, materials, err, filePath.c_str());
		if (!err.empty()) { std::cerr << err << std::endl; }// `err` may contain warning message.
		if (!ret)  exit(1);
		std::cout << "# of shapes    : " << shapes.size() << std::endl;
		std::cout << "# of materials : " << materials.size() << std::endl;

		int nbrVertices;
		Spline retSpline;
		for (size_t i = 0; i < shapes.size(); i++) {
			// SECTION INUTILE - ARRANGER POUR UTILISER mesh.positions!!!
			nbrVertices = shapes[i].VerticesPos.size();
			for (size_t v = 0; v < nbrVertices; v++) {
				retSpline.push_back(Vector3(shapes[i].VerticesPos[v].x, shapes[i].VerticesPos[v].y, shapes[i].VerticesPos[v].z));
			}
		}
		return(retSpline);
	}
}