// RobotView.cpp : implementation of the CRobotView class
//

#include "stdafx.h"
#include "robot.h"

#include "robotDoc.h"
#include "RobotView.h"
#include "../BaseLib/drw.h"

#include "KinDirDlg.h"
#include "PosDlg.h"
#include "RevDlg.h"
#include "DinRevDlg.h"
#include "EnergyDlg.h"
#include "GraphsDlg.h"
#include "TrajDlg.h"
#include "InertMatrDlg.h"
#include ".\robotview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CRobotDoc * pRD;
extern CRobotView * pRV;
extern CMainFrame * pMF;

//extern SLinkData LDs[3];

/////////////////////////////////////////////////////////////////////////////
// CRobotView

IMPLEMENT_DYNCREATE(CRobotView, CView)

BEGIN_MESSAGE_MAP(CRobotView, CView)
	//{{AFX_MSG_MAP(CRobotView)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
  ON_WM_DESTROY()
  ON_COMMAND(ID_ANLS_KIN_DIR, OnAnlsKinDir)
  ON_UPDATE_COMMAND_UI(ID_ANLS_KIN_DIR, OnUpdateAnlsKinDir)
  ON_COMMAND(ID_ANLS_KIN_REV, OnAnlsKinRev)
  ON_UPDATE_COMMAND_UI(ID_ANLS_KIN_REV, OnUpdateAnlsKinRev)
  ON_COMMAND(ID_ANLS_TRAJECTORY, OnAnlsTrajectory)
  ON_UPDATE_COMMAND_UI(ID_ANLS_TRAJECTORY, OnUpdateAnlsTrajectory)
  ON_COMMAND(ID_ANLS_MATR_INERT, OnAnlsMatrInert)
  ON_COMMAND(ID_CTRL_DO_TRAJ, OnCtrlDoTraj)
  ON_UPDATE_COMMAND_UI(ID_CTRL_DO_TRAJ, OnUpdateCtrlDoTraj)
  ON_COMMAND(ID_ANLS_DYN_REV, OnAnlsDynRev)
  ON_UPDATE_COMMAND_UI(ID_ANLS_AXIS, OnUpdateAnlsAxis)
  ON_UPDATE_COMMAND_UI(ID_ANLS_DER_MATRICES, OnUpdateAnlsDerMatrices)
  ON_UPDATE_COMMAND_UI(ID_ANLS_MATR_INERT, OnUpdateAnlsMatrInert)
  ON_UPDATE_COMMAND_UI(ID_ANLS_MATRICES, OnUpdateAnlsMatrices)
  ON_UPDATE_COMMAND_UI(ID_ANLS_DYN_REV, OnUpdateAnlsDynRev)
  ON_COMMAND(ID_ANLS_ENERGY, OnAnlsEnergy)
  ON_UPDATE_COMMAND_UI(ID_ANLS_ENERGY, OnUpdateAnlsEnergy)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRobotView construction/destruction

CRobotView::CRobotView()
{
	pRV = this;
	sTrans.Init(3);
  m_first_start = true;
}

CRobotView::~CRobotView()
{
}

BOOL CRobotView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CRobotView drawing

void CRobotView::OnDraw(CDC* pDC)
{
	CRobotDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	Draw();	
}

/////////////////////////////////////////////////////////////////////////////
// CRobotView printing

BOOL CRobotView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRobotView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CRobotView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

/////////////////////////////////////////////////////////////////////////////
// CRobotView diagnostics

#ifdef _DEBUG
void CRobotView::AssertValid() const
{
	CView::AssertValid();
}

void CRobotView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRobotDoc* CRobotView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRobotDoc)));
	return (CRobotDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRobotView message handlers

void CRobotView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	if (!glInit(m_hWnd))
	{
		MessageBox("Невозможно корректно инициализировать OpenGL!", "Критическая ошибка", MB_ICONERROR | MB_OK);
		OnClose();
	}	
	pRD = GetDocument();
	
  //CRect rect;
  //GetClientRect(rect);

  if (m_first_start)
  {
    //creating nm_windows;
    m_dlgs.kin_dir = new CKinDirDlg(this);
    m_dlgs.kin_dir->Create(IDD_KINDIR_DLG, this);
    m_dlgs.kin_dir_shown = false;


    m_dlgs.pos = new CPosDlg(this);
    m_dlgs.pos->Create(IDD_POS_DLG, this);
    m_dlgs.pos_shown = true;
    m_dlgs.pos->SetWindowPos(NULL, 2, 88, 0, 0, SWP_NOSIZE);
    m_dlgs.pos->ShowWindow(SW_SHOW);
  
    m_first_start = false;
  }

  m_playing = false;
  on_update(modify::big);
}

