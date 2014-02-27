// OptDlg.cpp : implementation file
//

#include "stdafx.h"
#include "robot.h"
#include "OptDlg.h"
#include ".\optdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptDlg dialog

extern CRobotDoc * pRD;

COptDlg::COptDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptDlg::IDD, pParent)
  , m_x(0)
  , m_y(0)
  , m_z(0)
{
	//{{AFX_DATA_INIT(COptDlg)
	//}}AFX_DATA_INIT
}


void COptDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(COptDlg)
  DDX_Text(pDX, IDC_MOT_STP, m_mot);
  DDV_MinMaxFloat(pDX, m_mot, 0.f, 1000.f);
  DDX_Text(pDX, IDC_ROT_STP, m_rot);
  DDV_MinMaxFloat(pDX, m_rot, 0.f, 180.f);
  DDX_Check(pDX, IDC_OPT_PARS_BORD, m_bord);
  DDX_Check(pDX, IDC_OPT_PARS_USE, m_use);
  DDX_Text(pDX, IDC_KR_PREC, m_kr_pr);
  DDX_Text(pDX, IDC_KR_DIVIDER, m_kr_div);
  DDV_MinMaxInt(pDX, m_kr_div, 1, 100);
  DDX_Check(pDX, IDC_KR_USE_PROBES, m_kr_use);
  //}}AFX_DATA_MAP
  DDX_Text(pDX, IDC_GRAV_X, m_x);
  DDX_Text(pDX, IDC_GRAV_Y, m_y);
  DDX_Text(pDX, IDC_GRAV_Z, m_z);
}


BEGIN_MESSAGE_MAP(COptDlg, CDialog)
	//{{AFX_MSG_MAP(COptDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptDlg message handlers

void COptDlg::OnOK() 
{
	UpdateData();
	core().options().cMotStp=m_mot;
	core().options().cRotStp=m_rot;
	core().options().aBorders=m_bord != 0;
	core().options().aUseNumbers=m_use != 0;
	core().options().drE=m_kr_pr;
	core().options().drProbes=m_kr_div;
	core().options().drUse=(m_kr_use!=0);

  core().options().grV[0] = m_x;
  core().options().grV[1] = m_y;
  core().options().grV[2] = m_z;
  
	CDialog::OnOK();
}

BOOL COptDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

	m_mot = core().options().cMotStp;
	m_rot = core().options().cRotStp;
	m_bord = core().options().aBorders;
	m_use = core().options().aUseNumbers;
	m_kr_pr = core().options().drE;
	m_kr_use = core().options().drUse;
	m_kr_div = core().options().drProbes;

  m_x = core().options().grV[0];
  m_y = core().options().grV[1];
  m_z = core().options().grV[2];

  UpdateData(FALSE);
  return TRUE;
}
