// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define NOMINMAX  // If not present, redefinition warning avec Newton
// Windows Header Files:                                              //***  Aussi dans GraphysXDef car des fois sinon pas reconnu dans le .h  ( ex: Scene.h)  et aussi pour les fichier sans cpp.
#include <windows.h>
#include <WinSock2.h>

#define DLL_EXPORT
#include "GraphysXdef.h" // TODO: ptete trop général d'avoir ca ici....   tout les CPP l'ont maintenant....

