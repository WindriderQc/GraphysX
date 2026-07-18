#pragma once
#include "Vec3D.h"


namespace GfxNet {

	static const char* eObjet_names[] = { "PRIMITIVE", "BILLBOARD", "FLOOR", "CUSTOM", "PHYSPRIMITIVE", "PHYSICCUSTOM", "XMESH", "PHYSICXMESH", "PHYSICXSTATIC", "DUPLICATEMESH", "TEXT", "VERTEXSTRIP", "ACTOR", "NONE", "V1_5" };
	static const char* ePhysmat_names[] = { "DEF_PHYSMAT", "WOOD", "FINISH", "LEVEL", "BALL", "ELEVATOR" };
	static const char* eMaterial_names[] = { "DEF_MATERIAL", "TRANSLUCENT", "GLASS", "MATE", "METAL_SHINE" };
	static const char* eGeometry_names[] = { "CUBE", "SPHERE", "CYLINDER", "CONE", "TEAPOT", "PLANE", "NON_PRIMITIVE", "NONE" };
	
	using namespace System::Xml::Serialization;
	enum class enumMATERIAL  { DEF_MATERIAL, TRANSLUCENT, MATE, METAL_SHINE, GLASS };  // TODO  : check... not used??

	[XmlRoot("EntityNET")]
	public ref class EntityNET
	{
	public:
		[XmlAttribute("Type")]
		enum class enumType { PRIMITIVE, BILLBOARD, FLOOR, CUSTOM, PHYSPRIMITIVE, PHYSICCUSTOM, XMESH, PHYSICXMESH, PHYSICXSTATIC, DUPLICATEMESH, TEXT, VERTEXSTRIP, ACTOR, NONE, V1_5 } Type;
		[XmlAttribute("Geom")]
		enum class enumGeom { CUBE, SPHERE, CYLINDER, CONE, TEAPOT, PLANE, NON_PRIMITIVE, CUSTOM_MESH } Geom;
		[XmlAttribute("NewtonMat")]
		enum class enumPHYSMAT  { DEF_PHYSMAT, WALL, FINISH, GROUND, BALL, HUMAN } physMat;
		[XmlAttribute("Name")]
		System::String^ Name;
		Vec3D^ Pos;
		Vec3D^ Rot;
		Vec3D^ Scale;
		[XmlAttribute("Enabled")]
		bool bEnable;
		[XmlAttribute]
		float Masse;
		[XmlAttribute]
		bool MeshControlled;
	
		System::String^ PathToMesh;    //  TODO : Path to mesh devient inutile si on utilise param.name comme nom de fichier
		System::String^ TextureName;
		Vec3D playerPos;

		EntityNET();
		EntityNET(enumType type, enumGeom geom, bool isActor, System::String^ name, Vec3D^ pos, Vec3D^ rot, Vec3D^ scale, bool enable, float masse
			, bool meshcontrolled, enumPHYSMAT physmat
			, System::String^ pathToMesh, System::String^ textureName);

	};
}