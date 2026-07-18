#include "FlightXScene.h"

// suppression de warning a la compilation
#pragma warning(disable: 4305) // troncation double -> float 
#pragma warning(disable: 4244) // conversion double -> float 

CLFlightXScene::CLFlightXScene(void)
{

	//pTVMaterialfactory = new CTVMaterialFactory();
  //  pTVTexturefactory = new CTVTextureFactory();
   // pTVPhysics = new CTVPhysics();       
    pTVScene = new CTVScene();
	pTVInput = new CTVInputEngine();
	pTVMaths = new CTVMathLibrary();


	clPipe = new CLSlideObject("media\\Pipe1.tvm", 5, cTV_3DVECTOR(0,-500,0),true); 
	clPlane = new CL3DObject("media\\Airplane\\Airplane.tvm", 1, NULL, cTV_3DVECTOR(0,0,0), true, 0) ;
}

CLFlightXScene::~CLFlightXScene(void)
{
}


void CLFlightXScene::Render(void)
{
	clPipe->Render();
	clPlane->Render();
}


cTV_3DVECTOR CLFlightXScene::GetPlanePos(void)
{
	return(clPlane->GetPosition());
}

CTVMesh* CLFlightXScene::GetPlaneMesh(void)
{
	return(clPlane->GetMesh());
}


void CLFlightXScene::ManagePlaneInput(float ElapsedTime)
{

    cTV_3DVECTOR Pos;
    cTV_3DVECTOR XAxis;
    cTV_3DVECTOR YAxis;
    cTV_3DVECTOR ZAxis;
    cTV_3DMATRIX MMatrix;
    float AirB;
    
	

    if(pTVInput->IsKeyPressed(cTV_KEY_R)) 
	{
		pTVMaths->TVMatrixIdentity(&MMatrix);
		pTVMaths->TVQuaternionIdentity(&QCurrent);
		clPlane->GetMesh()->SetMatrix(&MMatrix);
	}   
    
     if(pTVInput->IsKeyPressed(cTV_KEY_SPACE))
        AirB = 0.1;
	 else
        AirB = 0.0;


    
    MMatrix = clPlane->GetMesh()->GetRotationMatrix(); 
	pTVMaths->TVVec3TransformNormal (&XAxis, &cTV_3DVECTOR(1, 0, 0), &MMatrix);
    pTVMaths->TVVec3TransformNormal (&YAxis, &cTV_3DVECTOR(0, 1, 0), &MMatrix);
    pTVMaths->TVVec3TransformNormal (&ZAxis, &cTV_3DVECTOR(0, 0, 1), &MMatrix);

     if(pTVInput->IsKeyPressed(cTV_KEY_LEFTARROW)) 
	 {
		pTVMaths->TVQuaternionRotationAxis( &QRoll, &ZAxis, ROLL_FACTOR * PI / 180 * ElapsedTime);
        pTVMaths->TVQuaternionMultiply( &QCurrent, &QCurrent, &QRoll);
        pTVMaths->TVMatrixRotationQuaternion( &MMatrix, &QCurrent);
        clPlane->GetMesh()->SetRotationMatrix(&MMatrix);
	 }
    else if (pTVInput->IsKeyPressed(cTV_KEY_RIGHTARROW))
	{
        pTVMaths->TVQuaternionRotationAxis( &QRoll, &ZAxis, -ROLL_FACTOR * PI / 180 * ElapsedTime);
        pTVMaths->TVQuaternionMultiply( &QCurrent, &QCurrent, &QRoll);
        pTVMaths->TVMatrixRotationQuaternion( &MMatrix, &QCurrent);
        clPlane->GetMesh()->SetRotationMatrix(&MMatrix);
	 }
    

    if (pTVInput->IsKeyPressed(cTV_KEY_UPARROW))
	{
        pTVMaths->TVQuaternionRotationAxis( &QPitch, &XAxis, PITCH_UP_FACTOR * PI / 180 *  ElapsedTime);
        pTVMaths->TVQuaternionMultiply( &QCurrent, &QCurrent, &QPitch);
        pTVMaths->TVMatrixRotationQuaternion( &MMatrix, &QCurrent);
        clPlane->GetMesh()->SetRotationMatrix(&MMatrix);
	}
    else if (pTVInput->IsKeyPressed(cTV_KEY_DOWNARROW))
	{
        pTVMaths->TVQuaternionRotationAxis( &QPitch, &XAxis, -(PITCH_DOWN_FACTOR + AirB) * PI / 180 * ElapsedTime);
        pTVMaths->TVQuaternionMultiply( &QCurrent, &QCurrent, &QPitch);
        pTVMaths->TVMatrixRotationQuaternion(  &MMatrix, &QCurrent);
        clPlane->GetMesh()->SetRotationMatrix(&MMatrix);
	}
    
    if(pTVInput->IsKeyPressed(cTV_KEY_W)) 
        clPlane->GetMesh()->MoveRelative( 1 * ElapsedTime, 0, 0);
    else if (pTVInput->IsKeyPressed(cTV_KEY_S))
        clPlane->GetMesh()->MoveRelative( -1 * ElapsedTime, 0, 0);
        
   

}