void CRobotView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	glViewport(0,0,cx,cy);
}

void CRobotView::Prepare()
{
	RECT rClient;
	int ScaleY,ScaleX;
	GetClientRect(&rClient);
	int cx=rClient.right-rClient.left;
	int cy=rClient.bottom-rClient.top;
	if (cx<cy)
	{
		ScaleX=pRD->S.Scale;
		ScaleY=cy*pRD->S.Scale/cx;
	}
	else
	{
		ScaleX=cx*pRD->S.Scale/cy;
		ScaleY=pRD->S.Scale;
	}

	glMatrixMode(GL_PROJECTION);
	XVector<4,double> Vs[4];//column vectors;
	
	switch (pRD->S.Mode)
	{
	case 0://2D
		Vs[0]=-2.0/ScaleX/1.41421356,-2.0f/ScaleY/1.41421356,0,0;
		Vs[1]=2.0/ScaleX,0,0,0;
		Vs[2]=0,2.0/ScaleY,0,0;
		Vs[3]=0,0,0,1;
		break;
	}
	
	mProj=Vs[0],Vs[1],Vs[2],Vs[3];
	glLoadMatrixd(mProj.dptr());	

	sTrans=0,0,0;
	if (pRD->S.Center)
	{
		MMatrix<double> bmin(3),bmax(3);
		MMatrix<float> wc(3);
		bmin=0,0,0;
		bmax=-bmin;
		SRLinkPositions * pLPs=core().rLinkPositions();
		for (Size l=0;l<pLPs->pLPs.size();l++)
		{
			wc=pLPs->pLPs[l];
			if (wc[0]>bmax[0]) bmax[0]=wc[0];
			if (wc[1]>bmax[1]) bmax[1]=wc[1];
			if (wc[2]>bmax[2]) bmax[2]=wc[2];
			
			if (wc[0]<bmin[0]) bmin[0]=wc[0];
			if (wc[1]<bmin[1]) bmin[1]=wc[1];
			if (wc[2]<bmin[2]) bmin[2]=wc[2];
		}
		sTrans=-(bmax+bmin)/2;
	}

	//clearing
	glClearColor(1,1,1,1);
	glClear(GL_COLOR_BUFFER_BIT);
}

void CRobotView::RenderLinks()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	InitialModelView();
	for (int l=0;l<core().link_count();l++)
	{
		if (pRD->H.SelLink==l)
			glColor3f(1,0,0);
		else
			glColor3f(0,0,0);
		Link *pL=&core().link(l);
		if (l>0)
		{
			Link *pOL=&core().link(l-1);
			int pm=core().data().LDs[pOL->Type].Motion;
			switch (pm)
			{
			case 0:
				glTranslatef(0,0,pOL->m_curd);
				break;
			case 1:
				glRotatef(pOL->m_curd,0,0,1);
				break;
			}
		}
		glMultMatrixf(pLTMs->pTMs[l].t().pData);
		DrawElement(pL->Type, l);	
	}
	glPopMatrix();
}

void CRobotView::Draw()
{
	Prepare();
	if (core().link_count()>0)
	{
		pRCSs=core().rCoordinateSystems();
		pTMs=core().rStaticTMs();
		pLTMs=core().rStaticTMs(true);
		pPTs=core().rParamsTable();
		XMatrix<4,4,float> m;
	
		RenderLinks();
		if (pRD->S.aAxis) RenderCoordSystems();	
	

	}
	glSwap();
}

