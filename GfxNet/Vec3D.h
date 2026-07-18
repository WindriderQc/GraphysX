#pragma once


public ref class Vec3D
{
public:
	Vec3D() {};
	Vec3D(float X, float Y, float Z) { x = X; y = Y; z = Z; }

	[System::Xml::Serialization::XmlAttribute("x")]
	System::Single x;
	[System::Xml::Serialization::XmlAttribute("y")]
	System::Single y;
	[System::Xml::Serialization::XmlAttribute("z")]
	System::Single z;
};

public ref class Vec2D
{
public:
	Vec2D() {};
	Vec2D(float X, float Y) { x = X; y = Y; }

	[System::Xml::Serialization::XmlAttribute("x")]
	System::Single x;
	[System::Xml::Serialization::XmlAttribute("y")]
	System::Single y;
};