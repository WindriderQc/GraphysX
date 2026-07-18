#pragma once

namespace GraphysX
{
	class CLEntity3D
	{
	public:
		CLEntity3D();
		virtual ~CLEntity3D();
		virtual void update() = 0;
		//virtual void destroy() = 0;
		//std::string name = "entityDefault";

		bool isDead() { return bDead; }
		void kill() { bDead = true; }

		//static std::vector<CLEntity3D*> vecEntity;  // TODO: devrait etre private?
		static int getNbrEntity();
	private:
		static int nbrEntity;
		bool bDead = false;
		int entityID = NULL; 
	};
}