void CRobotView::DrawElement(int type, int num) const
{
	switch (type)
	{
	case 0:
		glBegin(GL_LINE_STRIP);
		glVertex3d(0,0.1,-0.2);
		glVertex3d(0,0.1,0.2);
		glVertex3d(0,-0.1,0.2);
		glVertex3d(0,-0.1,-0.2);
		glVertex3d(0,0.1,-0.2);
		glVertex3d(0,-0.1,0.2);
		glVertex3d(0,0.1,0.2);
		glVertex3d(0,-0.1,-0.2);
		glEnd();
		glBegin(GL_LINES);
		glVertex3d(0,0,-0.25);
		glVertex3d(0,0,-0.2);
		glVertex3d(0,0,0.25);
		glVertex3d(0,0,0.2);
		glEnd();
		break;
	case 1:
		glBegin(GL_LINE_STRIP);
		glVertex3d(0,0.1,-0.15);
		glVertex3d(0,0.05,-0.15);
		glVertex3d(0,0.05,0.15);
		glVertex3d(0,0.1,0.15);
		glEnd();
		glBegin(GL_LINE_STRIP);
		glVertex3d(0,-0.1,-0.15);
		glVertex3d(0,-0.05,-0.15);
		glVertex3d(0,-0.05,0.15);
		glVertex3d(0,-0.1,0.15);
		glEnd();
		glBegin(GL_LINES);
		glVertex3d(0,0,-0.25);
		glVertex3d(0,0,0.25);
		glVertex3d(0,0.05,-0.2);
		glVertex3d(0,-0.05,-0.2);
		glVertex3d(0,0.05,0.2);
		glVertex3d(0,-0.05,0.2);
		glEnd();
		break;
	case 2:
		glBegin(GL_LINE_STRIP);
		glVertex3d(0.0, 0.05,   0.05);
		glVertex3d(0.0, 0.1,  0.0);
		glVertex3d(0.0, 0.0,    -0.1);
		glVertex3d(0.0, 0.0,    -0.2);
		glVertex3d(0.0, 0.0,    -0.1);
		glVertex3d(0.0, -0.1, 0.0);
		glVertex3d(0.0, -0.05,  0.05);
		glEnd();
		break;
	}

  if (num == 0)
  {
    glBegin(GL_LINES);
    glVertex3d(0.0, 0.0, -0.2);
    glVertex3d(0.0, 0.0, -0.4);
    glEnd();

    for (double d = 0.025; d < 0.151; d += 0.025)
    {
      glBegin(GL_LINES);
      glVertex3d(0.0, 0.0, -0.2 - d);
      glVertex3d(0.0, -0.05, -0.250 - d);
      glEnd();
    }
  }
}

void CRobotView::DrawCoordSystem(bool selected) const
{
  float alpha = selected ? 1.0f : 0.7f;

  //X-axis
	glColor4f(alpha, 0, 0, alpha);
  glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
  DrawOrt();
  glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);

	//Y-axis
	glColor4f(0, alpha, 0, alpha);
  glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
  DrawOrt();
  glRotatef(-90.0f, -1.0f, 0.0f, 0.0f);
  
  //Z-axis
	glColor4f(0, 0, alpha, alpha);
  DrawOrt();
  
  //Return black color
	glColor4f(0, 0, 0, 1.0f);
}

void CRobotView::DrawOrt() const //by_z
{
  glPushMatrix();

  glBegin(GL_LINES);
  glVertex3d(0.0, 0.0, 0.0);
  glVertex3d(0.0, 0.0, 0.3);
  glEnd();

  glTranslated(0.0, 0.0, 0.3);
  DrawArrow();

  glPopMatrix();
}

void CRobotView::DrawArrow() const
{
  glPushMatrix();
  for (int x = 0; x < 8; ++ x)
  {
    glRotated(45.0, 0.0, 0.0, 1.0);

    glBegin(GL_POLYGON);

    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(0.0, 0.020, 0.0);
    glVertex3d(0.0, 0.0, 0.080);
    glVertex3d(0.0, -0.020, 0.0);
    glVertex3d(0.0, 0.0, 0.0);

    glEnd();
  }
  glPopMatrix();
}

void CRobotView::RenderCoordSystems()
{
	glPushMatrix();
	InitialModelView();
	for (Size c=0;c<pRCSs->pCSs.size();c++)
	{
		if (c>0)
		{
			int pm=core().data().LDs[core().link(c-1).Type].Motion;
			switch (pm)
			{
			case 0:
				glTranslatef(0,0,core().link(c-1).m_curd);
				break;
			case 1:
				glRotatef(core().link(c-1).m_curd,0,0,1);
				break;
			}
		}
		glMultMatrixf(pTMs->pTMs[c].t().pData);
		DrawCoordSystem(c == pRD->sel_index());
	}
	glPopMatrix();
}

