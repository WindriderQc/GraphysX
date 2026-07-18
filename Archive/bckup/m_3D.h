#ifndef __M_3D_H__
#define __M_3D_H__
#pragma once

using namespace System;
using namespace System::Collections;

// Math //
public ref class m_Vector3
{
public:
	Single x, y, z;
	
	m_Vector3(){};
	m_Vector3(Single X, Single Y, Single Z){ x = X; y = Y; z = Z; }
};
public ref class m_Spline3DVector3
{
public:
	List<m_Vector3^>^ Positions;
	List<m_Vector3^>^ Orientations;
	
	m_Spline3DVector3(){
		Positions = gcnew List<m_Vector3^>();
		Orientations = gcnew List<m_Vector3^>();
	};
};
// 3D  //
public ref class m_PhysicalEntity 
{
public:
	property m_Vector3^ Pos;
	property m_Vector3^ Orientation;
	property Single Lifetime;
	property Boolean Alive;
	
	m_PhysicalEntity(){};
	~m_PhysicalEntity(){};
}; 
// BigWay //
public ref class m_Flyer : m_PhysicalEntity
{
public:
	m_Flyer(){
		Path3D = gcnew m_Spline3DVector3();
	};
	~m_Flyer(){};

	m_Spline3DVector3^ Path3D;
	int iColor;
	int iGroup;
	int iID;
};

public ref class m_LoadOrganizer
{


};
/*
ref class Singleton
{
private:
Singleton() {}
Singleton(const Singleton%) { throw gcnew System::InvalidOperationException("singleton cannot be copy-constructed"); }
static Singleton m_instance;

public:
static property Singleton^ Instance { Singleton^ get() { return %m_instance; } }
};
*/


#endif