#include "StdAfx.h"
#include "STSceneParam.h"
#include "Tools.h"


namespace GraphysX
{

	STSceneParam::STSceneParam()
	{
		Entity3DList.push_back(STEntityParam());  // On s'Assure que la list d'entity ne soit pas vide  TODO: is that clean, ca creer un object fantome?!
	}

	//void STSceneParam::setPhysicWorld(NewtonWorld* physWorld) { m_world = physWorld; }  //  TODO: devrait pas etre requis dans cette structure...
	void STSceneParam::setPlayerPos(float fx, float fy, float fz) { playerPos = { fx, fy, fz }; }
	void STSceneParam::updateHeader()
	{
		ActionsHeader = "\r";
		//  Building the Header string with all Object ID number to be used in XML (used during serialization)
		size_t Nbr = sizeof(eObjet_names) / sizeof(eObjet_names[0]);  // divise l'array totale par la longueur de 1 élément pour avoir le nombre d'élements
		for (UINT i = 0; i < Nbr; i++)  {
			std::string objName = eObjet_names[i];
			ActionsHeader = ActionsHeader + "  " + Tools::floatToStr(i) + "=" + objName;
		}
		ActionsHeader = ActionsHeader + "\r";
		Nbr = sizeof(eGeometry_names) / sizeof(eGeometry_names[0]);
		for (UINT i = 0; i < Nbr; i++)  {
			std::string geomName = eGeometry_names[i];
			ActionsHeader = ActionsHeader + "  " + Tools::floatToStr(i) + "=" + geomName;
		}
		ActionsHeader = ActionsHeader + "\r";// fait la meme chose pour les Physix Material
		Nbr = sizeof(ePhysmat_names) / sizeof(ePhysmat_names[0]);
		for (UINT i = 0; i < Nbr; i++)
		{
			std::string matName = ePhysmat_names[i];
			ActionsHeader = ActionsHeader + "  " + Tools::floatToStr(i) + "=" + matName;

		}
		ActionsHeader = ActionsHeader + "\r";
	}

}