void CRobotView::InitialModelView()
{
	glLoadIdentity();
	glTranslated(sTrans[0],sTrans[1],sTrans[2]);
}

void CRobotView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	
	CView::OnLButtonDown(nFlags, point);
}

void CRobotView::OnDestroy()
{
  CView::OnDestroy();
  delete m_dlgs.kin_dir;
}

void CRobotView::on_update(int power)
{
  if (power > modify::none)
    update_dlgs(power);
  Draw();
}

void CRobotView::update_dlgs(int power)
{
  if (m_dlgs.kin_dir_shown)
    m_dlgs.kin_dir->update(power);
  m_dlgs.kin_dir->ShowWindow(m_dlgs.kin_dir_shown ? SW_SHOW : SW_HIDE);
  
  if (m_dlgs.pos_shown)
    m_dlgs.pos->update(power);
  m_dlgs.pos->ShowWindow(m_dlgs.pos ? SW_SHOW : SW_HIDE);
}

DWORD CRobotView::cur_time() const
{
  SYSTEMTIME sys_time;
  GetSystemTime(&sys_time);
  
  FILETIME file_time;
  SystemTimeToFileTime(&sys_time, &file_time);

  return file_time.dwLowDateTime;
}

void CRobotView::on_tick()
{
  DWORD now_time = cur_time();
  if (m_playing)
  {
    
    DWORD time_det = now_time - m_old_time;
    m_time += float(time_det / 10000) * 0.001f;

    Vars vars;
    const TrajRecord& traj = core().traj_record();
    traj.vars_in_time(m_time, vars);
    core().apply_vars(vars);
    Draw();

    if (m_time > traj.finish_time())
      m_playing = false;
  }

  m_old_time = now_time;
}

//////////////////////////////////////////////////////////////////////////
//CMD UI
void CRobotView::OnAnlsKinDir()
{
  m_dlgs.kin_dir_shown = !m_dlgs.kin_dir_shown;
  update_dlgs(modify::big);
}

void CRobotView::OnUpdateAnlsKinDir(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(core().manipulator_ready());
  pCmdUI->SetCheck(m_dlgs.kin_dir_shown ? 1 : 0);
}

void CRobotView::OnAnlsKinRev()
{
  CRevDlg dlg;
  dlg.DoModal();
}

void CRobotView::OnUpdateAnlsKinRev(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(core().manipulator_ready());
}

void CRobotView::OnAnlsTrajectory()
{
  CTrajDlg dlg;
  dlg.DoModal();
}

void CRobotView::OnUpdateAnlsTrajectory(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(core().manipulator_ready());
}

void CRobotView::OnAnlsMatrInert()
{
  CInertMatrDlg dlg;
  dlg.DoModal();
}

void CRobotView::OnCtrlDoTraj()
{
  m_time = core().traj_record().start_time();
  m_playing = true;
}

void CRobotView::OnUpdateCtrlDoTraj(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(core().traj_record().traj_ready());
}

void CRobotView::OnAnlsDynRev()
{
  CDinRevDlg dlg;
  dlg.DoModal();
}

void CRobotView::OnUpdateAnlsAxis(CCmdUI *pCmdUI)
{
  pCmdUI->SetCheck(pRD->S.aAxis ? 1 : 0);
}

void CRobotView::OnUpdateAnlsDerMatrices(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(core().manipulator_ready());
}

void CRobotView::OnUpdateAnlsMatrInert(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(core().manipulator_ready());
}

void CRobotView::OnUpdateAnlsMatrices(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(core().manipulator_ready());
}

void CRobotView::OnUpdateAnlsDynRev(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(core().manipulator_ready());
}

void CRobotView::OnAnlsEnergy()
{
  CEnergyDlg dlg;
  dlg.DoModal();
}

void CRobotView::OnUpdateAnlsEnergy(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(core().manipulator_ready());
}

//end
//////////////////////////////////////////////////////////////////////////