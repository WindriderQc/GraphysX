#ifndef __WRAPPER_H__
#define __WRAPPER_H__
#pragma once
#include "TV3DInterface.h"
#include <msclr\marshal_cppstd.h>
#include "BigWay.h"
#include "m_3D.h"


public ref class Engine3DWrapper {
	TV3DInterface* m_nativeClass;

public:
	Engine3DWrapper(System::IntPtr tvscreen, System::String^ AppPath)
	{
		m_nativeClass = new TV3DInterface((HWND)tvscreen.ToInt32(), msclr::interop::marshal_as<std::string>(AppPath));
		m_TV3D = m_nativeClass->GetTV3D();
		m_Input = m_nativeClass->GetInput();
		m_Area = m_nativeClass->GetArea();
		m_GlobalVar = m_nativeClass->GetGlobalVar();
	}
	~Engine3DWrapper() {
		delete m_nativeClass;
	}
	void UpdateLoop() {
		m_nativeClass->UpdateLoop();
	}
	void Cleanup() {
		m_nativeClass->Cleanup();
	}
	bool AppStillIdle() {
		return(m_nativeClass->AppStillIdle());
	}


	CLTV3DMoteur* m_TV3D;
	CLInput* m_Input;
	CLArea* m_Area;
	CLGlobalVar* m_GlobalVar;


protected:
	!Engine3DWrapper() { delete m_nativeClass; }
};


//XML wrap
using namespace System::Collections::Generic;
using namespace System::Xml::Serialization;
using namespace System::IO;                 // File System Methods



public ref class XMLDataWrapper {
	CLXMLData* m_nativeClass;

public:
	XMLDataWrapper(CLXMLData* data){
		m_nativeClass = data;
		Flyers = gcnew List<m_Flyer^>();

		int iCount = 0;

		for each(CLFlyer* fly in m_nativeClass->clFlyers){
			Flyers->Add(gcnew m_Flyer());
			for each(cTV_3DVECTOR Pos in fly->Position){
				Flyers[iCount]->Path3D->Positions->Add(gcnew m_Vector3(Pos.x, Pos.y, Pos.z));
			}
			iCount++;
		}	
	}
	~XMLDataWrapper() {
		delete m_nativeClass;
	}
	CLXMLData* GetXMLData(){
		return(m_nativeClass);
	}
	void Serialize(System::String^ Path){
		XmlSerializer^ serializer = gcnew XmlSerializer(Flyers->GetType());
		StreamWriter^ writer = File::CreateText(Path);
		serializer->Serialize(writer, Flyers);
		writer->Close();	
	}
	CLXMLData* Deserialized(System::String^ Path)
	{
		XmlSerializer^ serializer = gcnew XmlSerializer(Flyers->GetType());
		StreamReader^ reader = File::OpenText(Path);
		Flyers = (List<m_Flyer^>^)serializer->Deserialize(reader);

		CLXMLData* data = new CLXMLData();
		int iCount = 0;
		int jCount = 0;
		for each(m_Flyer^ mfly in Flyers){
			for each(m_Vector3^ pos in mfly->Position){
				data->clFlyers[iCount]->Position[jCount] = cTV_3DVECTOR(pos->x, pos->y, pos->z);
				jCount++;
			}
			iCount++;
		}
		return(data);
	}
	List<m_Flyer^>^ Flyers;

protected:
	!XMLDataWrapper() { delete m_nativeClass; }
};

/*
ManagedFlyer_List NativeToManaged(CLFlyer_Vector clFlyers_Native){
ManagedFlyer_List result = gcnew ManagedFlyer_List();
if (result != nullptr) {
for (CLFlyer_Vector::iterator i = clFlyers_Native.begin(); i != clFlyers_Native.end(); ++i) {
CLFlyer* nativeValue = *i;
result.Add(gcnew class1_cli(nativeValue));
}
}
return result;



typedef System::Collections::Generic::List<class1_cli> MyList;
typedef std::vector<class1_native> MyVector;

MyList^ NativeToManaged(MyVector& v) {
MyList^ result = gcnew MyList();
if (result != nullptr) {
for (MyVector::iterator i = v.begin(); i != v.end(); ++i) {
class1_native& nativeValue = *i;
result.Add(gcnew class1_cli(nativeValue));
}
}
return result;
}
*/



/*

public ref class InstrumentM
{
String^ _type;
unsigned _depth;

internal:
explicit InstrumentM(Instrument const& i)
: _type(gcnew String(i._type.c_str())),
_depth(i._depth)
{ }

public:
property String^ Type { String^ get() { return _type; } }
property unsigned Depth { unsigned get() { return _depth; } }
};

public ref class InstrumentListM
{
InstrumentList* _list;

public:
InstrumentListM() : _list(new InstrumentList()) { }
~InstrumentListM() { this->!InstrumentListM(); }
!InstrumentListM()
{
delete _list;
_list = nullptr;
}

array<InstrumentM^>^ GetInstruments()
{
if (!_list)
throw gcnew ObjectDisposedException(L"_list");

vector<Instrument*> const& v = _list->getInstruments();
array<InstrumentM^>^ ret = gcnew array<InstrumentM^>(v.size());
for (int i = 0, i_max = ret->Length; i != i_max; ++i)
if (v[i])
ret[i] = gcnew InstrumentM(*v[i])
return ret;
}
};





typedef System::Collections::Generic::List<class1_cli> MyList;
typedef std::vector<class1_native> MyVector;

MyList^ NativeToManaged(MyVector& v) {
MyList^ result = gcnew MyList();
if (result != nullptr) {
for (MyVector::iterator i = v.begin(); i != v.end(); ++i) {
class1_native& nativeValue = *i;
result.Add(gcnew class1_cli(nativeValue));
}
}
return result;
}




*/
#endif
