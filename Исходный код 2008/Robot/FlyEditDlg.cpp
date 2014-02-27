// FlyEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "robot.h"
#include "FlyEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFlyEditDlg dialog


CFlyEditDlg::CFlyEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFlyEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFlyEditDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFlyEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFlyEditDlg)
	DDX_Control(pDX, IDC_EDIT, m_edit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFlyEditDlg, CDialog)
	//{{AFX_MSG_MAP(CFlyEditDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlyEditDlg message handlers

#define Thin 3

BOOL CFlyEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetWindowPos(pPW,rect.left-Thin,rect.top-Thin,rect.right-rect.left+Thin*2,rect.bottom-rect.top+Thin*2,0);	
	CRect r;
	GetClientRect(&r);
	m_edit.SetWindowPos(this,r.left,r.top,r.right-r.left,r.bottom-r.top,0);
	m_edit.SetWindowText(Data);
	m_edit.SetSel(0,1000);
	return TRUE;
}

void CFlyEditDlg::OnOK() 
{
	m_edit.GetWindowText(Data);	
	CDialog::OnOK();
}
