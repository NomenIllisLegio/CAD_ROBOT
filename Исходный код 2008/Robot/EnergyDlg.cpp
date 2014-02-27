// EnergyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "robot.h"
#include "EnergyDlg.h"



// CEnergyDlg dialog

IMPLEMENT_DYNAMIC(CEnergyDlg, CDialog)
CEnergyDlg::CEnergyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEnergyDlg::IDD, pParent)
{
}

CEnergyDlg::~CEnergyDlg()
{
}

void CEnergyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_POWERS, m_eners);
}


BEGIN_MESSAGE_MAP(CEnergyDlg, CDialog)
END_MESSAGE_MAP()


// CEnergyDlg message handlers

BOOL CEnergyDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_eners.InsertColumn(0, "Звено", LVCFMT_CENTER, 90);
  m_eners.InsertColumn(1, "Кинетическая", LVCFMT_CENTER, 100);
  m_eners.InsertColumn(2, "Потенциальная", LVCFMT_CENTER, 100);

  //Заполнение
  MMatrix<float> K, P;
  Vars vars;
  core().enum_all_vars(vars);
  
  core().drKinEnerg(vars, K);
  core().drPotEnerg(vars, P);

  CString text;
  float s_kin = 0, s_pot = 0;
  int q = core().link_count() - 1;
  for (int x = 0; x < q; ++ x)
  {
    m_eners.InsertItem(x, core().link(x + 1).Name.c_str(), -1);
    text.Format("%g", K(x));
    m_eners.SetItemText(x, 1, text);
    
    text.Format("%g", P(x));
    m_eners.SetItemText(x, 2, text);

    s_kin += K(x);
    s_pot += P(x);
  }

  m_eners.InsertItem(q, "Итого:", -1);
  text.Format("%g", s_kin);
  m_eners.SetItemText(q, 1, text);
  text.Format("%g", s_pot);
  m_eners.SetItemText(q, 2, text);

  m_eners.InsertItem(q + 1, "Полная:", -1);
  text.Format("%g", s_kin + s_pot);
  m_eners.SetItemText(q + 1, 1, text);

  return TRUE;
}