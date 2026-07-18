using System;
using System.Collections.Generic;
using GraphysX;
using GraphysX.Entities;
using Entities;

namespace ScreenSys
{
    public class IntroScreen :  tvScreen
    { 

        public IntroScreen(Tv t, scEditor s) : base(t, s)
        {
            setScreenIndex(ScreenIndices.SCREEN_INDEX_MAINMENU);
        }

        
 
        List<Light> lights = new List<Light>();
        ArduinoGUI.FormLevels selector;
        bool math = false;

        override public int getNextScreenIndex()  {
            if (math)
                return ScreenIndices.SCREEN_INDEX_MATHGAME;
            else
                return ScreenIndices.SCREEN_INDEX_BALLZ;
	    }
        override public int getPreviousScreenIndex() {
		    return SCREEN_INDEX_NO_SCREEN;
	    }
        override public void onEntry()
        {
            this.infoWnd.Show();
            m_scene.Cam_CtrlMode(false);
            m_scene.Cam_setPos(0, 10, -10, 10, 5, 10);
            m_scene.newtonDebug(false);
            m_scene.showAxis(false);
            m_scene.Sky_setCurrent("SkyX", false);
           // m_scene.Sky_enableDayNight(true);
            m_scene.addFloor(40, "\\Texture\\Alien\\Alien01_B_diff.bmp", "\\Texture\\Alien\\Alien01_B_normal.bmp", 16);
            Console.Write("");



            // create point light 
            stLIGHT stLight = new stLIGHT();
            stLight.diffuse = System.Drawing.Color.FromArgb(255, 20, 20, 20);
            stLight.ambient = System.Drawing.Color.FromArgb(255, 10, 10, 10);
            stLight.specular = System.Drawing.Color.FromArgb(255, 1, 1, 1);
            stLight.type = GraphysX.eLIGHTTYPE.LIGHT_POINT;
            stLight.position = new Vec3D(8, 20, 8);
           // stLight.direction = new Vec3D(0, 0,0);
            stLight.range = 40;
            lights.Add(new Light(m_scene.getScene(), stLight, "pointlight1"));
            stLight.position.x = 32;
            stLight.position.z = 32;
            lights.Add(new Light(m_scene.getScene(), stLight, "pointlight2"));
           
            //lights.Add(new Light(m_tv.getActiveScene(), stLight, "pointlight3"));
            //stLight.position.x = 8;
           // lights.Add(new Light(m_tv.getActiveScene(), stLight, "pointlight4"));

            EntityNET stParam1 = new EntityNET()            {
                Name = "sprite",
                Type = EntityNET.enumType.VERTEXSTRIP,
                TextureName = "",
                Scale = new Vec3D(5.5f, 3.25f, 0.0f),
                Pos = new Vec3D(20.0f, 2.0f, 20.0f),
                Rot = new Vec3D(0, 0, 30),
                Masse = 0.0f
            };
            m_scene.addSprite(stParam1);

      
            EntityNET stParam = new EntityNET()            {
                Name = "cylinder",
                Rot = new Vec3D(0, 0, 0),
                Type = EntityNET.enumType.CYLINDER//stParam.Type = 1;
            };
            for (int i = 0; i < 10; i++)
            {
                stParam.TextureName = ""; // Damier.jpg";
                stParam.Scale = new Vec3D(0.5f, 0.25f,8);
                stParam.Pos = new Vec3D(10.0f, 0.5f + i, 10.0f);
                stParam.Masse = 0.1f;
                stParam.LightMatName = "copper";
                m_scene.addMeshPhys(stParam);

                stParam.Pos = new Vec3D(5.0f, 0.5f + i*0.75f, 7.50f);
                stParam.Scale = new Vec3D(0.2f, 0.5f, 8);
                stParam.TextureName = "Coke.jpg";
                stParam.Masse = 0.05f;
                stParam.LightMatName = "default";
                m_scene.addMeshPhys(stParam);

             }

            levlist.Deserialized();
            selector = new ArduinoGUI.FormLevels(levlist.levels);
            selector.Show();
         
           

            Console.Write("On Entry - IntroScreen");
        }

        override public void onExit()
        {
            // physMesh.Dispose();   //  bug car ne s'enleve pas du vEntity de la scene donc au clearscene ca pete....

         
            foreach (Light l in lights) {   l.Dispose();    }

            m_scene.clearScene();
            this.infoWnd.Hide();
            Console.Write( "OnExit - IntroScreen\n");
            // delete(lapChecker); lapChecker = nullptr;     // TODO devrait etre un smart pointer  pas besoin de ca...
                                                          //Builder.ClearScene3D(currentScene3D)
        }
        override public void build()
        {
            //m_gui.Init(m_graphysX->getAppPath() + "/GUI", GraphysX::CLTV3D::Get3DDevice());  // TODO : semble etre loadé a chaque screen..  mettre un flag static dans l'init de CEGUI...
        }
        override public void destroy()
        {
           // GraphysX::ResourceManager::releaseSounds();
        }

        public override void keyPress(char key)
        {
            switch (key)
            {
                case 'z':
                    math = !math;
                    break;
            }
            if (key == 'n')
            {
                if(LevelList.iSelected != -1)
                    base.keyPress(key);
            }  
            else base.keyPress(key);
        }
        public override void update()
        {
            m_scene.update();
            //lights[1].setPos(Tv.RotateAroundY( new Vec3D(10,2,10),10,0.01f));

            if (selector.iSelection != -1)
            {
                LevelList.iSelected = selector.iSelection;
                selector.Hide();
                nextScreen();
            }
            
        }
  

    }
}
