// RevDlg.cpp : implementation file
//

#include "stdafx.h"
#include "robot.h"
#include "RevDlg.h"
#include ".\revdlg.h"
#include "ChangeGivenDlg.h"
#include "WideView.h"

extern CRobotDoc* pRD;

// CRevDlg dialog

IMPLEMENT_DYNAMIC(CRevDlg, CDialog)
CRevDlg::CRevDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRevDlg::IDD, pParent)
{
}

CRevDlg::~CRevDlg()
{
}

void CRevDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_GIVEN, m_given);
  DDX_Control(pDX, IDC_PROGRESS, m_progress);
  DDX_Control(pDX, IDC_INFO, m_info);
  DDX_Control(pDX, IDC_MOVE, m_move);
  DDX_Control(pDX, IDC_SAVE, m_save);
}


BEGIN_MESSAGE_MAP(CRevDlg, CDialog)
  ON_BN_CLICKED(IDC_POSITION, OnBnClickedPosition)
  ON_BN_CLICKED(IDC_EDIT, OnBnClickedEdit)
  ON_BN_CLICKED(IDC_REMOVE, OnBnClickedRemove)
  ON_NOTIFY(NM_DBLCLK, IDC_GIVEN, OnNMDblclkGiven)
  ON_BN_CLICKED(IDC_SOLVE, OnBnClickedSolve)
  ON_BN_CLICKED(IDC_MOVE, OnBnClickedMove)
  ON_BN_CLICKED(IDC_VIEW, OnBnClickedView)
END_MESSAGE_MAP()


// CRevDlg message handlers

BOOL CRevDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_given.InsertColumn(0, "Номер", LVCFMT_CENTER, 45);
  m_given.InsertColumn(1, "Значение", LVCFMT_LEFT, 545);

  on_change_given();

  return TRUE;
}


void CRevDlg::redraw_given()
{
  m_given.DeleteAllItems();
  CString text;
  
  for (int g = 0; g < core().rev_record().arg_count(); ++ g)
  {
    const RevArg& arg = core().rev_record().arg(g);
    text.Format("%i", g + 1);
    m_given.InsertItem(g, text, -1);
    m_given.SetItemText(g, 1, arg.string_mean().c_str());
  }
}

int CRevDlg::selected_given() const
{
  POSITION pos = m_given.GetFirstSelectedItemPosition();
  if (!pos)
    return -1;
  else
    return m_given.GetNextSelectedItem(pos);
}

void CRevDlg::form_given()
{
  if (m_given_actual)
    return;
  m_data.clear();
  for (int g = 0; g < core().rev_record().arg_count(); ++ g)
  {
    core().rev_record().arg(g).apply_to_data(m_data);
  }

  Vars vars;
  core().enum_all_vars(vars);
  for (Vars::iterator it = vars.begin();
       it != vars.end();
       ++ it)
  {
    if (m_data.func.IsVarPresent(it->var))
      m_data.vars.push_back(*it);
  }
  m_given_actual = true;
}

void CRevDlg::OnBnClickedPosition()
{
  RevArg arg;
  arg.type = RevArg::rat_pos;
  arg.end = core().link_count() - 1;
  arg.mean = 0;
  arg.member = 3;
  arg.penalty = 1.0f;
  arg.start = 0;
  arg.value = 0.0f;

  CChangeGivenDlg dlg;
  dlg.arg = arg;
  if (dlg.DoModal() != IDOK)
    return;

  core().rev_record().add_arg(dlg.arg);
  on_change_given();
}

void CRevDlg::OnBnClickedEdit()
{
  int pos = selected_given();
  if (pos == -1)
    return;
  CChangeGivenDlg dlg;
  dlg.arg = core().rev_record().arg(pos);
  if (dlg.DoModal() != IDOK)
    return;
  core().rev_record().arg(pos) = dlg.arg;
  on_change_given();
}

void CRevDlg::OnBnClickedRemove()
{
  int pos = selected_given();
  if (pos == -1)
    return;
  core().rev_record().del_arg(pos);
  on_change_given();
}

void CRevDlg::OnNMDblclkGiven(NMHDR *pNMHDR, LRESULT *pResult)
{
  OnBnClickedEdit();
  *pResult = 0;
}

void CRevDlg::OnBnClickedSolve()
{
  form_given();
  m_data.init();
  
  m_info.SetWindowText("Вычисляется");

  float max_e = m_data.func.solve(m_data.given);
  m_progress.SetRange32(0, int(max_e));
  m_progress.SetPos(0);
  while (!core().drOptimize(m_data))
  {
    m_progress.SetPos(int(max_e - m_data.e));
  }

  CString text;
  text.Format("Вычисление завершено. Значение Целевой функции %g", mthRound(m_data.e));
  m_info.SetWindowText(text);
  m_move.EnableWindow(TRUE);
  m_save.EnableWindow(TRUE);
}

void CRevDlg::on_change_given()
{
  m_move.EnableWindow(FALSE);
  m_save.EnableWindow(FALSE);
  m_info.SetWindowText("Готово к рассчетам");
  m_given_actual = false;
  redraw_given();
}

void CRevDlg::OnBnClickedMove()
{
  core().apply_vars(m_data.vars);
  pRD->on_modify(modify::light);
}

void CRevDlg::OnBnClickedView()
{
  form_given();
  CWideView dlg;
  CString func = m_data.func.GetAsStr().c_str();;
  func.Replace("+ABS", "+\r\nABS");
  func += " = 0";
  
  dlg.text = "Целевая функция:\r\n";
  dlg.text += func;
  dlg.text += "\r\n";

  func = "Результаты поиска: \r\n";
  for (Size v = 0; v < m_data.vars.size(); ++v)
  {
    const SOVar& var = m_data.vars[v];
    CString s;
    s.Format("%s = %g\r\n", var.var.GetName().c_str(), mthRound(var.end));
    func += s;
  }
  dlg.text += func;

  func.Format("Значение целевой функции: Z = %g", mthRound(m_data.func.solve(m_data.given)));
  dlg.text += func;

  dlg.DoModal();
}
