using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using GraphysX;
using GraphysX.Entities;
using MathGame;


namespace ScreenSys
{
    class MathScreen : tvScreen
    {
        public const int MAX_MOLECULES = 10000;
        public const int MAX_ZLANES = 100;
        public const int RANGEMAX = 10;
        public const int RANGEOFFSET = RANGEMAX / 2;
        public enum FormulaType { PARABOLA, SLOPE };

        public double m = 1;
        public double a = 1;
        public  double b = 1;
        public  double c = 1;
        public  double xOffset = 0;
        public String stringFormule;// = "y = ax + bx^2 + c";
     
        private List<MoleculeNET> Molecules;
        private FormulaType iSelectedFormula = FormulaType.PARABOLA;
        public void setFormulatype(FormulaType type) { iSelectedFormula = type; }

        double x = 0;  //  TODO:  pkoi ici, et pas dans la fonction?
        double y = 0;
        double z = 0;

        private FormMath formSliders;
       
        public MathScreen(Tv t, scEditor s) : base(t, s)
        {
            setScreenIndex(ScreenIndices.SCREEN_INDEX_MATHGAME);
        }
        override public int getNextScreenIndex()
        {
            return ScreenIndices.SCREEN_INDEX_MAINMENU;
        }
        override public int getPreviousScreenIndex()
        {
            return ScreenIndices.SCREEN_INDEX_MAINMENU; 
        }
        override public void build()
        {
            Molecules = new List<MoleculeNET>(0);   // TODO :  Should it be deleted???  poche de .net  :S
            formSliders = new FormMath();
        }
        override public void destroy()
        {
        }
        override public void onEntry()
        {
            m_scene.Cam_CtrlMode(false);
            m_scene.Cam_setPos(50, 100, -250, 0, 50, 100);


            m_scene.Sky_setCurrent("NightSky", false);
            m_scene.Sky_enableDayNight(false);


            moleculesCreate();
            formSliders.Show();

            Console.Write("On Entry - MathScreen");
        }
        override public void onExit()
        {
            if (Molecules.Count() > 0)
            {
                moleculesDelete();
            }
            else Console.Write("No molecules created, nothing to delete!! \n" );

            formSliders.Hide();

            m_scene.clearScene();
            Console.Write("OnExit - MathScreen\n");
        }

        override public void update()
        {
            // a = sliderA->getCurrentValue() - RANGEOFFSET;
            // b = sliderB->getCurrentValue() - RANGEOFFSET;
            // c = sliderC->getCurrentValue() - RANGEOFFSET;
            // m = sliderM->getCurrentValue() - RANGEOFFSET;
            // xOffset = sliderX->getCurrentValue() - RANGEOFFSET;
            a = formSliders.a / 100;
            b = formSliders.b / 10;
            c = formSliders.c;
            xOffset = formSliders.xOffset;

            moleculesUpdate();
            m_scene.update();
        }
        public override void keyPress(char key)
        {
            if (key == 'n')
            {
                nextScreen();
            }
            else if (key == 'c')
            {
                m_scene.Cam_SwitchCtrlMode();
            }
        }

        void moleculesCreate()
        {
            for (int i = 0; i < MAX_MOLECULES; i++)
            {
                Molecules.Add(new MoleculeNET());
            }
        }
        void moleculesUpdate()
        {
            double halfX = MAX_MOLECULES / 2 / MAX_ZLANES;
            double xRow = -halfX;
            bool first = true;
            foreach(MoleculeNET mol in Molecules)   {
                if (first)
                {
                    x = xRow;
                    first = false;
                }
                if (x == MAX_MOLECULES / 2 / MAX_ZLANES)
                {  // si on a terminé une ligne de mol, on rammene X au début et on incremente d'une rangée Z
                    x = -halfX;
                    z++;
                }
                if (z == MAX_ZLANES) z = 0;


                switch (iSelectedFormula)              // y 
                {
                    case FormulaType.SLOPE:           //   = mx+b  
                        y = m * x + b;
                        stringFormule = "y = mx+b";
                        break;
                    case FormulaType.PARABOLA:
                        y = a * (x*x) + b * x + c; //a * pow(x, 2) + b * x + c;
                        stringFormule = "y = ax^2 + bx + c";  //  Si A = 0, la parabole devient un slope!! :)
                        break;
                }


                mol.setPos(x++, y, z * 1.5);
                double zRatio = ((z / MAX_ZLANES) * 255);
                mol.setColor(zRatio);
                mol.update();

            }
        }
        void moleculesDelete()
        {
            foreach(MoleculeNET mol in Molecules)
            {
                mol.Dispose();
            }
            Molecules.Clear();
        }
        /*
		While there are ways to calculate the slope on any point on a parabola I find a simpler method is to save the old position of the objects and set the angle from the old to current position.The event would look like this:

		+every tick
		set angle to angle(self.old_x, self.old_y, self.x, self.y)
		set old_x to self.x
		set old_y to self.y
		*/
    }
}




