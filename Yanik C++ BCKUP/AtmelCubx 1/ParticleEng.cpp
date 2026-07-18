//---------------------------------------------------------------------------
//
//  Particle Engine source - Copyright © 2000 Michael Fötsch.
//      August 16, 2000
//
//      Demo application for my tutorial "Building a Particle Engine"
//      at www.mr-gamemaker.com
//      If you have any questions, suggestions, or comments, feel free to
//      e-mail me at foetsch@crosswinds.net
//      Visit my homepage at www.crosswinds.net/~foetsch
//
//---------------------------------------------------------------------------

#include "ParticleEng.h"

#define RandFloat ((float)(rand()%1001)/1000.0f)
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//-------------------------- TParticleSystem --------------------------------
//---------------------------------------------------------------------------

TParticleSystem::TParticleSystem(int numparts, cTV_3DVECTOR origin)
{
    //D3DUtil_SetIdentityMatrix(IdentityMatrix);  // we'll need that...
    NumParts = numparts;                    // store number of particles
    Particles = new TParticle[NumParts];    // allocate particles array
    SystemOrigin = origin;                  // store system origin

	pTVScene = new CTVScene();
	pTVTexFac = new CTVTextureFactory();

}
//---------------------------------------------------------------------------

TParticleSystem::~TParticleSystem()
{
    if (Particles) { delete[] Particles; Particles = NULL; }
}
//---------------------------------------------------------------------------

// Reset system
//      Initialize particle system. This method does not have to be overwritten
//      in derived classes. You can overwrite it, of course, to add functionality...
void TParticleSystem::ResetSystem()
{
    for (int i=0; i < NumParts; i++)
        SetParticleDefaults(i);
}

//---------------------------------------------------------------------------
//-------------------------- TSnowfall --------------------------------------
//---------------------------------------------------------------------------

TSnowfall::TSnowfall(int numparts, cTV_3DVECTOR origin,
    float width, float depth, float ground, int texture) :
    TParticleSystem(numparts, origin)   // call inherited constructor
{
    Width = width;          // store system extents
    Depth = depth;
    Ground = ground;
    PartTexture = texture;
	
	
	TVInitialPartMesh = pTVScene->CreateBillboard(PartTexture,SystemOrigin.x,SystemOrigin.y,SystemOrigin.z,32,32,"",true);
    
	TVInitialPartMesh->SetColor(RGBA(1,1,1,1),false);
	int alpha = 160;
	TVInitialPartMesh->SetAlphaTest(true, alpha, true, -1);   
	TVInitialPartMesh->SetCollisionEnable(false,-1,cTV_TESTTYPE_DEFAULT);
	
	TVInitialPartMesh->Enable(false);

	for(int i =0; i<NumParts;i++)
	{
		Particles[i].ParticleMesh = NULL;
		Particles[i].Age = NULL;
	}

}
//---------------------------------------------------------------------------

TSnowfall::~TSnowfall()
{
    if (Particles) { delete[] Particles; Particles = NULL; }
	pTVTexFac->DeleteTexture(PartTexture);
}
//---------------------------------------------------------------------------

void TSnowfall::SetParticleDefaults(int i)
{
float fRand;
	// ****** Voir a utiliser TV Math pour reutiliser les opération sur les vector *******
    // position the particles somewhere near the system origin (same height, within
    // rectangle specified by Width and Depth). Give them random speed downwards:
	Particles[i].Position.x = Width * (RandFloat - 0.5f)  + SystemOrigin.x;
	Particles[i].Position.y = 0  + SystemOrigin.y;
	Particles[i].Position.z = Depth * (RandFloat - 0.5f) + SystemOrigin.z;  // a vérifier si c'Est bien .z qui doit etre la
	
	fRand = RandFloat * 50000 + 5;
	Particles[i].Velocity.x = fRand;//fRand*0.001f-0.0005f;
	fRand = RandFloat* 100000 + 5;
	Particles[i].Velocity.y = -fRand;//-(fRand*0.049+0.049f); //-(RandFloat*0.0049+0.0049f);
	fRand = RandFloat* 50000 + 5;
	Particles[i].Velocity.z = fRand;//fRand*0.001f-0.0005f;

    Particles[i].Acceleration.x = 0;
	Particles[i].Acceleration.y = 0;
	Particles[i].Acceleration.z = 0;

    Particles[i].Size = 3;// 0.1f;
	Particles[i].Alive = true;

	if(Particles[i].ParticleMesh == NULL)
	{
	Particles[i].ParticleMesh = TVInitialPartMesh->Duplicate("Parts",true);
	}
	Particles[i].ParticleMesh->SetScale(Particles[i].Size, Particles[i].Size, Particles[i].Size);
}
//---------------------------------------------------------------------------

