// SelectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "robot.h"
#include "SelectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectDlg dialog

extern CRobotDoc * pRD;

CSelectDlg::CSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectDlg)
	DDX_Control(pDX, IDC_LINK_LIST, m_llc);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectDlg)
	ON_LBN_DBLCLK(IDC_LINK_LIST, OnDblclkLinkList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectDlg message handlers

BOOL CSelectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	for (int l=0;l<core().link_count();l++)
		m_llc.AddString(core().link(l).Name.c_str());
	return TRUE;
}

void CSelectDlg::OnOK() 
{
	Link=-1;
	for (int l=0;l<core().link_count();l++)
		if (m_llc.GetSel(l)!=0)
			Link=l;	
	CDialog::OnOK();
}

void CSelectDlg::OnDblclkLinkList() 
{
	OnOK();	
}
