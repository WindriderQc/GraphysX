#pragma once
class Sprite
{
public:
	Sprite();
	~Sprite();

	void init();
	void draw();

private:
	float x, y, z;
	int Width, Height;
};

