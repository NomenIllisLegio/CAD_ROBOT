// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "robot.h"

#include "MainFrm.h"
#include "SelLinkDlg.h"
#include "EditLinkDlg.h"
#include "FormulaDlg.h"
#include "ParamsDlg.h"
#include "OptDlg.h"
#include "MatrDlg.h"
#include "KinDirDlg.h"
#include "KinRevDlg.h"
#include "SelectDlg.h"
#include "DynDirDlg.h"
//#include "DynSpdDlg.h."
#include "DerMatrDlg.h"
#include ".\mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

extern CRobotDoc * pRD;
extern CRobotView * pRV;
extern CMainFrame * pMF;

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_EDIT_ADD, OnAddLink)
	ON_COMMAND(ID_VIEW_MODES_2D, OnViewModes2d)
	ON_COMMAND(ID_VIEW_MODES_3D, OnViewModes3d)
	ON_COMMAND(ID_EDIT_DEL, OnDelLink)
	ON_COMMAND(ID_EDIT_PROPS, OnPropsLink)
	ON_COMMAND(ID_MAST_LESS, OnMastLess)
	ON_COMMAND(ID_MAST_MORE, OnMastMore)
	ON_COMMAND(ID_MAST_NORM, OnMastNorm)
	ON_COMMAND(ID_VIEW_CENTER, OnViewCenter)
	ON_COMMAND(ID_EDIT_FORMULA, OnEditFormula)
	ON_COMMAND(ID_ANLS_PARAMS, OnAnlsParams)
	ON_COMMAND(ID_ANLS_OPTIONS, OnAnlsOptions)
	ON_COMMAND(ID_ANLS_MATRICES, OnAnlsMatrices)
	ON_COMMAND(ID_ANLS_KIN_DIR, OnAnlsKinDir)
//	ON_COMMAND(ID_ANLS_KIN_REV, OnAnlsKinRev)
	ON_COMMAND(ID_VIEW_REFRESH, OnViewRefresh)
	ON_COMMAND(ID_EDIT_SELECT, OnEditSelect)
	ON_COMMAND(ID_CTRL_MOVE_DEFAULT, OnCtrlMoveDefault)
	ON_COMMAND(ID_CTRL_FORV, OnCtrlForv)
	ON_COMMAND(ID_CTRL_BACK, OnCtrlBack)
	ON_COMMAND(ID_EDIT_PREV, OnEditPrev)
	ON_COMMAND(ID_EDIT_NEXT, OnEditNext)
	ON_COMMAND(ID_ANLS_AXIS, OnAnlsAxis)
	//}}AFX_MSG_MAP
	// Global help commands
	ON_COMMAND(ID_HELP_FINDER, CFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP, CFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_ANLS_DER_MATRICES, OnAnlsDerMatrices)
	ON_UPDATE_COMMAND_UI(ID_ANLS_PARAMS, OnUpdateAnlsParams)
  ON_UPDATE_COMMAND_UI(ID_VIEW_MODES_3D, OnUpdateViewModes3d)
  ON_UPDATE_COMMAND_UI(ID_EDIT_ADD, OnUpdateEditAdd)
  ON_UPDATE_COMMAND_UI(ID_EDIT_DEL, OnUpdateEditDel)
  ON_UPDATE_COMMAND_UI(ID_EDIT_NEXT, OnUpdateEditNext)
  ON_UPDATE_COMMAND_UI(ID_EDIT_PREV, OnUpdateEditPrev)
  ON_UPDATE_COMMAND_UI(ID_EDIT_PROPS, OnUpdateEditProps)
  ON_UPDATE_COMMAND_UI(ID_VIEW_CENTER, OnUpdateViewCenter)
  ON_UPDATE_COMMAND_UI(ID_CTRL_BACK, OnUpdateCtrlBack)
  ON_UPDATE_COMMAND_UI(ID_CTRL_FORV, OnUpdateCtrlForv)
  ON_UPDATE_COMMAND_UI(ID_CTRL_MOVE_DEFAULT, OnUpdateCtrlMoveDefault)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	pMF=this;
}

