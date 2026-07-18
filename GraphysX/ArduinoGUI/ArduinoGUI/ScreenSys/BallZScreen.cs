using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using GraphysX;
using GraphysX.Entities;

namespace ScreenSys
{
    class BallZScreen : tvScreen
    {
       
         // PhysMesh physMesh;
        Zk zk;
        List<Light> lights = new List<Light>();
        LChecker lapCheck;
        SceneNET sceneNET;
        RingSys ringSys;
        Airplane plane;

        public BallZScreen(Tv t, scEditor s) : base(t,s)
        {
            setScreenIndex(ScreenIndices.SCREEN_INDEX_BALLZ);
        }
        override public int getNextScreenIndex()
        {
            return ScreenIndices.SCREEN_INDEX_MAINMENU;
        }
        override public int getPreviousScreenIndex()
        {
            return ScreenIndices.SCREEN_INDEX_MAINMENU;
        }
        override public void onEntry()
        {
            Console.Write("OnEntry - BallZScreen\n");
            m_scene.Cam_CtrlMode(false);
            m_scene.Cam_setPos(0, 10, -10, 10, 5, 10);
            m_scene.newtonDebug(false);
            m_scene.showAxis(false);

            levlist.levels.Clear();
            levlist.Deserialized();
            Level l = levlist.levels[LevelList.iSelected];
                    m_scene.Sky_setCurrent(l.SkyDay, false);
                    // m_scene.Sky_enableDayNight(true);
                    Vec2D size = m_scene.BuildASCIIScene(l.ASCIIFilePath);
                    if (l.bAddFloor)
                        m_scene.addFloor((int)size.x, l.FloorTex, l.FloorNormals,6);
        
            ringSys = new RingSys(m_scene.getScene());  //  TODO  : devrait pouvoir passer directement Scene3D
                    ringSys.setBillboardSize(0.5f);
                    ringSys.addRings(0.45f);
            

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

            sceneNET = m_scene.getSceneParam();
            lapCheck = new LChecker(m_scene.getScene(), sceneNET.fS, sceneNET.fF, sceneNET.hS, sceneNET.hF);
            plane = new Airplane(m_scene.getScene());
            EntityNET stStrip = new EntityNET()
            {
                Name = "LandingStrip",
                Rot = new Vec3D(0, 90, 0),
                Type = EntityNET.enumType.CUBE,
                Pos = new Vec3D(30, 0 , 15),
                Scale = new Vec3D(24, 0.1f, 4),
                TextureName = "airportRunway.jpg",
                Masse = 0,
                LightMatName = "default"
            };
            m_scene.addMeshPhys(stStrip);
     

            EntityNET zkParam = new EntityNET()
            {
                Name = "ZombieKiller",
                Pos = new Vec3D(5, 2, 5),
                Scale = new Vec3D(0.3f, 32, 32),
                Type = EntityNET.enumType.SPHERE,
                Masse = 1,
                // TextureName = "Tutash.png",
                // ShaderName = "..\\StockRoom\\shaders\\meshlight.shade"
                LightMatName = "ruby"
            };
            zk = new Zk(zkParam, m_scene.getScene());
          //  m_scene.Cam_setChase(zk.getMesh(), new Vec3D(0,3,-5), new Vec3D(0,0,0), 5);


            EntityNET stHuman = new EntityNET()
            {
                Name = "human",
                Rot = new Vec3D(0, 0, 0),
                Scale = new Vec3D(0.3f, 32, 32),
                Type = EntityNET.enumType.SPHERE,

                speed = 0.2f,
                Color = System.Drawing.Color.Green.ToArgb(),
                physMat = EntityNET.enumPHYSMAT.HUMAN,
                Masse = 0.2f,
            };
            m_scene.addFlameHumans(stHuman, levlist.levels[LevelList.iSelected].iNumHuman);

       

            EntityNET stParam = new EntityNET()
            {
                Name = "cylinder",
                Rot = new Vec3D(0, 0, 0),
                Type = EntityNET.enumType.CYLINDER//stParam.Type = 1;
            };
            for (int i = 0; i < 10; i++)
            {
                stParam.Pos = new Vec3D(5.0f, 0.5f + i, 7.50f);
                stParam.Scale = new Vec3D(0.2f, 0.5f, 16);
                stParam.TextureName = "Coke.jpg";
                stParam.Masse = 0.01f;
                stParam.LightMatName = "default";
                m_scene.addMeshPhys(stParam);
            }





            Console.Write("On Entry - BallZScreen\n");
        }
        override public void onExit()
        {
            levlist.Serialize();
            ringSys.Dispose();
            plane.Dispose();
            zk.Dispose();
            foreach (Light l in lights) { l.Dispose(); }

            m_scene.clearScene();
            Console.Write("OnExit - BallZScreen\n");
            // delete(lapChecker); lapChecker = nullptr;     // TODO devrait etre un smart pointer  pas besoin de ca...
            //Builder.ClearScene3D(currentScene3D)
        }
        override public void build()
        {
            //m_gui.Init(m_graphysX->getAppPath() + "/GUI", GraphysX::CLTV3D::Get3DDevice());  // TODO : semble etre loadé a chaque screen..  mettre un flag static dans l'init de CEGUI...
            //vecLevels.push_back(BallZLevel());  //  Checkerboard.png
            //vecLevels.push_back(BallZLevel("..\\StockRoom\\Suzanne2.xml", "..\\StockRoom\\Suzanne2.ASCII", true, "GrassSample.jpg", "..\\StockRoom\\Suzanne1.obj", RGBA_AQUA, 0.2f, 300));
        }
        override public void destroy()
        {
            // GraphysX::ResourceManager::releaseSounds();
        }
        public override void keyPress(char key)
        {

            switch (key)
            {
               // case 'z': physMesh.addImp(new Vec3D(100.0f, 0.0f, 0.0f)); break;
                case 'b': zk.shotBullet(); break;
                case ' ': zk.jump(); break;
            }
            base.keyPress(key);
 
        }
        public override void update()
        {
            m_scene.update();
            //light.setPos(Tv.RotateAroundY( new Vec3D(10,2,10),10,0.01f));

            Vec3D pos = zk.getPos();

            if (pos.y < -5)
            {
                plane.goCatch(pos);
                if (plane.isCatched())
                    zk.resetPos();
            }

            float ringScore = ringSys.updateNCheckProximity(pos, zk.getSize());

            if (lapCheck.IsLapCompleted(pos))
            {
                double time = lapCheck.getLapTime();
                double currentScore = (ringScore / time) * 100000000;

                if (levlist.levels[LevelList.iSelected].ScoreBest < currentScore) levlist.levels[LevelList.iSelected].ScoreBest = (float)currentScore;

                string string_time = time.ToString("0#:##:##", System.Globalization.CultureInfo.InvariantCulture);
                Console.Write("Lap Completed :  " + string_time + " \n");

            }
            Vec3D center = new Vec3D(sceneNET.mapSize.x / 2, 0, sceneNET.mapSize.y / 2);
            m_scene.Cam_alignWith(center, pos);
        }
        public override void render()
        {
            m_tv.renderBegin();
            m_scene.render();
            ringSys.render();

            m_tv.renderEnd();
        }
        public override void mouseDown(int x, int y)
        {
            zk.setThrottle(true);

        }
        public override void mouseUp(int x, int y)
        {
            zk.setThrottle(false);
        }
    }
}
