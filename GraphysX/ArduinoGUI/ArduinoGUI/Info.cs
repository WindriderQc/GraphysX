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


namespace GraphysX
{
    public partial class Info : Form
    {
        public Info(Tv t, scEditor s)
        {
            InitializeComponent();
            tv = t;
            sEd = s;


        }

        private Tv tv;
        private scEditor sEd;

        public void setName(String name)
        {
            this.labelName.Text = name;
        }
        public void setPos(Vec3D pos)
        {
            this.labelPos.Text = "Pos: " + pos.x + " , " + pos.y + " , " + pos.z;
        }
        public void setRot(Vec3D rot)
        {
            this.labelRot.Text = "Rot: " + rot.x + " , " + rot.y + " , " + rot.z; ;
        }
        public void setTex(String name)
        {
            this.labelTex.Text = name;
        }



        private void btnLoad_Click(object sender, EventArgs e)
        {
            sEd.deserializeScene();
        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            sEd.serializeScene();           
        }

        private void btnClear_Click(object sender, EventArgs e)
        {
            sEd.Dispose();
        }
    }
}
