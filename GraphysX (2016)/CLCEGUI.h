#pragma once
#include "CEGUI\CEGUI.h"
#include "CEGUI\RendererModules\Direct3D9\Renderer.h"
#include "TV3D65\tv_types.h"
#include "GraphysXdef.h"
#include <SDL/SDL_events.h>

namespace GraphysX
{
	class CLCEGUI
	{
	public:
		GRAPHYSX_API void Draw();
		GRAPHYSX_API char ScancodeToASCII(UINT scancode, byte KEY_PRESSED[256]);
		GRAPHYSX_API void Update(float elapsedTime);
		GRAPHYSX_API void UpdateTVKeys(byte PressedKey, char PressedChar, byte ReleasedKey); // used if using TV3D input instead of SDL
		GRAPHYSX_API void LoadScheme(const std::string& schemeFile);
		GRAPHYSX_API void SetFont(const std::string& fontFile);
		GRAPHYSX_API void SetMouseCursor(const std::string& imageFile);
		GRAPHYSX_API CEGUI::Window* LoadLayout(const std::string& layoutFile);

		GRAPHYSX_API CEGUI::Window* CreateWidget(const std::string& type, const cTV_4DVECTOR DestPerc, const cTV_4DVECTOR DestPix, const std::string& name = "");
		static void setWidgetDestSize(CEGUI::Window* widgetWin, const cTV_4DVECTOR DestPerc, const cTV_4DVECTOR DestPix);
		//Getters
		static CEGUI::Direct3D9Renderer* getRenderer(){ return Renderer; }
		CEGUI::GUIContext* getContext(){ return m_context; }

		GRAPHYSX_API void ShowGUI(bool show);
		GRAPHYSX_API CEGUI::Window* GetRootWnd(void) { return(m_root); }
		GRAPHYSX_API CEGUI::Window* GetLayoutWnd(void) { return(mLayoutWnd); }
		//GRAPHYSX_API CLCEGUI* getCLCEGUI(){ return this; }
		GRAPHYSX_API void onSDLEvent(SDL_Event& evnt);
		GRAPHYSX_API void Init(const std::string& resourceDirectory, LPDIRECT3DDEVICE9 device3d, std::string PathToLayout = "", const std::string& scheme = "", const std::string& font = "", const std::string& MouseCursor = "");
		GRAPHYSX_API void destroy();
	protected:
	private:
		static CEGUI::Direct3D9Renderer* Renderer;
		CEGUI::GUIContext* m_context = nullptr;
		CEGUI::Window* m_root = nullptr;
		CEGUI::Window* mLayoutWnd = nullptr;
		LPDIRECT3DDEVICE9 d3device;
		HKL keyboardLayout;

		CEGUI::MouseButton SDLButtonToCEGUIButton(Uint8 sdlButton);
		CEGUI::Key::Scan SDLKeyToCEGUIKey(SDL_Keycode key);
	};
}