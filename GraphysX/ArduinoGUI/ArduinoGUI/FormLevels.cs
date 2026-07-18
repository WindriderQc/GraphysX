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

namespace ArduinoGUI
{
    public partial class FormLevels : Form
    {
        int iCurrentLevel = 0;
        public  int iSelection = -1;
        List<Level> pLevels;
        string stockPath = "C:/Users/beayani2/Documents/Visual Studio 2017/Projects/GraphysX/StockRoom/";  //  TODO : burk...   linker avec le appPath
        public FormLevels(List<Level> levels)
        {
            pLevels = levels;
            InitializeComponent();
        }

        private void updateFields()
        {
            labelSkyName.Text = pLevels[iCurrentLevel].SkyDay.ToString();
            labelNbrLap.Text = "Nbr Lap: " + pLevels[iCurrentLevel].nbrTour.ToString();
            labelNbrHuman.Text = "Nbr Human: " + pLevels[iCurrentLevel].iNumHuman.ToString();
            labelBestScore.Text = "Best Score: " + pLevels[iCurrentLevel].ScoreBest.ToString();
        }

        private void pictureBoxPrev_Click(object sender, EventArgs e)
        {
            iCurrentLevel--;
            if (iCurrentLevel < 0) iCurrentLevel = pLevels.Count() - 1;

            pictureBoxScene.Image = Image.FromFile(stockPath + pLevels[iCurrentLevel].Screenshot);

            updateFields();
        }

        private void pictureBoxScene_Click(object sender, EventArgs e)
        {
            iSelection = iCurrentLevel;
        }

        private void pictureBoxNext_Click(object sender, EventArgs e)
        {
            iCurrentLevel++;
            if (iCurrentLevel > pLevels.Count()-1) iCurrentLevel = 0;

            pictureBoxScene.Image = Image.FromFile(stockPath + pLevels[iCurrentLevel].Screenshot);

            updateFields();
        }

        private void FormLevels_Shown(object sender, EventArgs e)
        {

            updateFields();
        }
    }
}
