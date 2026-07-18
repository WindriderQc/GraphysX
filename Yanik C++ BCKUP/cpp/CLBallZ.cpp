#include "CLBallZ.h"

CLBallZ::CLBallZ(void)
{
	clBallZShell = new CL3DObject("media\\Ball\\BallShell.tvm", NULL, cTV_3DVECTOR(0,500,0),cTV_3DVECTOR(1,1,1), true, 5);  //cTV_3DVECTOR(-20,310,175)
	clBallZShell->SetOpacity(.5);
	clBallZShell->GetMesh()->SetAlphaTest();
	clBallZShell->GetMesh()->SetBlendingMode(cTV_BLEND_ALPHA);
	clBallZCtrl = new CL3DObject("media\\Ball\\BallCtrl.tvm", NULL, cTV_3DVECTOR(0,500,0),cTV_3DVECTOR( 0.8, 0.8, 0.8), true, 5);
}

CLBallZ::~CLBallZ(void)
{
	delete(clBallZShell); clBallZShell = NULL;
	delete(clBallZCtrl); clBallZCtrl = NULL;

}

CTVMesh* CLBallZ::GetBallZMesh()
{
	return(clBallZShell->GetMesh()  );
}

cTV_3DVECTOR CLBallZ::GetBallZPos()
{
	return(clBallZCtrl->GetPosition());
}
void CLBallZ::SetBallZPos(cTV_3DVECTOR pos)
{
	clBallZShell->SetPosition(pos);
    clBallZCtrl->SetPosition(pos);
}



void CLBallZ::Render(void)
{

	  clBallZShell->Render();
	  clBallZCtrl->Render();
}


void CLBallZ::ManageInput(void)
        {
            //Accelerate and Brake
            //float CarPower = 3000; //3000
			stMoveControl.Power = 3000;

            if (pTVInput->IsKeyPressed(cTV_KEY_UPARROW)) //Accellerate
            {
				stMoveControl.Up = true;
            }
            else
            {
               stMoveControl.Up = false; 
               
			   if (pTVInput->IsKeyPressed(cTV_KEY_DOWNARROW))
                {
					stMoveControl.Down = true;
                }
			   else
			   {
					stMoveControl.Down = false;
			   }
            }

            //Steering
            if (pTVInput->IsKeyPressed(cTV_KEY_LEFTARROW))
            {
				stMoveControl.Left = true;
            }
            else
            {
				stMoveControl.Left = false;
                if (pTVInput->IsKeyPressed(cTV_KEY_RIGHTARROW))
                {
                   stMoveControl.Right = true;
                }
                else
                {
                    stMoveControl.Right = false;
                }
            }

            //Car Handbrake
            if (pTVInput->IsKeyPressed(cTV_KEY_RIGHTCONTROL))
            {
				stMoveControl.Handbrake = true;
            }
			else
			{
				stMoveControl.Handbrake = false;
			}



SetBallForce(stMoveControl);

}

void CLBallZ::SetBallForce(STMoveControl stMoveControl)
{

	int iMoveFactor = 0.9;
    cTV_3DVECTOR vec;

    if (stMoveControl.Up)
    {		
		vec = GetBallZPos();
		vec.y = vec.y + iMoveFactor; 
		SetBallZPos(vec);
	}
	else if (stMoveControl.Down)
    {			
		vec = GetBallZPos();
		vec.y = vec.y - iMoveFactor; 
		SetBallZPos(vec);
	}

	if (stMoveControl.Left)
    {	
		vec = GetBallZPos();
		vec.z = vec.z + iMoveFactor; 
		SetBallZPos(vec);
	}
	else if (stMoveControl.Right)
    {
		vec = GetBallZPos();
		vec.z = vec.z - iMoveFactor; 
		SetBallZPos(vec);
	}

             //   pTVPhysics->SetVehicleWheelTorque(car_ID, rlw, stEngCtrl.Power, -1000);
              //  pTVPhysics->SetVehicleWheelTorque(car_ID, rrw, stEngCtrl.Power, -1000);
            
           
            //Car Handbrake
			//if (stEngCtrl.Handbrake)
            //{
            //    pTVPhysics->VehicleWheelHandBrake(car_ID, rlw, 1, 2000);
            //    pTVPhysics->VehicleWheelHandBrake(car_ID, rrw, 1, 2000);
           // }
}