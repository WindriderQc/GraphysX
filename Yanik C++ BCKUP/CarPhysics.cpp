

using MTV3D65; //Truevision3D

namespace VehicleStarterKit
{
    public partial class frmCanvas : Form
    {
        public TVEngine tv;                        
        public TVScene scene;                      
        public TVInputEngine input;                
        public TVGlobals globals;                   
        public TVMathLibrary maths;                 
        public TVTextureFactory texturefactory;     
        public TVMaterialFactory materialfactory;   
        public TVCamera camera;                     
        public TVViewport viewport;
        public TVLightEngine lights;
        public TVAtmosphere atmosphere;
        
        //Game
        public bool bDoLoop;
        
        //Terrain
        TVLandscape Land;

        //Materials
        int matLand;


        public frmCanvas()
        {
            InitializeComponent();
        }

        private void frmCanvas_Load(object sender, EventArgs e)
        {
            lights = new TVLightEngine();
            globals = new TVGlobals();
            atmosphere = new TVAtmosphere();
            maths = new TVMathLibrary();
            materialfactory = new TVMaterialFactory();
            texturefactory = new TVTextureFactory();
            tv = new TVEngine();
            
            //Setup TV
            tv.SetDebugMode(true, true);
            tv.SetDebugFile(System.IO.Path.GetDirectoryName(Application.ExecutablePath) + "\\debugfile.txt");
            tv.SetAntialiasing(true, CONST_TV_MULTISAMPLE_TYPE.TV_MULTISAMPLE_2_SAMPLES);
            
            //Enter Your Beta Username And Password Here
            tv.SetBetaKey("", "");

            tv.SetAngleSystem(CONST_TV_ANGLE.TV_ANGLE_DEGREE);
            tv.Init3DWindowed(this.Handle, true);
            tv.GetViewport().SetAutoResize(true);
            tv.DisplayFPS(true);
            tv.SetVSync(true);
            
            scene = new TVScene();
            
            input = new TVInputEngine();
            input.Initialize(true, true);

            camera = new TVCamera();
            camera = scene.GetCamera();
            camera.SetViewFrustum(45, 1000, 0.1f);
            camera.SetPosition(0, 5, -20);
            camera.SetLookAt(0, 3, 0);

            viewport = new TVViewport();
            viewport = tv.CreateViewport(this.Handle, "viewport");
            viewport.SetCamera(camera);
            viewport.SetBackgroundColor(Color.Blue.ToArgb());
            bDoLoop = true;


            InitSound();

            InitMaterials();

            InitTextures();

            InitFonts();

            InitShaders();

            InitEnvironment();

            InitPhysics();

            InitLandscape();

            InitObjects();

            InitLights();
            
            InitPhysicsMaterials();

            Init2DText();

            this.Show();
            this.Focus();

            GameLoop();

            tv = null;

            this.Close();
        }

        private void InitSound()
        {
            //Add code here
        }

        private void InitGame2DText()
        {
            //Add code here
        }

        private void InitEnvironment()
        {
            //SkyBox
            atmosphere.SkyBox_SetTexture(globals.GetTex("SkyFront"), globals.GetTex("SkyBack"), globals.GetTex("SkyLeft"), globals.GetTex("SkyRight"), globals.GetTex("SkyTop"), globals.GetTex("SkyBottom"));
            atmosphere.SkyBox_Enable(true);
        }

        private void InitFonts()
        {
            //Add code here
        }

        private void InitShaders()
        {
            //Add code here
        }

        private void Init2DText()
        {
            //Add code here
        }

        private void InitLights()
        {
            lights.CreateDirectionalLight(new TV_3DVECTOR(1, -1, -1), 1, 1, 1, "Sun", 1);
            lights.SetLightProperties(globals.GetLight("Sun"), true, true, true);
            lights.SetSpecularLighting(true);
        }

        private void InitPhysics()
        {
            //Add code here
        }

        private void InitObjects()
        {
            //Add code here            
        }

        
        private void InitPhysicsMaterials()
        {
            //Add code here
        }

        private void InitMaterials()
        {
            //Create Materials
            matLand = materialfactory.CreateMaterial("land");

            //Land
            materialfactory.SetSpecular(matLand, 0.1f, 0.1f, 0.1f, 1f);

        }