void TSnowfall::UpdateSystem(float TimePassed)
{
    for (int i=0; i < NumParts; i++)
    {
		Particles[i].Position.x = Particles[i].Position.x + Particles[i].Velocity.x * (TimePassed); 
		Particles[i].Position.y = Particles[i].Position.y + Particles[i].Velocity.y * (TimePassed);  
		Particles[i].Position.z = Particles[i].Position.z + Particles[i].Velocity.z * (TimePassed);

		Particles[i].Velocity.x = Particles[i].Velocity.x +  Particles[i].Acceleration.x;
		Particles[i].Velocity.y = Particles[i].Velocity.y +  Particles[i].Acceleration.y;
        Particles[i].Velocity.z = Particles[i].Velocity.z +  Particles[i].Acceleration.z;
		
        if (Particles[i].Position.y < Ground) 
			SetParticleDefaults(i);

    }

}
//---------------------------------------------------------------------------

HRESULT TSnowfall::RenderSystem(void)
{
    HRESULT hr;

    for (int i=0; i < NumParts; i++)
    {
        if(Particles[i].Alive)
		{
			Particles[i].ParticleMesh->Enable(true);
			Particles[i].ParticleMesh->SetPosition(Particles[i].Position.x,Particles[i].Position.y,Particles[i].Position.z);
		}
		else
		{
		Particles[i].ParticleMesh->Enable(false);
		}

    }

	hr = true;  //  fausse validation
    return hr;
}


//---------------------------------------------------------------------------
//-------------------------- TSmoke -----------------------------------------
//---------------------------------------------------------------------------

CLSmoke::CLSmoke( int numparts, cTV_3DVECTOR origin,
    float height, int texture) :
    TParticleSystem(numparts, origin)
{
    Height = height;
    PartTexture = texture;

	TVInitialPartMesh = pTVScene->CreateBillboard(PartTexture,SystemOrigin.x,SystemOrigin.y,SystemOrigin.z,32,32,"",true);
    
	TVInitialPartMesh->SetColor(RGBA(1,1,1,1),false);
	int alpha = 160;
	TVInitialPartMesh->SetAlphaTest(true, alpha, true, -1);   
	TVInitialPartMesh->SetCollisionEnable(false,-1,cTV_TESTTYPE_DEFAULT);
	
	TVInitialPartMesh->Enable(false);

	for(int i =0; i<NumParts;i++)
	{
		Particles[i].ParticleMesh = NULL;
		Particles[i].Age = NULL;
	}


}
//---------------------------------------------------------------------------

CLSmoke::~CLSmoke()
{
    if (Particles) { delete[] Particles; Particles = NULL; }
}
//---------------------------------------------------------------------------

