#pragma once

using namespace System;

public ref class Player {
public:
	Player(){
		Name = "Leskargau";
		iTime = 9999;
	};
	Player(String^ name, int time){
		Name = name;
		iTime = time;
	};
	String^ Name;
	int iTime;
};