        private void InitTextures()
        {
            //Land
            texturefactory.LoadTexture(@"Textures\grass.jpg", "Grass");

            //Sky Box
            texturefactory.LoadTexture(@"Textures\skyup.jpg", "SkyTop");
            texturefactory.LoadTexture(@"Textures\skydown.jpg", "SkyBottom");
            texturefactory.LoadTexture(@"Textures\skyleft.jpg", "SkyLeft");
            texturefactory.LoadTexture(@"Textures\skyright.jpg", "SkyRight");
            texturefactory.LoadTexture(@"Textures\skyfront.jpg", "SkyFront");
            texturefactory.LoadTexture(@"Textures\skyback.jpg", "SkyBack");
        }

        private void InitLandscape()
        {
            Land = scene.CreateLandscape("Land");
            
            Land.SetAffineLevel(CONST_TV_LANDSCAPE_AFFINE.TV_AFFINE_LOW);
            int twidth = (64 * 8) / 256;
            int theight = (64 * 8) / 256;

            Land.GenerateTerrain(@"Heightmaps\heightmap.bmp", CONST_TV_LANDSCAPE_PRECISION.TV_PRECISION_HIGH, twidth, theight, 0, 0, 0,true);

            Land.SetTexture(globals.GetTex("Grass"));
            Land.SetMaterial(matLand);
            Land.SetTextureScale(10, 10);
            Land.SetPosition(-((twidth * 256) / 2), 0, -((theight * 256) / 2));
            
        }

        private void GameLoop()
        {
            while (bDoLoop)
            {
                if (this.Focused)
                {
                    CheckInput();
                    
                    tv.Clear(false);

                    //Render Atmosphere
                    atmosphere.SkyBox_Render();
                    
                    //Render Objets
                    Land.Render();
                    
                    //Render Transparent Objects

                    
                    scene.FinalizeShadows();

                    
                    //Lastly Render 2DText or Interface
                    DrawInterface();



                    tv.RenderToScreen();

                }
                else
                {
                    System.Threading.Thread.Sleep(100);
                }

                Application.DoEvents();
            }
        }


        private void DrawInterface()
        {
            
            try
            {
                //Add Code Here
            }
            catch { }
        }

        private void CheckInput()
        {
            if (input.IsKeyPressed(CONST_TV_KEY.TV_KEY_ESCAPE))
            {
                bDoLoop = false;
            }
            float speed = 1f;
            float mousespeed = 0.1f;
            if (input.IsKeyPressed(CONST_TV_KEY.TV_KEY_W))
            {
                camera.MoveRelative(speed, 0, 0, true);
            }
            if (input.IsKeyPressed(CONST_TV_KEY.TV_KEY_S))
            {
                camera.MoveRelative(-speed, 0, 0, true);
            }
            if (input.IsKeyPressed(CONST_TV_KEY.TV_KEY_A))
            {
                camera.MoveRelative(0, 0, -speed, true);
            }
            if (input.IsKeyPressed(CONST_TV_KEY.TV_KEY_D))
            {
                camera.MoveRelative(0, 0, speed, true);
            }
            if (input.IsKeyPressed(CONST_TV_KEY.TV_KEY_E))
            {
                camera.MoveRelative(0, speed / 2, 0, true);
            }
            if (input.IsKeyPressed(CONST_TV_KEY.TV_KEY_Q))
            {
                camera.MoveRelative(0, -(speed / 2), 0, true);
            }

            //Mouse
            int tmpMouseX = 0;
            int tmpMouseY = 0;
            int tmpMouseScrollNew = 0;
            bool tmpMouseB1 = false;
            bool tmpMouseB2 = false;
            bool tmpMouseB3 = false;
            bool tmpMouseB4 = false;

            input.GetMouseState(ref tmpMouseX, ref tmpMouseY, ref tmpMouseB1, ref tmpMouseB2, ref tmpMouseB3, ref tmpMouseB4, ref tmpMouseScrollNew);
            camera.RotateY(tmpMouseX * (mousespeed * 2));
            camera.SetLookAt(camera.GetLookAt().x, camera.GetLookAt().y - (tmpMouseY * ((mousespeed * 2) / 100)), camera.GetLookAt().z);

        }

        private void frmCanvas_FormClosing(object sender, FormClosingEventArgs e)
        {
            bDoLoop = false;
        }
    }
}