void CLSmoke::SetParticleDefaults(int i)
{
	float fRand;
    // Position particle at origin, let it slowly move up:
    Particles[i].Position = cTV_3DVECTOR(0, 0, 0) + SystemOrigin;

	Particles[i].Position.x =  SystemOrigin.x;
	Particles[i].Position.y =  SystemOrigin.y;
	Particles[i].Position.z =  SystemOrigin.z;  

	fRand = RandFloat * 5000 + 5;
	Particles[i].Velocity.x = fRand;//RandFloat*0.0005f-0.00025f;
	fRand = RandFloat* 50000 + 50;
	Particles[i].Velocity.y = fRand;//0.002f+RandFloat*0.002f
	fRand = RandFloat* 5000 + 5;
	Particles[i].Velocity.z = fRand;//RandFloat*0.0005f-0.00025f;

	Particles[i].Size = 3;// 0.1f;
	Particles[i].Alive = true;

	if(Particles[i].ParticleMesh == NULL)
	{
	Particles[i].ParticleMesh = TVInitialPartMesh->Duplicate("Parts",true);
	}
	Particles[i].ParticleMesh->SetScale(Particles[i].Size, Particles[i].Size, Particles[i].Size);


}
//---------------------------------------------------------------------------

void CLSmoke::UpdateSystem(float TimePassed)
{
	 for (int i=0; i < NumParts; i++)
    {
		Particles[i].Position.x = Particles[i].Position.x + Particles[i].Velocity.x * (TimePassed); 
		Particles[i].Position.y = Particles[i].Position.y + Particles[i].Velocity.y * (TimePassed);  
		Particles[i].Position.z = Particles[i].Position.z + Particles[i].Velocity.z * (TimePassed);
		
        if (Particles[i].Position.y  > SystemOrigin.y + Height) 
			SetParticleDefaults(i);

		Particles[i].Size = 2.5f/Height*(Particles[i].Position.y-SystemOrigin.y)+0.5f;
                    // make particle larger with time
    }
}
//---------------------------------------------------------------------------

