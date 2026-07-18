using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using GfxNet;

namespace GfxTester
{
    public partial class Form1 : Form
    {
        GfxTools gfx;
        public Form1()
        {
            InitializeComponent();
            gfx = new GfxTools();
            gfx.window(pictureBox1.Handle);
       
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Console.Write(gfx.time());
          //  Console.Write(gfx.m_t->);
        }
    }
}
