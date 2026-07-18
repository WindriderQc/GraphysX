#pragma once
#include "IScreen3D.h"
#include "ScreenIndices.h"
#include <GfxNet\SceneNET.h>
#include "BallZLevel.h"
#include "LapChecker.h"
#include "Anneaux.h"
#include <vcclr.h>
#include <GraphysX\ZombieKiller.h>

#include <GraphysX\DynamicMesh.h>

// TO DO:
/*
Game Mode:
Get All Rings -> Best Time
5/10 Laps -> Best time + bonus for rings(-10sec)
2/5 minutes race -> Most Lap + Bonus for Rings (1/NbrRings = Fraction of LAP Bonus)

Ring Class - STD rings + Hard to Get Ring

Seclect Ball Screen - Fire Ball, Ice Ball, etc...    avec different controle value-> friction, speed, etc...

Plane qui get Ball when falling....   Balljoint + slider under plane. Plane grabs ball...   ball slides along slider upon plane speed.

Modelisation : Bungee center as a Level.  BallZ doit aller partout, ce qui rends le site accessible to handicap
*/

class Zombie;  // TODO: check pertinence
//class Bullet;


namespace BallZ
{
	//void GenericContactProcess(const NewtonJoint* contactJoint, dFloat timestep, int threadIndex);


	class GameplayScreen : public GraphysX::IScreen3D
	{
	public:
		GameplayScreen();
		~GameplayScreen();

		virtual int getNextScreenIndex() const override;
		virtual int getPreviousScreenIndex() const override;

		virtual void render() override;
		virtual void update() override;

	

		//called at beginning and end of application
		virtual void build() override;
		virtual void destroy()override;
		// called on screen focus changes
		virtual void onEntry() override;
		virtual void onExit() override;

	private:
		virtual void checkInput();
		std::vector<GraphysX::Bullet*> vecBullets;
		
		
		float MouseX, MouseY;
		gcroot<LapChecker^> lapChecker = nullptr;
		gcroot<GfxNet::SceneNET^> currentScene3D = nullptr;
		vector<BallZLevel> vecLevels;
		int iCurrentLevel = 0;
		CLAnneaux* clAnneaux = nullptr; 
		
		GraphysX::PushPlate* testPlate;
		
		GraphysX::ZombieKiller* m_zombieKiller;
		//std::vector<Human*> vHumans;
		std::vector<Zombie*> vZombies;

		GraphysX::CLDynamicMesh* airplane;

		//bool bMenu = false;
	
		
		
	};

}