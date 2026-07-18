#include "StdAfx.h"
#include "Elevator.h"

CLElevator::CLElevator(void)
{
		pTVScene = new CTVScene();
		pTVScreen = new CTVScreen2DImmediate();
		path = new CTVPath();

		cTV_3DVECTOR coord[4];
        coord[0] = cTV_3DVECTOR(0, 20, 0);
        coord[1] = cTV_3DVECTOR(100, 20, 0);
        coord[2] = cTV_3DVECTOR(100, 20, 100);
        coord[3] = cTV_3DVECTOR(0, 20, 100);

        for each(cTV_3DVECTOR vec in coord)
            path->AddPathNode(&vec);
      
		Mesh = pTVScene->CreateMeshBuilder();
        Mesh->CreateSphere(5);
        Mesh->SetPath(path);
        //Mesh->SetPathSpeed(20 * tv.AccurateTimeElapsed(), true);
        Mesh->SetPathSpeed(20, true);

		//char* saved = "";
		//path->Save(saved);
}

CLElevator::~CLElevator(void)
{
}

void CLElevator::Render(void)
{
 pTVScreen->Draw_Line3D(coord[0].x, coord[0].y, coord[0].z, coord[1].x, coord[1].y, coord[1].z, RED);
 pTVScreen->Draw_Line3D(coord[1].x, coord[1].y, coord[1].z, coord[2].x, coord[2].y, coord[2].z, RED);
 pTVScreen->Draw_Line3D(coord[2].x, coord[2].y, coord[2].z, coord[3].x, coord[3].y, coord[3].z, RED);
 pTVScreen->Draw_Line3D(coord[3].x, coord[3].y, coord[3].z, coord[0].x, coord[0].y, coord[0].z, RED);

 Mesh->Render();
}