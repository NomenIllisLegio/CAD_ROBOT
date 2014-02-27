// DinRevDlg.cpp : implementation file
//

#include "stdafx.h"
#include "robot.h"
#include "DinRevDlg.h"
#include ".\dinrevdlg.h"


// CDinRevDlg dialog

IMPLEMENT_DYNAMIC(CDinRevDlg, CDialog)
CDinRevDlg::CDinRevDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDinRevDlg::IDD, pParent)
{
}

CDinRevDlg::~CDinRevDlg()
{
}

void CDinRevDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_POWERS, m_powers);
}


BEGIN_MESSAGE_MAP(CDinRevDlg, CDialog)
END_MESSAGE_MAP()


// CDinRevDlg message handlers

BOOL CDinRevDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_powers.InsertColumn(0, "Звено", LVCFMT_CENTER, 90);
  m_powers.InsertColumn(1, "Координата", LVCFMT_CENTER, 100);
  m_powers.InsertColumn(2, "Сила (Qi)", LVCFMT_CENTER, 100);

  //Заполнение
  MMatrix<float> Q;
  Vars vars;
  core().enum_all_vars(vars);
  
  core().drRevDyn(vars, Q);

  CString text;
  for (int x = 0; x < core().link_count() - 1; ++ x)
  {
    m_powers.InsertItem(x, core().link(x + 1).Name.c_str(), -1);
    m_powers.SetItemText(x, 1, core().var_by_number(x).GetName().c_str());
    
    text.Format("%g", Q(x));
    m_powers.SetItemText(x, 2, text);
  }

  return TRUE;
}
