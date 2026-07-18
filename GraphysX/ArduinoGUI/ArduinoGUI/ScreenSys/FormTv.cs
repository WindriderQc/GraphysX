using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using GraphysX;

namespace ScreenSys
{
     public partial class FormTv : Form 
    {
        public FormTv()
        {
            InitializeComponent();
            Application.Idle += HandleApplicationIdle;
            init();
        }

        private Tv m_tv;
        private scEditor m_scene;
        private ScreenList m_screenList;

        //virtual void onSDLEvent(SDL_Event& evnt);

        public bool isRunning() { return m_isRunning; }
        public Tv getTv() { return m_tv; }


      //  private void processInput() { }

        tvScreen m_currentScreen;
        // SDL_Window* sdlWindow = nullptr;
        // HWND winHWND = nullptr;   // used to transfer the window handle to GraphysX
       // private int iScreenWidth = 1280, iScreenHeight = 720;
        private static bool m_isRunning = false;

        protected bool init()
        {
            m_tv = new Tv(this.Handle); 
            if (m_tv == null)
            {
                System.Windows.Forms.MessageBox.Show("Could not init Tv", "Error", System.Windows.Forms.MessageBoxButtons.OK);
                Close();
            }
            m_scene = new scEditor();

            Show();

          

            m_screenList = new ScreenList();
            m_screenList.addScreens(m_tv, m_scene);

            m_currentScreen = m_screenList.getCurrent();
            m_currentScreen.onEntry();
            m_currentScreen.setRunning();
            m_isRunning = true;
           
            return true;
        }

        private void HandleApplicationIdle(object sender, EventArgs e)
        {
            if (m_isRunning)
            {
                while (m_tv.AppStillIdle())
                {
                    render();
                    update();
                }
            }
        }
        private void FormTv_Load(object sender, EventArgs e)
        {
        }

        private void FormTv_FormClosing(object sender, FormClosingEventArgs e)
        {
            m_currentScreen.onExit();
            //SDL_Quit();

            m_tv.Dispose();
        }
       virtual public void render()
        {
           // tv.render();
            if (m_currentScreen != null && m_currentScreen.getState() == ScreenState.RUNNING)
            {
                m_currentScreen.render();
            }
        }
       virtual public void update()
        {
           // tv.update();

            if (m_currentScreen != null)
            {
                switch (m_currentScreen.getState())
                {
                    case ScreenState.RUNNING:
                        m_currentScreen.update();
                        break;
                    case ScreenState.CHANGE_NEXT:
                        m_currentScreen.onExit();
                        m_currentScreen = m_screenList.moveNext();
                        if (m_currentScreen != null)  // TODO : est - ce que ce check esty nécessaire????
                        {
                            m_currentScreen.setRunning();
                            m_currentScreen.onEntry();
                        }
                        break;
                    case ScreenState.CHANGE_PREVIOUS:
                        m_currentScreen.onExit();
                        m_currentScreen = m_screenList.movePrevious();
                        if (m_currentScreen != null)
                        {
                            m_currentScreen.setRunning();
                            m_currentScreen.onEntry();
                        }
                        break;
                    case ScreenState.EXIT_APPLICATION:
                        Close();
                        break;
                    default:
                        break;
                }
            }
            else
            {
               Console.Write("Weird, no currentscreen");
                Close();// exitGame();
            }
        }

        private void FormTv_KeyPress(object sender, KeyPressEventArgs e)
        {
            m_currentScreen.keyPress(e.KeyChar);
        }

        private void FormTv_MouseClick(object sender, MouseEventArgs e)
        {
            m_currentScreen.mouseClick(e.X, e.Y);
        }

        private void FormTv_MouseDown(object sender, MouseEventArgs e)
        {
            m_currentScreen.mouseDown(e.X, e.Y);
        }

        private void FormTv_MouseUp(object sender, MouseEventArgs e)
        {
            m_currentScreen.mouseUp(e.X, e.Y);
        }
    }
}
