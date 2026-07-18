#include "stdafx.h"
#include "ActorFactory.h"

CLActorFactory::CLActorFactory(NewtonWorld* world)
{
	World = world; 
	std::cout << "ActorFactory constructed." << std::endl;
	
	//CreateActor("Gate", "..\\StockRoom\\Suzanne1.DoorGate.x", cTV_3DVECTOR(0, 0, 0), "..\\StockRoom\\tv3dlogo_d.dds");
}
CLActorFactory::~CLActorFactory()
{
	for each(CLActor* Obj in vActors)
		delete(Obj);
	std::cout << "ActorFactory destructed." << std::endl;
}
CLActor* CLActorFactory::createCubX()
{	
	vActors.push_back(new CLActor(NULL)); //CubX Rotating as test model
	vActors.back()->tvActor->SetTexture(IDDiffuse, -1);
	return (vActors.back());
}
void CLActorFactory::CreateActor(char* name, char* xMeshPath, cTV_3DVECTOR pos, char* diffuseTexPath)
{
	//Load a texture in to our integer ID.
	CTVTextureFactory* m_Tex = new CTVTextureFactory;
	IDDiffuse = m_Tex->LoadTexture(diffuseTexPath, "diffuse texture", -1, -1, cTV_COLORKEY_NO, true);
	//	IDNormal = m_Tex->LoadTexture("..\\BallZ2015\\Media\\tv3dlogo_n.dds", "bump texture", -1, -1, cTV_COLORKEY_USE_ALPHA_CHANNEL, true);

	//Create and load our shader.
	CTVShader*m_Shader = NULL;
	/*m_Shader = CLTV3D::pTVScene->CreateShader("PerPixel");
	m_Shader->CreateFromEffectFile("..\\BallZ2015\\Media\\shaders\\ppl.shade");

	m_Shader = CLTV3D::pTVScene->CreateShader("PerPixel");
	m_Shader->CreateFromEffectFile("..\\BallZ2015\\Media\\shaders\\ppl.shade");
	m_Shader->SetEffectParamVector3("LPos", &Vector3(0, 0, -10));      //Give the shader a 3-vector, the shader will take this as a light position.
	m_Shader->SetEffectParamFloat("BumpAmount", 0.001f);
	m_Shader->SetEffectParamTexture("Normal", IDNormal); //Set our bump map so the shader can find it. You can do this via semantics, but I wanted to demonstrate SetEffectParam etc.
	*/

	vActors.push_back(new CLActor(name, xMeshPath, pos, m_Shader, World));
	clActorGate = vActors.back();
	clActorGate->tvActor->SetTexture(IDDiffuse, -1);
	//	clActorGate->tvActor->SetTextureEx(cTV_LAYER_NORMALMAP, -1);      //Set our normal map ID on to the cube.

	/*vActors.push_back(new CLActor("testanim", "..\\StockRoom\\TestCube.x", cTV_3DVECTOR(0, 0, 0), m_Shader, World));
	clTestAnim = vActors.back();
	std::cout << "TestAnim Loaded...  Bone Count: " << clTestAnim->tvActor->GetBoneCount() << std::endl;*/
	if(m_Tex) delete(m_Tex);
	if (m_Shader) delete(m_Shader);
}
void CLActorFactory::Update()
{
	for each(CLActor* Obj in vActors){
		/*if (Obj->clPhysicEntity){ 
			//Obj->clPhysicEntity->NewtonDebug(); 
			
			//cTV_3DMATRIX mx = Obj->tvActor->GetBoneMatrix(1);
			//std::cout << mx.m41 << " " << mx.m42 << " " << mx.m43 << " " << mx.m44 << std::endl;
			//NewtonBodySetMatrix(Obj->clPhysicEntity->pbody, mx.m[0]); // A Corriger pour choisir le Bone en question (1)
		Obj->MoveBodyToBone();
		}*/
		Obj->Update(NULL);
	}
}