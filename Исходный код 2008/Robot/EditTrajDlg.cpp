// EditTrajDlg.cpp : implementation file
//

#include "stdafx.h"
#include "robot.h"
#include "EditTrajDlg.h"
#include ".\edittrajdlg.h"


// CEditTrajDlg dialog

IMPLEMENT_DYNAMIC(CEditTrajDlg, CDialog)
CEditTrajDlg::CEditTrajDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditTrajDlg::IDD, pParent)
  , m_x(0)
  , m_y(0)
  , m_z(0)
  , m_time(0)
{
}

CEditTrajDlg::~CEditTrajDlg()
{
}

void CEditTrajDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_X, m_x);
  DDX_Text(pDX, IDC_Y, m_y);
  DDX_Text(pDX, IDC_Z, m_z);
  DDX_Text(pDX, IDC_TIME, m_time);
}


BEGIN_MESSAGE_MAP(CEditTrajDlg, CDialog)
END_MESSAGE_MAP()


// CEditTrajDlg message handlers

BOOL CEditTrajDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_time = traj_point->time;
  m_x = traj_point->x;
  m_y = traj_point->y;
  m_z = traj_point->z;

  UpdateData(FALSE);

  return TRUE;
}

void CEditTrajDlg::OnOK()
{
  UpdateData(TRUE);

  traj_point->time = m_time;
  traj_point->x = m_x;
  traj_point->y = m_y;
  traj_point->z = m_z;

  CDialog::OnOK();
}
