// drw.cpp: implementation of drw.
//////////////////////////////////////////////////////////////////////

#include "drw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///attributes
static HWND hWnd;
static HDC dc;
static HGLRC hrc;

///methods
bool glInit(HWND hwnd)
{
	hWnd=hwnd;
	dc=GetDC(hwnd);
	if (!dc) return false;

	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd,sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize=sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion=1;
	pfd.dwFlags=PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_GENERIC_ACCELERATED |PFD_DOUBLEBUFFER;
	pfd.iPixelType=PFD_TYPE_RGBA;
	pfd.cColorBits=16;
	pfd.cDepthBits=16;
	pfd.iLayerType=PFD_MAIN_PLANE;

	int iPixelFormat = ChoosePixelFormat(dc, &pfd);
	if (iPixelFormat==-1) return false;
	SetPixelFormat (dc, iPixelFormat, &pfd);
	hrc=wglCreateContext(dc);
	wglMakeCurrent(dc,hrc);
	if (!hrc) return false;

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	return true;
}

void glSwap()
{
	glFlush();
	SwapBuffers(dc);
}

void glProject(double& ox,double& oy,double& oz,double& wx,double& wy,double& wz)
{
	double mm[16],pm[16];
	glGetDoublev(GL_MODELVIEW_MATRIX,mm);
	glGetDoublev(GL_PROJECTION_MATRIX,pm);
	int vp[4];
	glGetIntegerv(GL_VIEWPORT,vp);

	gluProject(	ox,oy,oz,
				mm,pm,vp,
				&wx,&wy,&wz);
}