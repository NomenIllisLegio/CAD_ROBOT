// ChangeGivenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "robot.h"
#include "ChangeGivenDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChangeGivenDlg dialog

//extern CString MLs[12];
extern CRobotDoc * pRD;

CChangeGivenDlg::CChangeGivenDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChangeGivenDlg::IDD, pParent)
  , m_type(0)
{
	//{{AFX_DATA_INIT(CChangeGivenDlg)
	m_iv = -1;
	m_pv = -1;
	m_sv = -1;
	m_pen = 0.0f;
	m_val = 0.0f;
	//}}AFX_DATA_INIT
}


void CChangeGivenDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CChangeGivenDlg)
  DDX_Control(pDX, IDC_PARAM, m_param);
  DDX_Control(pDX, IDC_SYST, m_syst);
  DDX_Control(pDX, IDC_INSYST, m_insyst);
  DDX_CBIndex(pDX, IDC_INSYST, m_iv);
  DDX_CBIndex(pDX, IDC_PARAM, m_pv);
  DDX_CBIndex(pDX, IDC_SYST, m_sv);
  DDX_Text(pDX, IDC_PEN, m_pen);
  DDX_Text(pDX, IDC_VAL, m_val);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_TYPE, m_type_c);
  DDX_CBIndex(pDX, IDC_TYPE, m_type);
}


BEGIN_MESSAGE_MAP(CChangeGivenDlg, CDialog)
	//{{AFX_MSG_MAP(CChangeGivenDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChangeGivenDlg message handlers

BOOL CChangeGivenDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
  ////fillings	
	for (Size i=0;i<12;i++) m_param.AddString(core().data().MLs[i].c_str());
	
	int lq=core().link_count();
	m_syst.AddString("Инерциальная");
	for (int l=1;l<core().link_count();l++)
		m_syst.AddString(core().link(l).Name.c_str());
	m_insyst.AddString("Cтатической");
	if (lq>0) m_insyst.AddString("Инерциальной");
	for (int l=1;l<core().link_count()-1;l++)
		m_insyst.AddString(core().link(l).Name.c_str());

  for (int l = 0; l < 3; ++l)
    m_type_c.AddString(core().data().PTs[l].c_str());

	m_sv = arg.end;
	m_iv = arg.start + 1;
	m_pv = arg.member;
	m_val = arg.value;
	m_pen = arg.penalty;
  m_type = arg.mean;
	
	UpdateData(false);
	return TRUE;
}

void CChangeGivenDlg::OnOK() 
{
	UpdateData();

	arg.end = m_sv;
	arg.start = m_iv - 1;
	arg.member = m_pv;
	arg.value = m_val;
	arg.penalty = m_pen;
  arg.mean = m_type;

	CDialog::OnOK();
}
