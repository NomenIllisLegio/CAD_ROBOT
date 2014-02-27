// EditPosDlg.cpp : implementation file
//

#include "stdafx.h"
#include "robot.h"
#include "EditPosDlg.h"
#include ".\editposdlg.h"


// CEditPosDlg dialog

IMPLEMENT_DYNAMIC(CEditPosDlg, CDialog)
CEditPosDlg::CEditPosDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditPosDlg::IDD, pParent)
  , m_pos(0)
  , m_vel(0)
  , m_acc(0)
{
}

CEditPosDlg::~CEditPosDlg()
{
}

void CEditPosDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_POS, m_pos);
  DDX_Text(pDX, IDC_VEL, m_vel);
  DDX_Text(pDX, IDC_ACC, m_acc);
}


BEGIN_MESSAGE_MAP(CEditPosDlg, CDialog)
END_MESSAGE_MAP()


// CEditPosDlg message handlers

BOOL CEditPosDlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  
  m_pos = pos;
  m_vel = vel;
  m_acc = acc;
  UpdateData(false);
  
  return TRUE;
}

void CEditPosDlg::OnOK()
{
  UpdateData(TRUE);
  pos = m_pos;
  vel = m_vel;
  acc = m_acc;

  CDialog::OnOK();
}
