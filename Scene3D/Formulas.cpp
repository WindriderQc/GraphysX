#include "stdafx.h"
#include "Formulas.h"
#include <GraphysX\IGraphysX.h>

namespace MathGame
{
	Molecule::Molecule() {
		//m_graphysX->pActiveScene->addDynamicMesh(stCone, ePHYSMAT::DEF_PHYSMAT, "twoway.jpg", 1.0f);
		mesh = GraphysX::CLTV3D::pTVScene.CreateMeshBuilder("Molecule");
		mesh->SetLightingMode(cTV_LIGHTING_MANAGED);
		mesh->CreateBox(0.9f, 0.9f, 0.9f);
		mesh->SetColor(RGBA_BLUE);	
	}
	Molecule::~Molecule() {
		//mesh->Destroy();
		delete(mesh);  mesh = nullptr;
	}
	void Molecule::update()
	{
		mesh->SetPosition(pos.x, pos.y, pos.z);

		if (color != -1) mesh->SetColor(color);
	}

	Formulas::Formulas()
	{	}
	Formulas::~Formulas()
	{
		if (Molecules.size())
		{
			for each(Molecule* mol in Molecules)
			{
				delete(mol); mol = nullptr;
			}
			Molecules.clear();
		}
		else std::cout << "No molecules created, can't delete!! " << std::endl;
	}
	void Formulas::moleculesCreate()
	{
		for (int i = 0; i < MAX_MOLECULES; i++)
			{
				Molecules.push_back(new Molecule());     
			}	
	}
	void Formulas::moleculesUpdate()
	{	
		double halfX = MAX_MOLECULES / 2 / MAX_ZLANES;
		double xRow = -halfX;
		bool first = true;
		for each(Molecule* mol in Molecules)
		{
			if (first)
			{
				x = xRow;
				first = false;
			}
			if (x == MAX_MOLECULES /2 / MAX_ZLANES)  {  // si on a terminé une ligne de mol, on rammene X au début et on incremente d'une rangée Z
				x = -halfX; 
				z++; 
			}
			if (z == MAX_ZLANES)  z = 0;




			switch (iSelectedFormula)              // y 
			{
			case SLOPE:           //   = mx+b  
				y = m*x + b;
				stringFormule = "y = mx+b";
				break;
			case PARABOLA:
				y = a*pow(x,2) + b*x + c; 	
				stringFormule = "y = ax^2 + bx + c";  //  Si A = 0, la parabole devient un slope!! :)
				break;
			}
			
			
		
			mol->pos.x = float(x++);	
			mol->pos.y = float(y);
			mol->pos.z = float(z * 1.5);
			int zRatio = int((z / MAX_ZLANES) * 255);
			mol->color = RGBA256(zRatio, 0, 255 - zRatio, 255);
			mol->update();	
		
		}
	}
	void Formulas::moleculesDelete()
	{
		for each(Molecule* mol in Molecules)
		{
			mol->mesh->Destroy();
			delete(mol);
		}
		Molecules.clear();
	}
}
