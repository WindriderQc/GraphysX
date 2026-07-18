#include "stdafx.h"
#include "SceneNET.h"

namespace GfxNet
{
	SceneNET::SceneNET()
	{
		mapSize->x = 0;
		mapSize->y = 0;
	}
	void SceneNET::Serialize(String^ Path, Object^ Scene)
	{
		if (Path != "")
		{
			SceneNET^ scene3D = (SceneNET^)Scene;
			scene3D->updateHeader(); /////// RE ECRIT LE HEADER DU XML en cas de modification par l'app dans les enums

			// Write in XML
			StreamWriter^ xmlFile = File::CreateText(Path);
			XmlSerializer^ serializer = gcnew XmlSerializer(scene3D->GetType());
			serializer->Serialize(xmlFile, scene3D);
			xmlFile->Close();
		}
	}
	SceneNET^ SceneNET::Deserialized(std::string Path)
	{
		SceneNET^ scene3D = gcnew SceneNET();
		//if (Path.length() > 1)
		if (Path != "")
		{
			XmlSerializer^ serializer = gcnew XmlSerializer(scene3D->GetType());
			StreamReader^ reader = File::OpenText(ToolsNET::charsToSystemString(Path.c_str()));
			scene3D = (SceneNET^)serializer->Deserialize(reader);
			reader->Close();
		}
		return scene3D;
	}

	void SceneNET::updateHeader()
	{
		ActionsHeader = "\r";
		//  Building the Header string with all Object ID number to be used in XML (used during serialization)
		size_t Nbr = sizeof(eObjet_names) / sizeof(eObjet_names[0]);  // divise l'array totale par la longueur de 1 élément pour avoir le nombre d'élements
		for (UINT i = 0; i < Nbr; i++)  {
			System::String^ objName = gcnew System::String(eObjet_names[i]);
			ActionsHeader = ActionsHeader + "  " + i + "=" + objName;
		}
		ActionsHeader = ActionsHeader + "\r";
		Nbr = sizeof(eGeometry_names) / sizeof(eGeometry_names[0]);
		for (UINT i = 0; i < Nbr; i++)  {
			System::String^ geomName = gcnew System::String(eGeometry_names[i]);
			ActionsHeader = ActionsHeader + "  " + i + "=" + geomName;
		}
		ActionsHeader = ActionsHeader + "\r";// fait la meme chose pour les Physix Material
		Nbr = sizeof(ePhysmat_names) / sizeof(ePhysmat_names[0]);
		for (UINT i = 0; i < Nbr; i++)
		{
			System::String^ matName = gcnew System::String(ePhysmat_names[i]);
			ActionsHeader = ActionsHeader + "  " + i + "=" + matName;

		}
		ActionsHeader = ActionsHeader + "\r";
	}
}