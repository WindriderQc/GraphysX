/*
## Simple Shadow Mapping.
## Written By Geoff Wilson.
--
## Shadow mapping, in my opinion, is a far better option over shadow volumes in most cases.
## It can however take more skill to manage, especially when you start dealing with multiple
## lights.
##
## The type of shadow mapping here is probably the simplest implementation. Uniform, cubic.
## This is for point lights only. Point lights are omni-directional( 540 degree coverage - 360 around then up/down, 90*6 )
## and the cube map allows us to capture the light in all directions, 1 face on the cubemap
## for every direction.
##
## Don't be afraid by this though. The complicated stuff is done in the shaders, depth and lighting, all we really need to
## do is render the cubemap.
--
## The shaders for this example can be found under common\shaders
--
## Remember to setup library paths in the solution properties.
## Additional:
## This is compiled using multi-byte character set.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "stdafx.h"
#include <windows.h>
#include <windowsx.h>

//TV header files. You don't need to include them all of course, but for the purposes of this I am going too.

#include <TV3D65\CTVCamera.h>
#include <TV3D65\CTVCameraFactory.h>
#include <TV3D65\CTVDeviceInfo.h>
#include <TV3D65\CTVShader.h>
#include <TV3D65\CTVTextureFactory.h>
#include <TV3D65\tv_types.h>
#include <TV3D65\HelperFunctions.h>
#include <TV3D65\CTVScreen2DText.h>
#include <TV3D65\CTVScreen2DImmediate.h>
#include <TV3D65\CTVScene.h>
#include <TV3D65\CTVRenderSurface.h>
#include <TV3D65\CTVMesh.h>
#include <TV3D65\CTVMathLibrary.h>
#include <TV3D65\CTVMaterialFactory.h>
#include <TV3D65\CTVLightEngine.h>
#include <TV3D65\CTVInputEngine.h>
#include <TV3D65\CTVGlobals.h>
#include <TV3D65\CTVEngine.h>

//Startup.
CTVMesh            *m_LightMark; //We're going to create a little sphere to represent our light. Don't need to do this of course, it's just so we can see where the light is located.
CTVMesh            *m_Room;      //Our box room.
CTVTextureFactory  *m_Tex;       //This is for basic texturing functionality.
CTVCamera          *m_Cam;       //This is a camera.
CTVScene           *m_Scene;     //This is used for instantiating resource types - Geometry, Render Targets, Shaders and so on.
CTVEngine          *m_Engine;    //This is the main engine class. It's all we require for a blank screen.

CTVMesh            *m_Sphere;    

CTVInputEngine     *m_Inp;       //Input functionality.
CTVMathLibrary     *m_Math;      //Some common math.

CTVRenderSurface   *rtDeep;      //This is our cubic depth map.
CTVShader          *Deep;        //This is our depth shader.
CTVShader          *Lighting;    //This is our lighting and shadowing shader.

float fTime;                     //This is our tick value.
float fHoriz;                    //We use this to rotate the camera horizontally.

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//Setup TV.
void SetupScene(HWND Handle)
{
	//Initialize our horiztontal variable.
	fHoriz      = 0.01f;

	//Instantiate. NOTE: We do NOT instantiate geometry in this way.
	m_Tex       = new CTVTextureFactory;
	m_Cam       = new CTVCamera;
	m_Scene     = new CTVScene;
	m_Engine    = new CTVEngine;
	m_Inp       = new CTVInputEngine;
	m_Math      = new CTVMathLibrary;

	//Now, lets initialize the engine. It's important to understand that this must be done before any other TV class is used - with the exception of CTVDeviceInfo.
	m_Engine->SetDebugFile("debug.txt");                //We want TV to dump a debug file, this is very useful, Sylvain is good at catching common mistakes and informing you that you've made them in this files log.
	m_Engine->SetAngleSystem(cTV_ANGLE_DEGREE);         //Degrees are easier. The default for TV is radians.
	m_Engine->Init3DWindowed(Handle, true);             //We want to initialize on our windows handle and we have hardware transform and lighting.
	//m_Engine->Init3DFullscreen(800, 600, 32, true, false, cTV_DEPTHBUFFER_BESTBUFFER, 1, Handle);

	//Ok, we need to initialize the input engine here.
	m_Inp->Initialize(true, true);

	//Create our cube map. It must be a floating point format and the depthbuffer must be enabled.
	rtDeep = m_Scene->CreateCubeRenderSurface(1024, true, cTV_TEXTUREFORMAT_HDR_FLOAT16, "bah");

	//Load our shaders and set a few of the params.
	Deep = m_Scene->CreateShader("Depth");
	Deep->CreateFromEffectFile("..\\common\\shaders\\meshdeep.shade");

	//Create a small sphere and put it in the same place as the light to give it a visual marker.
	m_LightMark = m_Scene->CreateMeshBuilder("cube");
	m_LightMark->CreateSphere(0.25f, 4, 4);
	m_LightMark->SetPosition(0, 15, 0);



	Lighting = m_Scene->CreateShader("Light and Shadows");
	Lighting->CreateFromEffectFile("..\\common\\shaders\\meshlight.shade");
	Lighting->SetEffectParamTexture("DeepMap", rtDeep->GetTexture());
	Lighting->SetEffectParamVector3("LightPos", &Vector3(0, 15, 0));
	Lighting->SetEffectParamVector4("LightColour", &Vector4(1, 1, 1, 1));
	Lighting->SetEffectParamFloat("SubFactor", float(1.0f / ((2.0f * 3.0f) + 1.0f)) - 0.001f);

	//Same as the cube, just for a different mesh.
	m_Room = m_Scene->CreateMeshBuilder("room");
	m_Room->LoadTVM("..\\common\\room2.tvm", true, true);           
	m_Room->SetPosition(0, -12.5f, 0);                   //The cube is at 0, 0, 0. So lets move the room down a little bit.
	m_Room->SetScale(2, 2, 2);                           //Scale it slightly.
	m_Room->SetLightingMode(cTV_LIGHTING_BUMPMAPPING_TANGENTSPACE, 0, 1);

	m_Sphere = m_Scene->CreateMeshBuilder("sphere");
	m_Sphere->CreateTeapot();
	m_Sphere->SetScale(3, 3, 3);
	m_Sphere->SetLightingMode(cTV_LIGHTING_BUMPMAPPING_TANGENTSPACE, 0, 1);


	m_Cam->SetPosition(0, 20, -20);                      //We move the camera back by 20 on Z because the cube is at 0, 0, 0. So we move it back so we can see the cube.
	m_Cam->SetLookAt(0, 0, 0);
}

void UpdateScene()
{
	//Ok, now we update the camera.
	cTV_3DVECTOR vNew = m_Math->MoveAroundPoint(&Vector3(0, 20, 0), 50, fHoriz, 0);
	m_Cam->SetCamera(vNew.x, vNew.y, vNew.z, 0, 0, 0);

	//Now, lets run some input. This is pretty simple. We just check the A and D keys to see if they're pressed. If they are, we add or subtract from the Horiz variable. :)
	if(m_Inp->IsKeyPressed(cTV_KEY_A)) fHoriz += 0.03f * fTime;
	if(m_Inp->IsKeyPressed(cTV_KEY_D)) fHoriz -= 0.03f * fTime;

	//We put the depth shader on to room geometry.
	m_Room->SetShader(Deep);
	m_Sphere->SetShader(Deep);
	rtDeep->SetCubeMapProperties(true, &Vector3(0, 15, 0));
	rtDeep->GetCamera()->SetViewFrustum(90, 1500, 1);
	//This doesn't need to be set each frame. It only needs to be updated when/if the light moves. I've put it here for clarity.
	Deep->SetEffectParamVector3("LightPos", &Vector3(0, 15, 0));

	//Now, we render to the 6 faces of the cube to record the depth of the scene.
	for(int i = 0; i < 6; i++)
	{
		rtDeep->StartCubeRender(i, false);
			m_Room->Render();
			m_Sphere->Render();
		rtDeep->EndCubeRender(i);
	}
	
	//Once that is done, we set the lighting/shadowing shader on the room geometry.
	m_Room->SetShader(Lighting);
	m_Sphere->SetShader(Lighting);
}

//Entry point.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
//Firstly lets create a window.
    HWND hWnd;
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = (WNDPROC)WindowProc;
    wc.hInstance     = hInstance;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = "WindowClass1";

    RegisterClassEx(&wc);

    int Cx = GetSystemMetrics(SM_CXSCREEN);
    int Cy = GetSystemMetrics(SM_CYSCREEN);

    hWnd = CreateWindowEx(NULL, "WindowClass1", "Simple Shadow Mapping", WS_CAPTION, (Cx / 2) - (800 / 2), (Cy / 2) - (600 / 2), 800, 600, NULL, NULL, hInstance, NULL);    
    ShowWindow(hWnd, nCmdShow);

//Setup our scene, we send it a handle to our window.
	SetupScene(hWnd);

//Now we create a game loop.
    MSG msg;

	while(TRUE)
	{
		//Fill our tick value with the time it took to render the last frame.
		fTime = m_Engine->AccurateTimeElapsed();

		//Render a frame.
		m_Engine->Clear(false);
			m_Room->Render();        //Render the room.
			m_LightMark->Render();   //Render our marker.
			m_Sphere->Render();
		m_Engine->RenderToScreen();

		//Update the scene.
		UpdateScene();

		//Now we process the message queue.
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//If the user closes the window, we break the game loop.
			if(msg.message == WM_QUIT) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

//If the game loop was broken, we clean up.
	m_Engine->ReleaseAll();
	delete(m_Engine);

	return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case WM_DESTROY:
            {
                PostQuitMessage(0);
                return 0;
            } break;
    }
    return DefWindowProc (hWnd, message, wParam, lParam);
}