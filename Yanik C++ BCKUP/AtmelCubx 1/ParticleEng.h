//---------------------------------------------------------------------------
//
//  Particle Engine header - Copyright © 2000 Michael Fötsch.
//      August 16, 2000
//
//      Demo application for my tutorial "Building a Particle Engine"
//      at www.mr-gamemaker.com
//      If you have any questions, suggestions, or comments, feel free to
//      e-mail me at foetsch@crosswinds.net
//      Visit my homepage at www.crosswinds.net/~foetsch
//
//---------------------------------------------------------------------------

#ifndef ParticleEngH
#define ParticleEngH

#include "StdAfx.h"


//---------------------------------------------------------------------------

// TParticle
//      Describes a single particle
class TParticle
{
public:
	CTVMesh *ParticleMesh;
    cTV_3DVECTOR Position;         // Position of particle in world-space
    cTV_3DVECTOR OldPos;           // Position in last frame
    cTV_3DVECTOR Velocity;         // Velocity along the three axes
    cTV_3DVECTOR Acceleration;     // Acceleration along the three axes
    DWORD Age;                  // Age of particle (sum of TimePassed passed to UpdateSystem)
    float Size;                 // Size of particle
    bool Alive;                 // Does this particle need to be updated? 
};

//---------------------------------------------------------------------------
//-------------------------- TParticleSystem --------------------------------
//---------------------------------------------------------------------------

// TParticleSystem
//      Abstract base class for all particle systems.
class TParticleSystem
{
protected:
    //D3DLVERTEX Shape[4];        // lit vertices for particle billboard
    TParticle *Particles;       // array of particles
    int NumParts;               // number of items pointed to by above pointer

public:
    TParticleSystem(int numparts, cTV_3DVECTOR origin);    // contructor
    virtual ~TParticleSystem();                         // destructor

    cTV_3DVECTOR SystemOrigin;     // origin of system in world-space (origin param of constructor)
	
	CTVScene* pTVScene;
	CTVTextureFactory* pTVTexFac;
	int PartTexture;
	CTVMesh* TVInitialPartMesh;



    virtual void ResetSystem(); // Initializes system (needn't overwrite)
    virtual void SetParticleDefaults(int i) = 0;    // resets particle; must overwrite
    virtual void UpdateSystem(float TimePassed) = 0;    // must overwrite
    virtual HRESULT RenderSystem(void) = 0;   // must overwrite
};

//---------------------------------------------------------------------------
//-------------------------- TSnowfall --------------------------------------
//---------------------------------------------------------------------------

class TSnowfall : public TParticleSystem
{
protected:
    char TexFile[MAX_PATH];     // file name of particle texture (MUST be .bmp or .tga!)

public:
    TSnowfall(int numparts, cTV_3DVECTOR origin, float width, float depth, float ground, int texture);
    virtual ~TSnowfall();

    float Width, Depth, Ground; // size of system. Can be freely changed after initialization.

    virtual void SetParticleDefaults(int i);
    virtual void UpdateSystem(float TimePassed);
    virtual HRESULT RenderSystem(void);
};
/*
//---------------------------------------------------------------------------
//-------------------------- TFountain --------------------------------------
//---------------------------------------------------------------------------

class TFountain : public TParticleSystem
{
protected:
    char TexFile[MAX_PATH];             // texture file MUST be DDS!
    LPDIRECTDRAWSURFACE7 lpDDSTexture;  // surface containing particle texture

public:
    TFountain(LPDIRECT3DDEVICE7 lpDevice, int numparts, D3DVECTOR origin,
        BYTE red, BYTE green, BYTE blue, char *texfile);
    virtual ~TFountain();

    BYTE Red, Green, Blue;  // color of fountain. Can be freely changed after init.

    virtual void SetParticleDefaults(int i);
    virtual void UpdateSystem(DWORD TimePassed);
    virtual HRESULT RenderSystem(LPDIRECT3DDEVICE7 lpDevice);
};
*/
//---------------------------------------------------------------------------
//-------------------------- TSmoke -----------------------------------------
//---------------------------------------------------------------------------

class CLSmoke : public TParticleSystem
{
protected:
    char TexFile[MAX_PATH];     // texture file MUST be .bmp or .tga!

public:
    CLSmoke(int numparts, cTV_3DVECTOR origin, float height, int texture);
    virtual ~CLSmoke();

    float Height;   // can be freely changed after initialization

    virtual void SetParticleDefaults(int i);
    virtual void UpdateSystem(float TimePassed);
    virtual HRESULT RenderSystem(void);
};
/*
//---------------------------------------------------------------------------
//-------------------------- TLawnSprinkler ---------------------------------
//---------------------------------------------------------------------------

// This particle system draws lines instead of billboards. It just happened
// when I used a wrong parameter for the fountain. After a few modifications,
// it started to look interesting...

// Contants specifying behavior of lawn sprinkler:
enum TSprinklerType {stFastRotation, stSlowRotation, stImpulse};

class TLawnSprinkler : public TParticleSystem
{
public:
    TLawnSprinkler(int numparts, D3DVECTOR origin, TSprinklerType type);
    virtual ~TLawnSprinkler();

    TSprinklerType Type;    // one of the above constants
    DWORD Time;             // total age of system (used for rotation)

    virtual void SetParticleDefaults(int i);
    virtual void UpdateSystem(DWORD TimePassed);
    virtual HRESULT RenderSystem(LPDIRECT3DDEVICE7 lpDevice);
};
*/
#endif