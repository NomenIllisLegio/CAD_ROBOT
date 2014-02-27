// drw.h: interface for graphics.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRW_H__D8B5687D_D407_40EA_B1EE_9D9DA5FE571A__INCLUDED_)
#define AFX_DRW_H__D8B5687D_D407_40EA_B1EE_9D9DA5FE571A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include "gl\gl.h"
#include "gl\glu.h"

bool glInit(HWND hwnd);
void glSwap();
void glProject(double& ox,double& oy,double& oz,double& wx,double& wy,double& wz);

#endif // !defined(AFX_DRW_H__D8B5687D_D407_40EA_B1EE_9D9DA5FE571A__INCLUDED_)