HRESULT CLSmoke::RenderSystem(void)
{
    HRESULT hr;
    BYTE Alpha;

	for (int i=0; i < NumParts; i++)
    {
		// Calculate transparency: (particles dissolve with time)
        Alpha = 255 - (int)(255.0f/Height*(Particles[i].Position.y-SystemOrigin.y));

        if(Particles[i].Alive)
		{
			Particles[i].ParticleMesh->Enable(true);
			Particles[i].ParticleMesh->SetAlphaTest(true,Alpha,true,-1);
			Particles[i].ParticleMesh->SetPosition(Particles[i].Position.x,Particles[i].Position.y,Particles[i].Position.z);
		}
		else
		{
		Particles[i].ParticleMesh->Enable(false);
		}

    }

	hr = true; // fake valid
    return hr;
}
/*
//---------------------------------------------------------------------------
//-------------------------- TFountain --------------------------------------
//---------------------------------------------------------------------------

TFountain::TFountain(LPDIRECT3DDEVICE7 lpDevice, int numparts, D3DVECTOR origin,
    BYTE red, BYTE green, BYTE blue, char *texfile) :
    TParticleSystem(numparts, origin)
{
    Red = red;          // store color of fountain
    Green = green;
    Blue = blue;

    // Load DDS texture (MUST be DDS texture!)
    lpDevice->EnumTextureFormats( EnumTextureFormats, NULL );
    strcpy(TexFile, texfile);
    lpDDSTexture = NULL;
    LoadTexture( lpDevice, TexFile, &lpDDSTexture );
}
//---------------------------------------------------------------------------

TFountain::~TFountain()
{
    if (Particles) { delete[] Particles; Particles = NULL; }
    if (lpDDSTexture) lpDDSTexture->Release();
}
//---------------------------------------------------------------------------

void TFountain::SetParticleDefaults(int i)
{
    // Place particle at origin, give it speed up and acceleration down (gravity)
    Particles[i].Position = D3DVECTOR(0, 0, 0) + SystemOrigin;
    Particles[i].Velocity = D3DVECTOR(
        RandFloat*0.003f-0.0015f, 0.02f+RandFloat*0.01f, RandFloat*0.003f-0.0015f);
    Particles[i].Acceleration = D3DVECTOR(0, -0.002f, 0);
    Particles[i].Age = 0;
}
//---------------------------------------------------------------------------

void TFountain::UpdateSystem(DWORD TimePassed)
{
    for (int i=0; i < NumParts; i++)
    {
        Particles[i].Position += Particles[i].Velocity * TimePassed;
        Particles[i].Velocity += Particles[i].Acceleration;
        if (Particles[i].Position.y < SystemOrigin.y) SetParticleDefaults(i);
        Particles[i].Age += TimePassed;
        if (Particles[i].Age > 1000) SetParticleDefaults(i);
        Particles[i].Size = 0.5f + (Particles[i].Age/2000.0f);  // make particles larger
                                                                // with time
    }
}
//---------------------------------------------------------------------------

HRESULT TFountain::RenderSystem(LPDIRECT3DDEVICE7 lpDevice)
{
    HRESULT hr;

    lpDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, true);
    lpDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
    lpDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
    lpDevice->SetRenderState(D3DRENDERSTATE_LIGHTING, false);
    lpDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, false);

    D3DMATRIX matViewBak;
    lpDevice->GetTransform(D3DTRANSFORMSTATE_VIEW, &matViewBak);
    lpDevice->SetTransform(D3DTRANSFORMSTATE_VIEW, &IdentityMatrix);

    lpDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, &IdentityMatrix);

    // set texture stage states: (DDS texture will be completely white. Alpha
    // channel contains particle shape.)
    lpDevice->SetTexture( 0, lpDDSTexture );
    lpDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    lpDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
    lpDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    lpDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    lpDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
    lpDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

    D3DVECTOR TransPos;
    BYTE Alpha;
    float fAlpha;
    for (int i=0; i < NumParts; i++)
    {
        D3DMath_VectorMatrixMultiply(TransPos, Particles[i].Position, matViewBak);

        // Calculate particle transparency: (fades out after some time)
        fAlpha = 255.0f-(255.0f/500.0f)*Particles[i].Age;
        if (fAlpha < 0) Alpha = 0;
        else Alpha = (BYTE)fAlpha;
        Shape[0] = D3DLVERTEX(TransPos + D3DVECTOR(-1.0f*Particles[i].Size, 1.0f*Particles[i].Size, 0.0f),
            RGBA_MAKE(Red, Green, Blue, Alpha), 0xffffffff, 0.0f, 0.0f);
        Shape[1] = D3DLVERTEX(TransPos + D3DVECTOR(1.0f*Particles[i].Size, 1.0f*Particles[i].Size, 0.0f),
            RGBA_MAKE(Red, Green, Blue, Alpha), 0xffffffff, 1.0f, 0.0f);
        Shape[2] = D3DLVERTEX(TransPos + D3DVECTOR(-1.0f*Particles[i].Size, -1.0f*Particles[i].Size, 0.0f),
            RGBA_MAKE(Red, Green, Blue, Alpha), 0xffffffff, 0.0f, 1.0f);
        Shape[3] = D3DLVERTEX(TransPos + D3DVECTOR(1.0f*Particles[i].Size, -1.0f*Particles[i].Size, 0.0f),
            RGBA_MAKE(Red, Green, Blue, Alpha), 0xffffffff, 1.0f, 1.0f);
        hr = lpDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, Shape, 4, NULL);
    }

    lpDevice->SetTexture( 0, NULL );
    lpDevice->SetTransform(D3DTRANSFORMSTATE_VIEW, &matViewBak);
    lpDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, false);
    lpDevice->SetRenderState(D3DRENDERSTATE_LIGHTING, true);
    lpDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, true);

    lpDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    lpDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
    lpDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    lpDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    lpDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
    lpDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

    return hr;
}
//---------------------------------------------------------------------------
//-------------------------- TLawnSprinkler ---------------------------------
//---------------------------------------------------------------------------

TLawnSprinkler::TLawnSprinkler(int numparts, D3DVECTOR origin, TSprinklerType type) :
    TParticleSystem(numparts & ~1, origin)
{
    Type = type;
}
//---------------------------------------------------------------------------

TLawnSprinkler::~TLawnSprinkler()
{
    if (Particles) { delete[] Particles; Particles = NULL; }
}
//---------------------------------------------------------------------------

void TLawnSprinkler::SetParticleDefaults(int i)
{
    // Position particle at system origin:
    Particles[i].Position = Particles[i].OldPos = D3DVECTOR(0, 0, 0) + SystemOrigin;

    switch (Type)
    {
    case stFastRotation:        // different angle every millisecond (more or less random)
        Particles[i].Velocity =
            D3DVECTOR( sin(Time)+RandFloat*0.1f-0.05f,
                RandFloat*0.5f+0.5f,
                cos(Time)+RandFloat*0.1f-0.05f );
        break;
    case stSlowRotation:        // change angle slowly
        Particles[i].Velocity =
            D3DVECTOR( sin((float)Time/500.0f)+RandFloat*0.1f-0.05f,
                RandFloat*0.5f+0.5f,
                cos((float)Time/500.0f)+RandFloat*0.1f-0.05f );
        break;
    case stImpulse:             // change angle abruptly every 0.5 seconds
    default:
        Particles[i].Velocity =
            D3DVECTOR( sin(Time/500)+RandFloat*0.1f-0.05f,
                RandFloat*0.5f+0.5f,
                cos(Time/500)+RandFloat*0.1f-0.05f );
        break;
    }

    Particles[i].Velocity /= 50.0f;
    Particles[i].Acceleration = D3DVECTOR(0, -9.8f/10000.0f, 0);    // gravity
    Particles[i].Age = 0;
}
//---------------------------------------------------------------------------

void TLawnSprinkler::UpdateSystem(DWORD TimePassed)
{
    Time += TimePassed;

    for (int i=0; i < NumParts; i++)
    {
        Particles[i].OldPos = Particles[i].Position;
        Particles[i].Position += Particles[i].Velocity * TimePassed;
        Particles[i].Velocity += Particles[i].Acceleration;
        if (Particles[i].Position.y < SystemOrigin.y) SetParticleDefaults(i);
        Particles[i].Age += TimePassed;
    }
}
//---------------------------------------------------------------------------

HRESULT TLawnSprinkler::RenderSystem(LPDIRECT3DDEVICE7 lpDevice)
{
    HRESULT hr;

    lpDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, true);
    lpDevice->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
    lpDevice->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
    lpDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, false);
    lpDevice->SetRenderState(D3DRENDERSTATE_LIGHTING, false);

    D3DMATRIX matViewBak;
    lpDevice->GetTransform(D3DTRANSFORMSTATE_VIEW, &matViewBak);
    lpDevice->SetTransform(D3DTRANSFORMSTATE_VIEW, &IdentityMatrix);
    lpDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, &IdentityMatrix);

    D3DVECTOR TransPos, TransOldPos;    // current particle pos, previous particle pos
    for (int i=0; i < NumParts/2; i++)
    {
        D3DMath_VectorMatrixMultiply(TransPos, Particles[i].Position, matViewBak);
        D3DMath_VectorMatrixMultiply(TransOldPos, Particles[i].OldPos, matViewBak);

        // set up vertices of line:
        Shape[0] = D3DLVERTEX(TransOldPos, 0x50ffffff, 0xffffffff, 0, 0);
        Shape[1] = D3DLVERTEX(TransPos, 0x50ffffff, 0xffffffff, 0, 0);

        // draw line:
        hr = lpDevice->DrawPrimitive(D3DPT_LINELIST, D3DFVF_LVERTEX,
            Shape, 2, NULL);

        // Please note that length of lines depends on speed of machine! You
        // might wish to limit the framerate!
    }

    lpDevice->SetTransform(D3DTRANSFORMSTATE_VIEW, &matViewBak);
    lpDevice->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, false);
    lpDevice->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, true);
    lpDevice->SetRenderState(D3DRENDERSTATE_LIGHTING, true);

    return hr;
}
//---------------------------------------------------------------------------
*/
