#include "stdafx.h"
#include "EntityNET.h"

namespace GfxNet {

	EntityNET::EntityNET()
	{
		Type = enumType::PHYSPRIMITIVE;
		Geom = enumGeom::CUBE;
		Name = "CubeTestPhys";
		PathToMesh = "";
		TextureName = "TwoWay";
		Pos = gcnew Vec3D(0, 0, 0);
		Rot = gcnew Vec3D(0, 0, 0);
		Scale = gcnew Vec3D(1, 1, 1);
		bEnable = true;
		Masse = 0;
		physMat = enumPHYSMAT::DEF_PHYSMAT;

		MeshControlled = false;
	}


	EntityNET::EntityNET(enumType type, enumGeom geom, bool isActor, System::String^ name, Vec3D^ pos, Vec3D^ rot, Vec3D^ scale, bool enable, float masse
		, bool meshcontrolled, enumPHYSMAT physmat
		, System::String^ pathToMesh, System::String^ textureName)
	{
		Type = type;
		Geom = geom;
		Name = name;
		PathToMesh = pathToMesh;
		TextureName = textureName;
		Pos = pos;
		Rot = rot;
		Scale = scale;
		bEnable = enable;
		Masse = masse;
		physMat = physmat;
		MeshControlled = meshcontrolled;
	}

}