CMainFrame::~CMainFrame()
{
	ReleaseResources();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	//standard toolbar
	if (!m_wndStdTB.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndStdTB.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndStdTB.EnableDocking(CBRS_ALIGN_ANY);
	m_wndStdTB.SetWindowText("Стандартная");
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndStdTB);

	//construct toolbar
	if (!m_wndConstructTB.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndConstructTB.LoadToolBar(IDR_CONSTRUCT))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndConstructTB.EnableDocking(CBRS_ALIGN_ANY);
	m_wndConstructTB.SetWindowText("Конструирование");
	//RECT r={22,0,23,0};
	//m_wndStdTB.GetClientRect(&r);
	DockControlBar(&m_wndConstructTB,AFX_IDW_DOCKBAR_TOP);

	//status bar
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	
	//resources
	UploadResources();

	//defaults;
	UpdateFrameByDoc();
	/*
	CMenu * pM=GetMenu();
	pM->CheckMenuRadioItem(ID_VIEW_MODES_2D,ID_VIEW_MODES_3D,ID_VIEW_MODES_2D,MF_BYCOMMAND);
*/
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnAddLink() 
{
	CSelLinkDlg sld;
	if (sld.DoModal()==IDCANCEL) return;
	int Link=core().AddLink(-1,sld.iType);
	pRD->on_modify(modify::big);
	EditLink(Link);
	pRV->Draw();
}

bool CMainFrame::UploadResources()
{
	//done
	return true;
}

void CMainFrame::ReleaseResources()
{
}

void CMainFrame::OnViewModes2d() 
{
	pRD->S.Mode=0;
	UpdateFrameByDoc();
}

void CMainFrame::OnViewModes3d() 
{
	pRD->S.Mode=1;
	UpdateFrameByDoc();
}


void CMainFrame::EditLink(int Index)
{
	CEditLinkDlg eld;
	eld.link=Index;
	eld.DoModal();
}

void CMainFrame::OnDelLink() 
{
	if (!pRD->selection_valid())
		return;
	core().DelLink(pRD->H.SelLink);
	//pRD->H.SelLink=-1;
	pRD->on_modify(modify::big);
  //pRV->Draw();
}

void CMainFrame::OnPropsLink() 
{
	if (pRD->H.SelLink==-1)
		return;
	EditLink(pRD->H.SelLink);
	pRV->Draw();
}

void CMainFrame::OnMastLess() 
{
	(pRD->S.Scale*=3)/=2;
	pRV->Draw();
}

void CMainFrame::OnMastMore() 
{
	(pRD->S.Scale*=2)/=3;
	pRV->Draw();
}

void CMainFrame::OnMastNorm() 
{
	pRD->S.Scale=3000;
	pRV->Draw();
}

void CMainFrame::OnViewCenter() 
{
	pRD->S.Center=!pRD->S.Center;
	UpdateFrameByDoc();
}

void CMainFrame::UpdateFrameByDoc()
{
	CMenu * pM=GetMenu();

	//Center
	pM->CheckMenuItem(ID_VIEW_CENTER,(pRD->S.Center)?MF_CHECKED:MF_UNCHECKED);
	
	//Axis
	pM->CheckMenuItem(ID_ANLS_AXIS,(pRD->S.aAxis)?MF_CHECKED:MF_UNCHECKED);
	//Mode
	pM->CheckMenuRadioItem(ID_VIEW_MODES_2D,ID_VIEW_MODES_3D,(pRD->S.Mode==0)?ID_VIEW_MODES_2D:ID_VIEW_MODES_3D,MF_BYCOMMAND);
	
	//Add/Del/Props
	pM->EnableMenuItem(ID_EDIT_DEL,(core().link_count()>0)?MF_ENABLED:MF_GRAYED|MF_DISABLED);
	pM->EnableMenuItem(ID_EDIT_PROPS,(core().link_count()>0)?MF_ENABLED:MF_GRAYED|MF_DISABLED);

	pRD->UpdateAllViews(NULL);	
}

void CMainFrame::OnEditFormula() 
{
	CFormulaDlg dlg;
	dlg.pRD=pRD;
	dlg.DoModal();
	UpdateFrameByDoc();
}

void CMainFrame::OnAnlsParams() 
{
	CParamsDlg dlg;
	dlg.DoModal();
}

void CMainFrame::OnAnlsOptions() 
{
	COptDlg dlg;
	dlg.DoModal();
}

void CMainFrame::OnAnlsMatrices() 
{
	CMatrDlg dlg;
	dlg.DoModal();
}

void CMainFrame::OnAnlsKinDir() 
{
	CKinDirDlg dlg;
	dlg.DoModal();	
}

//void CMainFrame::OnAnlsKinRev() 
//{
//	CKinRevDlg dlg;
//	dlg.DoModal();
//}

void CMainFrame::OnViewRefresh() 
{
    pRV->Draw();
}

void CMainFrame::OnEditSelect() 
{
	CSelectDlg dlg;
	dlg.Link=pRD->H.SelLink;
	if (dlg.DoModal()==IDOK)
	{
		pRD->H.SelLink=dlg.Link;
		pRV->Draw();
	}
}

void CMainFrame::OnCtrlMoveDefault() 
{
	for (int l=0;l<core().link_count();l++)
		core().link(l).m_curd=0;
	//pRV->Draw();
  pRD->on_modify(modify::light);
}

void CMainFrame::OnCtrlForv() 
{
	if (!pRD->selection_valid())
    return;
  int mt=core().data().LDs[pRD->selected_link().Type].Motion;
	if (mt==-1)
		return;
	
	float &d = pRD->selected_link().m_curd;

	d+=(mt==0)?core().options().cMotStp:core().options().cRotStp;
	SRParamsTable * pPT = core().rParamsTable();
	SRParamsTable::SVars & v = pPT->pVs[pRD->H.SelLink];
	if (d>v.max-v.def) d=v.max-v.def;
  pRD->on_modify(modify::light);
  //pRV->Draw();
}

void CMainFrame::OnCtrlBack() 
{
	if (!pRD->selection_valid())
    return;
  int mt=core().data().LDs[pRD->selected_link().Type].Motion;
	if (mt==-1)
		return;
	
	float &d = pRD->selected_link().m_curd;

	d-=(mt==0)?core().options().cMotStp:core().options().cRotStp;
	SRParamsTable * pPT = core().rParamsTable();
	SRParamsTable::SVars & v = pPT->pVs[pRD->H.SelLink];
	if (d<v.min-v.def) d=v.min-v.def;
	//pRV->Draw();
  pRD->on_modify(modify::light);
}

void CMainFrame::OnEditPrev() 
{
	if (pRD->selection_valid())
  {
	  if (pRD->H.SelLink>0)
		  -- pRD->H.SelLink;
  }
  else
  {
    pRD->select_link(core().link_count() - 1);
  }
	pRV->Draw();
}

void CMainFrame::OnEditNext() 
{
	if (pRD->selection_valid())
  {
    if (pRD->H.SelLink< int(core().link_count()) - 1)
		  ++ pRD->H.SelLink;
  }
  else
    pRD->select_link(0);

	pRV->Draw();
}

void CMainFrame::OnAnlsAxis() 
{
	pRD->S.aAxis=!pRD->S.aAxis;
	UpdateFrameByDoc();
}

void CMainFrame::OnAnlsDerMatrices()
{
	CDerMatrDlg dlg;
	dlg.DoModal();
}

void CMainFrame::OnUpdateAnlsParams(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(core().manipulator_ready());
}

void CMainFrame::OnUpdateViewModes3d(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateEditAdd(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(core().attach_avaiable());
}

void CMainFrame::OnUpdateEditDel(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(pRD->selection_valid());
}

void CMainFrame::OnUpdateEditNext(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(pRD->sel_index() < core().link_count() - 1 || (!pRD->selection_valid() && core().link_count() > 0));
}

void CMainFrame::OnUpdateEditPrev(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(pRD->sel_index() > 0 || (!pRD->selection_valid() && core().link_count() > 0));
}

void CMainFrame::OnUpdateEditProps(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(pRD->selection_valid());
}

void CMainFrame::OnUpdateViewCenter(CCmdUI *pCmdUI)
{
  pCmdUI->SetCheck(pRD->S.Center ? 1 : 0);
}

void CMainFrame::OnUpdateCtrlBack(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(pRD->selection_valid() && pRD->selected_link().Type != 2);
}

void CMainFrame::OnUpdateCtrlForv(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(pRD->selection_valid() && pRD->selected_link().Type != 2);
}

void CMainFrame::OnUpdateCtrlMoveDefault(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(core().link_count() > 0);
}
