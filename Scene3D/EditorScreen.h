#pragma once
#include "IScreen3D.h"
#include "ScreenIndices.h"
#include <GfxNet\SceneNET.h>
#include <vcclr.h>  // for gcroot


using namespace GraphysX;

namespace BallZ
{

	class EditorScreen : public GraphysX::IScreen3D
	{
	public:
		EditorScreen();
		~EditorScreen();

		virtual int getNextScreenIndex() const override;
		virtual int getPreviousScreenIndex() const override;

		virtual void render() override;
		virtual void update() override;

		//called at beginning and end of application
		virtual void build() override;
		virtual void destroy() override;
		// called on screen focus changes
		virtual void onEntry() override;
		virtual void onExit() override;

	private:
		virtual void checkInput();
		
		gcroot<GfxNet::SceneNET^> currentScene3D = nullptr;
		int m_nextScreenIndex = SCREEN_INDEX_MAINMENU;
		CEGUI::Window* StaticTextNbrPolys = nullptr;
		CEGUI::Window* StaticTextTime = nullptr;
		CEGUI::Window* EditBoxName = nullptr;
		bool onButtonLoadClick(const CEGUI::EventArgs& e);
		bool onButtonQuitClick(const CEGUI::EventArgs& e);
		bool onButtonClearClick(const CEGUI::EventArgs& e);
		bool onButtonSaveClick(const CEGUI::EventArgs& e);
		bool onButtonAddX(const CEGUI::EventArgs& e);
		
		CLMeshPrimitive* SelectedMesh = nullptr;

		CLMeshPrimitive* Sprite;
		CLMeshPrimitive* floor;
		CTVRenderSurface   *rtDeep;      //This is our cubic depth map.
		CTVShader* m_Shader;
		CTVShader* m_ShadowMap;
		CLMeshPrimitive* test;
		int time = 0;
	};

}