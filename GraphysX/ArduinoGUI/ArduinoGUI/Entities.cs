using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using GraphysX;
using GraphysX.Entities;

namespace Entities
{
    class PhysMesh : DynMesh
    {
        public PhysMesh(EntityNET param, IntPtr scene) : base( param, scene)
        {

        }
        override protected void ForceNTorque()
        {
            addG();
           // addF(new Vec3D(0.5f, 0, 0));
        }
    }
}
