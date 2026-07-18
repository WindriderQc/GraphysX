using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;



namespace MathGame
{
    public partial class FormMath : Form
    {
        public FormMath()
        {
            InitializeComponent();
        }

       public double a, b, c, xOffset;

        

        private void FormMath_Load(object sender, EventArgs e)
        {
         
        }


        private void hScrollBarA_ValueChanged(object sender, EventArgs e)
        {
            
        }

        private void hScrollBarA_LocationChanged(object sender, EventArgs e)
        {
        a = hScrollBarA.Value;
        }

        private void trackBar1_Scroll(object sender, EventArgs e)
        {
            a = trackBar1.Value;
        }

        private void hScrollBarB_ValueChanged(object sender, EventArgs e)
        {
            b = hScrollBarB.Value;
        }
        private void hScrollBarC_Scroll(object sender, ScrollEventArgs e)
        {
            c = hScrollBarC.Value;
        }
        private void hScrollBarXOffset_ValueChanged(object sender, EventArgs e)
        {
            xOffset = hScrollBarXOffset.Value;
        }


    }
}
