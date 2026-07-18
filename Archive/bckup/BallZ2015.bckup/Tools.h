#pragma once
#ifndef __Tools_H__
#define __Tools_H__

#include "Fcntl.h" // pour la console
#include "io.h"  // pour la console
#include "tv_Types.h" // pour le cTV_3DVECTOR
#include "tinyxml.h"

// A rendre plus complet


int RandomDice(int Size);
void DevConsoleCreate(void);
vector<cTV_3DVECTOR> XmlSplineRead(string filename);
void XmlSplineAddNode(cTV_3DVECTOR vec, string filename);
std::string FloatToStr(float Value);

#endif