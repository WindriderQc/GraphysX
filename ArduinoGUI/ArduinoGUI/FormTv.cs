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
using GraphysX;

namespace ArduinoGUI
{

    public enum netObjet { PRIMITIVE, BILLBOARD, FLOOR, CUSTOM, PHYSPRIMITIVE, PHYSICCUSTOM, XMESH, PHYSICXMESH, PHYSICXSTATIC, DUPLICATEMESH, TEXT, VERTEXSTRIP, ACTOR, NONE, V1_5 };
    public enum netPHYSMAT { DEF_PHYSMAT, WALL, FINISH, GROUND, BALL, HUMAN };
    public enum netMATERIAL { DEF_MATERIAL, TRANSLUCENT, MATE, METAL_SHINE, GLASS };
    public enum netGeometry { CUBE, SPHERE, CYLINDER, CONE, TEAPOT, PLANE, NON_PRIMITIVE, CUSTOM_MESH };


    public partial class FormTv : Form
    {
        public FormTv()
        {
            InitializeComponent();
            Application.Idle += HandleApplicationIdle;
        }


        private Tv tv;

        private void HandleApplicationIdle(object sender, EventArgs e)
        {
            while (tv.AppStillIdle())
            {
                tv.render();
                tv.update();
            }
        }
        private void FormTv_Load(object sender, EventArgs e)
        {
            tv = new Tv(this.Handle);

            tv.addFloor(10, "twoway.jpg");


            Vec3D pos, rot, scale;
            pos = new Vec3D(3.0f, 0.5f, 10.0f);
            rot = new Vec3D(0, 0, 0);
            scale = new Vec3D(0.5f, 0.5f, 0.5f);


            EntityNET stParam = new EntityNET();
            stParam.Name = "cylinder";
            stParam.Scale = new Vec3D(0.5f, 0.5f, 0.5f);
            stParam.Rot = new Vec3D(0, 0, 0);
            stParam.Geom = EntityNET.enumGeom.CYLINDER;
            //stParam.Type = 1;

            for (int i = 0; i < 100; i++)
            {
                stParam.TextureName = "Zack.jpg";
                stParam.Masse = 0.5f;
                stParam.Pos = new Vec3D(3.0f, 0.5f + i, 10.0f);
                tv.addPhysMesh(stParam);

                stParam.Pos = new Vec3D(1.0f, 0.5f + i, 7.50f);
                stParam.TextureName = "twoway.jpg";
                stParam.Masse = 1.0f;
                tv.addPhysMesh(stParam);

            }


            Console.Write("Screen Open");
        }

        private void FormTv_FormClosing(object sender, FormClosingEventArgs e)
        {
            tv.Dispose();
        }
    }
}
