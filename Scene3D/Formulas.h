#pragma once
//#include "Vec3D.h"
#include <vector>
#include <TV3D65\tv_types.h>
using namespace System::Collections::Generic;

#define MAX_MOLECULES  10000
#define MAX_ZLANES 100

class CTVMesh;


namespace MathGame
{
	enum FormulaType { PARABOLA, SLOPE};

	class Molecule
	{
	public:
		Molecule();
		~Molecule();

		void update();

		cTV_3DVECTOR pos = {0,0,0};
		CTVMesh* mesh;
		int color = -1;
	};

	class Formulas
	{
	public:
		Formulas();
		~Formulas();

		void moleculesCreate();
		void moleculesUpdate();
		void moleculesDelete();

		double m = 1;
		double a = 1;
		double b = 1;
		double c = 1;
		double xOffset = 0;
		
		std::string stringFormule;// = "y = ax + bx^2 + c";
		void setFormulatype(FormulaType type) { iSelectedFormula = type; }
	private:
		std::vector<Molecule*> Molecules;
		FormulaType iSelectedFormula = PARABOLA;
		
		double x = 0;
		double y = 0;
		double z = 0;

		/*
		While there are ways to calculate the slope on any point on a parabola I find a simpler method is to save the old position of the objects and set the angle from the old to current position.The event would look like this:

		+every tick
		set angle to angle(self.old_x, self.old_y, self.x, self.y)
		set old_x to self.x
		set old_y to self.y
		*/
	};	
	
	
}