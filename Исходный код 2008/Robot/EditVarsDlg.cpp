// EditVarsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "robot.h"
#include "EditVarsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditVarsDlg dialog


CEditVarsDlg::CEditVarsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditVarsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditVarsDlg)
	m_pen = 0.0f;
	m_val = 0.0f;
	//}}AFX_DATA_INIT
}


void CEditVarsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditVarsDlg)
	DDX_Text(pDX, IDC_PEN, m_pen);
	DDX_Text(pDX, IDC_VALUE, m_val);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditVarsDlg, CDialog)
	//{{AFX_MSG_MAP(CEditVarsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditVarsDlg message handlers

void CEditVarsDlg::OnOK() 
{
	UpdateData(true);
	val=m_val;
	pen=m_pen;
	CDialog::OnOK();
}

BOOL CEditVarsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_val=val;
	m_pen=pen;
	UpdateData(false);
	return TRUE;
}
