using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GraphysX;

namespace ScreenSys
{
    public class ScreenIndices
    {
        public const int SCREEN_INDEX_MAINMENU = 0;
        public const int SCREEN_INDEX_MATHGAME = 1;
        public const int SCREEN_INDEX_BALLZ = 2;
       // public const int SCREEN_INDEX_EDITOR = 5;
       // public const int SCREEN_INDEX_AFTERRACE = 3;
       // public const int SCREEN_INDEX_SELECTRACE = 4;
    }

    public class ScreenList
    {
        public ScreenList()
        {
            m_screens = new List<tvScreen>();
        }

        private IntroScreen m_IntroScreen;
        private MathScreen m_mathGameScreen;
        private BallZScreen m_ballZScreen;
       

        public void addScreens(Tv tv, scEditor s)
        { 
            m_IntroScreen = new IntroScreen(tv, s);
            addScreen(m_IntroScreen);
            m_mathGameScreen = new MathScreen(tv,s);
            addScreen(m_mathGameScreen);
            m_ballZScreen = new BallZScreen(tv,s);
            addScreen(m_ballZScreen);

            setScreen(m_IntroScreen.getScreenIndex());
        }

        public tvScreen moveNext()
        {
            tvScreen currentScreen = getCurrent();
            if (currentScreen.getNextScreenIndex() != tvScreen.SCREEN_INDEX_NO_SCREEN)
            {
                m_currentScreenIndex = currentScreen.getNextScreenIndex();
            }
            return getCurrent();
        }
        public tvScreen movePrevious()
        {
            tvScreen currentScreen = getCurrent();
            if (currentScreen.getPreviousScreenIndex() != tvScreen.SCREEN_INDEX_NO_SCREEN)
            {
                m_currentScreenIndex = currentScreen.getPreviousScreenIndex();
            }
            return getCurrent();
        }
        public  void setScreen(int nextScreen)
        {
            m_currentScreenIndex = nextScreen;
        }
        public void addScreen(tvScreen newScreen)
        {
            newScreen.setScreenIndex(m_screens.Count());
            m_screens.Add(newScreen);
            newScreen.build();
        }
        public  tvScreen getCurrent()
        {
            if (m_currentScreenIndex == tvScreen.SCREEN_INDEX_NO_SCREEN) return null;
            return m_screens[m_currentScreenIndex];
        }

        protected List<tvScreen>  m_screens;
        protected int m_currentScreenIndex = tvScreen.SCREEN_INDEX_NO_SCREEN;
    }
}
