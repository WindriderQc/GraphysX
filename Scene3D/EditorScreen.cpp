#include "stdafx.h"
#include "EditorScreen.h"
#include "ToolsNET.h"
#include <GraphysX\ResourceManager.h>
#include <GraphysX\Tools.h>
#include <GfxNet\GfxNet.h>
#include "IScreenApp.h"
#include <SDL\SDL.h>

namespace BallZ
{

	EditorScreen::EditorScreen()  {
		m_ScreenIndex = SCREEN_INDEX_EDITOR;
	}
	EditorScreen::~EditorScreen()  {
	}
	int EditorScreen::getNextScreenIndex() const  {
		return m_nextScreenIndex;
	}
	int EditorScreen::getPreviousScreenIndex() const  {
		return SCREEN_INDEX_MAINMENU;
	}
	void EditorScreen::render() {
		GraphysX::CLTV3D::renderBegin();
		m_graphysX->renderActiveScene();
		m_gui.Draw();
		//m_graphysX->MouseHighlight();// Find3DObjectWithName("BallZ")->Mesh->GetPosition().y);
		GraphysX::CLTV3D::renderEnd();
		//////////// 
	}
	void EditorScreen::update() {
		m_graphysX->Update();
		//Sprite->m_Shader->SetEffectParamFloat("time", (float)time);
		time++;
		StaticTextNbrPolys->setText(Tools::floatToStr((float)GraphysX::CLTV3D::pTVScene.GetTriangleNumber()));  //  TOOLS DEVRAIT avoir un template ou les autre type que juste float....
		StaticTextTime->setText(Tools::floatToStr((float)GraphysX::CLClock::getRealTime()));

		checkInput();
		if (SelectedMesh)
			EditBoxName->setText(SelectedMesh->mesh->GetMeshName());  // semble crasher si le mesh est effacer... SelectedMesh n'Est ptete pas resetter?

		


		//We put the depth shader on to room geometry.
		/*floor->Mesh->SetShader(m_ShadowMap);
		//test->Mesh->SetShader(m_ShadowMap);
		rtDeep->SetCubeMapProperties(true, &m_graphysX->pActiveScene->getDefaultLightPos());
		rtDeep->GetCamera()->SetViewFrustum(60, 1000, 0.1f);// (90, 1500, 1);
		//This doesn't need to be set each frame. It only needs to be updated when/if the light moves. I've put it here for clarity.
		m_ShadowMap->SetEffectParamVector3("LightPos", &m_graphysX->pActiveScene->getDefaultLightPos());
		//Now, we render to the 6 faces of the cube to record the depth of the scene.
		for (int i = 0; i < 6; i++)
		{
			rtDeep->StartCubeRender(i);
			GraphysX::CLTV3D::pTVScene.RenderAll(true);
			//floor->Mesh->Render();
			//test->Mesh->Render();
			rtDeep->EndCubeRender(i);
		}
		//Once that is done, we set the lighting/shadowing shader on the room geometry.
		floor->Mesh->SetShader(m_Shader);
		//test->Mesh->SetShader(m_Shader);
		m_Shader->SetEffectParamVector3("LightPos", &m_graphysX->pActiveScene->getDefaultLightPos());    //Give the shader a 3-vector, the shader will take this as a light position.
	*/
	}
	void EditorScreen::checkInput() {

		SDL_Event evnt;
		//CTVMesh* tvmesh;
		//CLMeshPrimitive* cube;
		while (SDL_PollEvent(&evnt)) {
			m_screenApp->onSDLEvent(evnt);
			switch (evnt.type) {
			case SDL_MOUSEMOTION:
				//std::cout << evnt.motion.x << "." << evnt.motion.y << std::endl;
				break;
			case SDL_MOUSEBUTTONDOWN:
				SelectedMesh = m_graphysX->isClickOnMesh((float)evnt.motion.x, (float)evnt.motion.y);
				break;
			case SDL_KEYDOWN:
			{
				switch (evnt.key.keysym.sym)
				{
				case SDLK_c:
					m_graphysX->pActiveScene->clCamera.bKeyboardNMouseCtrl = !m_graphysX->pActiveScene->clCamera.bKeyboardNMouseCtrl;
					break;
				case SDLK_r:
					if (SelectedMesh)
						SelectedMesh->mesh->LookAtPoint(&m_graphysX->pActiveScene->getMouse3DPos(), true);
					break;
				case SDLK_s:
				{
					//if (SelectedMesh)
					//SelectedMesh->Mesh->SetColor(RGBA_BLUE);
					bool test = GraphysX::CLTV3D::pTVTexturefactory.SaveTexture(rtDeep->GetTextureEx(0), "shadowmap.bmp"); //.dds", cCONST_TV_IMAGEFORMAT::cTV_IMAGE_DDS);
					//SelectedMesh->SetPPLShader("..\\StockRoom\\distortiontexture.dds", m_graphysX->pActiveScene->getDefaultLightPos(), "pointlight", 0.05f);
				}
				break;
				case SDLK_SPACE:
				{
					STEntityParam param;
					param.name = "cubeBasic";
					m_graphysX->pActiveScene->addDynamicMesh(param);
					currentScene3D->EntityNETList.Add(GraphysX::Tv::nativeToEntityNET(param));
				}
				break;
				case SDLK_m:
				{
					if (SelectedMesh)
					{
						cTV_3DVECTOR pos = SelectedMesh->mesh->GetPosition();
						cTV_3DVECTOR dir;
						GraphysX::CLTV3D::pTVMaths.TVVec3Subtract(&dir, &m_graphysX->pActiveScene->getMouse3DPos(), &pos);
						GraphysX::CLTV3D::pTVMaths.TVVec3Normalize(&dir, &dir);
						//SelectedMesh->Mesh->SetPosition(m_graphysX->pActiveScene->Mouse3DPos.x, m_graphysX->pActiveScene->Mouse3DPos.y, m_graphysX->pActiveScene->Mouse3DPos.z);

						//for (int i = 0; i <= 4; i++)   for all flyers
						//{

						// Check if mesh has reached destination, if not, update the position by adding a scale of the vector destination.
						if (GraphysX::CLTV3D::getDistance3D(pos, m_graphysX->pActiveScene->getMouse3DPos()) > 0.02)
						{
							// Update all the tank's position
							cTV_3DVECTOR dV2;
							GraphysX::CLTV3D::pTVMaths.TVVec3Scale(&dV2, &dir, CLTV3D::getTimeElapsed() * 0.01f);
							GraphysX::CLTV3D::pTVMaths.TVVec3Add(&pos, &pos, &dV2);
							//pos.y = 0;// Land.GetHeight(TankPosition[i].x, TankPosition[i].z) + 10;

							// Update the tank's mesh position
							SelectedMesh->mesh->SetPosition(pos.x, pos.y, pos.z);
						}
					}
				}
				break;
				
				}
			}
			}
		}
	}
	void EditorScreen::build() {
		m_gui.Init(m_graphysX->getAppPath() + "/GUI", GraphysX::CLTV3D::Get3DDevice(), "EditorScreen.layout");
		StaticTextNbrPolys = static_cast<CEGUI::Window*>(m_gui.GetLayoutWnd()->getChild("StaticTextNbrPolys"));
		StaticTextTime = static_cast<CEGUI::Window*>(m_gui.GetLayoutWnd()->getChild("StaticTextTime"));
		CEGUI::PushButton* ButtonQuit = static_cast<CEGUI::PushButton*>(m_gui.GetLayoutWnd()->getChild("ButtonQuit"));
		ButtonQuit->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&EditorScreen::onButtonQuitClick, this));

		EditBoxName = static_cast<CEGUI::Window*>(m_gui.GetLayoutWnd()->getChild("FrameWindowObject/EditboxName"));
		CEGUI::PushButton* ButtonLoad = static_cast<CEGUI::PushButton*>(m_gui.GetLayoutWnd()->getChild("FrameWindowActions/ButtonLoad"));
		ButtonLoad->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&EditorScreen::onButtonLoadClick, this));
		CEGUI::PushButton* ButtonClear = static_cast<CEGUI::PushButton*>(m_gui.GetLayoutWnd()->getChild("FrameWindowActions/ButtonClear"));
		ButtonClear->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&EditorScreen::onButtonClearClick, this));
		CEGUI::PushButton* ButtonSave = static_cast<CEGUI::PushButton*>(m_gui.GetLayoutWnd()->getChild("FrameWindowActions/ButtonSave"));
		ButtonSave->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&EditorScreen::onButtonSaveClick, this));


		CEGUI::PushButton* ButtonAddX = static_cast<CEGUI::PushButton*>(m_gui.GetLayoutWnd()->getChild("FrameWindowActions/ButtonAddX"));
		ButtonAddX->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&EditorScreen::onButtonAddX, this));


		if (!StaticTextNbrPolys) ToolsNET::fatalError("CEGUI Window not found - Editor Screen Layout!");

		currentScene3D = gcnew GfxNet::SceneNET();
	}
	void EditorScreen::destroy(){
	}
	void EditorScreen::onEntry() {
		std::cout << "OnEntry - EditorScreen\n";
		m_graphysX->pActiveScene->clCamera.bKeyboardNMouseCtrl = true;
		m_graphysX->pActiveScene->clCamera.fMoveFactor = 0.08f;
		m_graphysX->pActiveScene->clCamera.pCamera->SetPosition(-0.5, 4, -7);
		m_graphysX->pActiveScene->clCamera.pCamera->SetLookAt(0, 0, 0);
		
		m_graphysX->pActiveScene->CreateVoieLactee();
		//floor = m_graphysX->clMeshFactory.CreateFloor(ResourceManager::getTexture("Damier.jpg"), Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(5, 5, 5), 1, 1, "floor", true);
		
		/*if (true)
		{
			clAtmos = new CLAtmosphere();
			clWater = new CLWater();
		}*/

		

		cTV_3DVECTOR vec = { 3, 1.5f, 0 };
		m_graphysX->pActiveScene->addPrimSphere("test sphere",vec , 1.0f, "Galaxy\\EarthHI.jpg");
		
		STEntityParam stParam;

		stParam.name = "test";
		stParam.geom = eGeometry::CUBE;
		stParam.pos = { 0, 20, 0 };
		stParam.texName = "twoway.jpg";
		stParam.masse = 2;
		test = m_graphysX->pActiveScene->addDynamicMesh(stParam);

		stParam.pos = { 0, 0, 0 };
		stParam.scale = { 10, 0.01f, 10 };
		stParam.texName = "Damier.jpg";
		stParam.masse = 0;
		floor = m_graphysX->pActiveScene->addDynamicMesh(stParam);

		//Sprite = m_graphysX->clMeshFactory.CreateSprite(cTV_3DVECTOR(0, 0, 0), cTV_3DVECTOR(0, 45, 0), cTV_3DVECTOR(5, 5, 0), "triangle", true);
		//Sprite->SetFunkyShader("..\\StockRoom\\cubenormal.png", m_graphysX->pActiveScene->getDefaultLightPos(), "SpriteShader", 0.1f);   //  !!!  Funky shader devrait pas avoir besoin de lightpos
		//test = m_graphysX->clMeshFactory.CreateSphere(ResourceManager::getTexture("ZRing.png"), Vector3(0, .5, 0), Vector3(0, 0, 0), Vector3(2, 2, 2), "test", true);
		int mat = ResourceManager::getMaterial(eMATERIAL::DEF_MATERIAL);
		int IDNormal = ResourceManager::getTexture("..\\StockRoom\\ball_Normal.png");
		test->mesh->SetMaterial(mat);
		floor->mesh->SetMaterial(mat);
		cTV_3DVECTOR lpos = m_graphysX->pActiveScene->getDefaultLightPos();
		
		CTVShader* pplShader = GraphysX::CLTV3D::pTVScene.CreateShader("ppl");
		pplShader->CreateFromEffectFile("..\\StockRoom\\shaders\\ppl.shade");
		pplShader->SetEffectParamVector3("LPos", &m_graphysX->pActiveScene->getDefaultLightPos());
		pplShader->SetEffectParamTexture("Normal", IDNormal);//test->Mesh->SetTextureEx(1, IDNormal);
		pplShader->SetEffectParamFloat("BumpAmount", 0.025f);
		test->mesh->SetShader(pplShader);
		test->mesh->SetLightingMode(cTV_LIGHTING_BUMPMAPPING_TANGENTSPACE);
			
		/*//Create our cube map. It must be a floating point format and the depthbuffer must be enabled.
		rtDeep = GraphysX::CLTV3D::pTVScene.CreateCubeRenderSurface(2048, true, cTV_TEXTUREFORMAT_HDR_FLOAT16, "ShadowMapRenderSurface");
		m_ShadowMap = GraphysX::CLTV3D::pTVScene.CreateShader("ShadowMap");
		m_ShadowMap->CreateFromEffectFile("..\\StockRoom\\shaders\\meshdeep.shade");
		m_Shader = GraphysX::CLTV3D::pTVScene.CreateShader("pplMeshlight");
		m_Shader->CreateFromEffectFile("..\\StockRoom\\shaders\\meshlight.shade");
		m_Shader->SetEffectParamTexture("DeepMap", rtDeep->GetTexture());
		m_Shader->SetEffectParamFloat("pStrength", 0.25f); // pStrength  // BumpAmount
		m_Shader->SetEffectParamVector4("LightColour", &Vector4(1, 0.94f, 0.86f, 1));   // HALOGEN LIGHT :  255, 241, 224
		m_Shader->SetEffectParamFloat("SubFactor", float(1.0f / ((2.0f * 3.0f) + 1.0f)) - 0.001f);
		*/
		
	}
	void EditorScreen::onExit(){
		
		
			std::cout << "OnExit - EditorScreen\n";
	}
	bool EditorScreen::onButtonLoadClick(const CEGUI::EventArgs& e) {
		GraphysX::CLTV3D::pTV.ShowWinCursor(true);
		gcroot<GfxNet::SceneNET^> LoadedScene3D = nullptr;
		LoadedScene3D = GfxNet::SceneNET::Deserialized(ToolsNET::OpenFile("XML Files|*.xml|All Files|*.*")); // Désérialise le XML dans le Active Scene3D
		GraphysX::CLTV3D::pTV.ShowWinCursor(false);

		if (static_cast<CEGUI::ToggleButton*>(m_gui.GetLayoutWnd()->getChild("FrameWindowActions/CheckboxClear"))->isSelected())
		{ 
			m_graphysX->pActiveScene->ClearScene();
			m_graphysX->pActiveScene->BuildScene(GraphysX::Tv::sceneNETtoNative(LoadedScene3D));
		}
		else{
			for each(GfxNet::EntityNET^ Obj in LoadedScene3D->EntityNETList)
			{
				currentScene3D->EntityNETList.Add(Obj);
			}
			m_graphysX->pActiveScene->BuildScene(GraphysX::Tv::sceneNETtoNative(LoadedScene3D));
		}	
		return true;
	}
	bool EditorScreen::onButtonQuitClick(const CEGUI::EventArgs& e) {
		m_nextScreenIndex = SCREEN_INDEX_MAINMENU;
		m_currentState = GraphysX::ScreenState::CHANGE_NEXT;
		return true;
	}
	bool EditorScreen::onButtonClearClick(const CEGUI::EventArgs& e) {
		m_graphysX->pActiveScene->ClearScene();
		currentScene3D->EntityNETList.Clear();
		return true;
	}
	bool EditorScreen::onButtonAddX(const CEGUI::EventArgs& e) {
		GraphysX::CLTV3D::pTV.ShowWinCursor(true);
		//Builder.LoadX(currentScene3D, ToolsNET::OpenFile("X Files|*.x|All Files|*.*"));
		GraphysX::CLTV3D::pTV.ShowWinCursor(false);
		return true;
	}  
	bool EditorScreen::onButtonSaveClick(const CEGUI::EventArgs& e) {
		
		GraphysX::CLTV3D::pTV.ShowWinCursor(true);
		GfxNet::SceneNET::Serialize(ToolsNET::SaveFile("XML Files|*.xml|All Files|*.*"), currentScene3D);
		GraphysX::CLTV3D::pTV.ShowWinCursor(false);
		return true;
	}
}