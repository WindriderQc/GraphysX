#pragma once
#include "Entity3D.h"
#include <Newton\dCustomJoints\CustomHinge.h>
#include <Newton\dCustomJoints\Custom6DOF.h>
#include <Newton\dCustomJoints\CustomSlidingContact.h>
#include <Newton\newton\Newton.h>
#include <Newton\dMath\dMatrix.h>

namespace GraphysX
{
	class CLHingeJoint
	{
	public:
		CLHingeJoint(NewtonBody* Parent, NewtonBody* Child, float friction, bool limitsEnabled, float limitsMinAngle, float limitsMaxAngle){
			ParentBody = Parent;
			ChildBody = Child;
			Friction = friction;
			LimitsEnabled = limitsEnabled;
			LimitsMinAngle = limitsMinAngle;
			LimitsMaxAngle = limitsMaxAngle;

			//the joint pin is the first row of the matrix, to make a upright pin we
			//take the x axis and rotate by 90 degree around the y axis
			dMatrix localPin(dRollMatrix(90.0f * 3.141592f / 180.0f));
			dMatrix matrix;
			NewtonBodyGetMatrix(ParentBody, &matrix[0][0]);
			dVector size(1, 1, 1);
			matrix.m_posit += dVector(-size.m_x * 0.5f, 0.0f, 0.0f);
			matrix = localPin * matrix;

			// add hinge with limit and friction
			hinge = new CustomHinge(matrix, ChildBody, ParentBody);
			hinge->EnableLimits(LimitsEnabled);
			hinge->SetLimits(LimitsMinAngle, LimitsMaxAngle); // -45.0f * 3.141592f / 180.0f, 45.0f * 3.141592f / 180.0f);
			hinge->SetFriction(Friction);
		};
		~CLHingeJoint(){
			if (hinge) delete(hinge); hinge = NULL;
		};

		CustomHinge* hinge;
		NewtonBody* ParentBody;
		NewtonBody* ChildBody;
		float Friction;
		bool LimitsEnabled;
		float LimitsMinAngle;
		float LimitsMaxAngle;
	};


	class CL6DOFJoint : public CLEntity3D
	{
	public:
		CL6DOFJoint(NewtonBody* Parent, NewtonBody* Child, dVector limitsMinAngle, dVector limitsMaxAngle){

			dMatrix pinMatrix(dGrammSchmidt(dVector(0.0f, -1.0f, 0.0f, 0.0f)));  // TODO :  doit surement etre configurable.
			dMatrix matrix0;
			NewtonBodyGetMatrix(Child, &matrix0[0][0]);
			dVector size(1, 1, 1);
			pinMatrix.m_posit = matrix0.m_posit + dVector(0.0f, size.m_y, 0.0f, 0.0f);  // TODO :  le size doit etre en fonction du mesh size je crois...

			newtonJoint = new Custom6DOF(pinMatrix, pinMatrix, Child, Parent);
			newtonJoint->SetAngularLimits(limitsMinAngle, limitsMaxAngle);
		};
		~CL6DOFJoint(){
			delete(newtonJoint); newtonJoint = nullptr;
		};

		void update() {};
		Custom6DOF* newtonJoint;
	};



	class CLSlidingJoint
	{
	public:
		CLSlidingJoint(NewtonBody* Parent, NewtonBody* Child, float limitsMinDist, float limitsMaxDist, float MinAngularLimit, float MaxAngularLimit){
			ParentBody = Parent;
			ChildBody = Child;

			dMatrix matrix;
			// connect the bodies by a Slider joint
			NewtonBodyGetMatrix(Parent, &matrix[0][0]);        //  !!!!!!!!!!!!!!!!!  TODO : Doit etre le child car le parent peut ete null   !!!!!
			slider = new CustomSlidingContact(matrix, Child, Parent);
			slider->EnableLinearLimits(true);
			slider->SetLinearLimis(limitsMinDist, limitsMaxDist);

			// enable limit of first axis
			slider->EnableAngularLimits(true);
			slider->SetAngularLimis(MinAngularLimit, MaxAngularLimit);// (-7.0f * 3.1416f, 5.0f * 3.1416f);

		};
		~CLSlidingJoint(){
			if (slider) delete(slider); slider = NULL;
		};

		CustomSlidingContact* slider;
		NewtonBody* ParentBody;
		NewtonBody* ChildBody;

	};

}