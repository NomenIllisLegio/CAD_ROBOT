// ValueDlg.cpp : implementation file
//

#include "stdafx.h"
#include "robot.h"
#include "ValueDlg.h"

// CValueDlg dialog

IMPLEMENT_DYNAMIC(CValueDlg, CDialog)
CValueDlg::CValueDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CValueDlg::IDD, pParent)
  , m_value(_T(""))
{
}

CValueDlg::~CValueDlg()
{
}

void CValueDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_EDIT1, m_value);
  DDX_Control(pDX, IDC_EDIT1, m_value_c);
}


BEGIN_MESSAGE_MAP(CValueDlg, CDialog)
END_MESSAGE_MAP()


// CValueDlg message handlers

BOOL CValueDlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  m_value = data;
  UpdateData(false);
  m_value_c.SetSel(0, 255);

  return TRUE;
}

void CValueDlg::OnOK()
{
  UpdateData();
  data = m_value;
  CDialog::OnOK();
}
