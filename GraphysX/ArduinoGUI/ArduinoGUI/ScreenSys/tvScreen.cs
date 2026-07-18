using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using GraphysX;
//#include "..\GraphysX\CLCEGUI.h"

namespace ScreenSys
{
    public enum ScreenState
    {
        NONE,
		    RUNNING,
		    EXIT_APPLICATION,
		    CHANGE_NEXT,
		    CHANGE_PREVIOUS
    };

    public abstract class tvScreen
    {
        public tvScreen(Tv t, scEditor s)
        {
            m_tv = t;
            m_scene = s;
            infoWnd = new Info(m_tv, m_scene);
        }

        protected Tv m_tv = null;
        protected scEditor m_scene = null;
        public Info infoWnd;
        //CLCEGUI m_gui;
        private ScreenState m_currentState = ScreenState.NONE;
        private int m_ScreenIndex = SCREEN_INDEX_NO_SCREEN;
        private bool enable = false;
        public const int SCREEN_INDEX_NO_SCREEN = -1;   //  Other SCREEN INDEX are defined in the application 
        public LevelList levlist = new LevelList();


        public abstract int getNextScreenIndex();
        public abstract int getPreviousScreenIndex();

        public virtual void render()
        {
            m_tv.renderBegin();
            m_scene.render();
            m_tv.renderEnd();
        }
        public virtual void update() { m_scene.update(); }
        public virtual void keyPress(char key)
        {
            switch (key)
            {
                case 'n': nextScreen();                                 break;
                case 'c': m_scene.Cam_SwitchCtrlMode();                    break;
                case 'd': enable = !enable; m_scene.newtonDebug(enable);  break;
                case 's': m_scene.screenShot("C:\\Users\\beayani2\\Desktop\\screenShot_" + DateTime.Now.TimeOfDay.Hours+"_"+ DateTime.Now.TimeOfDay.Minutes + "_" + DateTime.Now.TimeOfDay.Seconds + ".jpg"); break;


            }
        }
        public virtual void mouseClick(int x, int y) 
        {
            infoWnd.Show();
            EntityNET e = m_scene.mousePick();

            if (e != null)
            {
                infoWnd.setName(e.Name);
                infoWnd.setPos(e.Pos);
                infoWnd.setRot(e.Rot);
                infoWnd.setTex(e.TextureName);
            }
        }
        public virtual void mouseDown(int x, int y)
                            {

                            }
        public virtual void mouseUp(int x, int y)
                            {

                            }
        //called at beginning and end of application
        public abstract void build();
        public abstract void destroy();
		// called on screen focus changes
		public abstract void onEntry();
		public abstract void onExit();

		public int getScreenIndex() { return m_ScreenIndex; }
        public void setScreenIndex(int index) { m_ScreenIndex = index; }
        public void setRunning() { m_currentState = ScreenState.RUNNING; }
        public ScreenState getState() { return m_currentState; }
        public void nextScreen() { m_currentState = ScreenState.CHANGE_NEXT; }
        public void prevScreen() { m_currentState = ScreenState.CHANGE_PREVIOUS; }
    }
} 

