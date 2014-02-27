// TrajDlg.cpp : implementation file
//

#include "stdafx.h"
#include "robot.h"
#include "TrajDlg.h"
#include ".\trajdlg.h"
#include "EditTrajDlg.h"
#include "GraphsDlg.h"

// CTrajDlg dialog

IMPLEMENT_DYNAMIC(CTrajDlg, CDialog)
CTrajDlg::CTrajDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTrajDlg::IDD, pParent)
{
}

CTrajDlg::~CTrajDlg()
{
}

void CTrajDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_POINTS, m_points);
  DDX_Control(pDX, IDC_PROG, m_progr);
  DDX_Control(pDX, IDC_VARS, m_vars_c);
  DDX_Control(pDX, ID_START, m_start_c);
  DDX_Control(pDX, ID_VIEW, m_view);
}


BEGIN_MESSAGE_MAP(CTrajDlg, CDialog)
  ON_BN_CLICKED(ID_ADD, OnBnClickedAdd)
  ON_BN_CLICKED(ID_EDIT, OnBnClickedEdit)
  ON_BN_CLICKED(ID_REMOVE, OnBnClickedRemove)
  ON_BN_CLICKED(ID_START, OnBnClickedStart)
  ON_BN_CLICKED(ID_VIEW, OnBnClickedView)
END_MESSAGE_MAP()


// CTrajDlg message handlers

BOOL CTrajDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_points.InsertColumn(0, "Время", LVCFMT_CENTER, 80);
  m_points.InsertColumn(1, "Х", LVCFMT_CENTER, 50);
  m_points.InsertColumn(2, "Y", LVCFMT_CENTER, 50);
  m_points.InsertColumn(3, "Z", LVCFMT_CENTER, 50);

  redraw_given();

  SRParamsTable* pPT = core().rParamsTable();
  for (Size v = 0; v < pPT->pVs.size(); ++ v)
  {
    m_vars_c.AddString((pPT->pVs[v].var.GetName() + "(t)").c_str());
  }

  m_vars_c.AddString("X(t) схвата");
  m_vars_c.AddString("Y(t) схвата");
  m_vars_c.AddString("Z(t) схвата");

  m_view.EnableWindow(core().traj_record().traj_ready());
  m_start_c.EnableWindow(core().traj_record().size() > 1);

  return TRUE;
}

void CTrajDlg::redraw_given()
{
  m_points.DeleteAllItems();
  CString text;
  
  for (Size s = 0; s < core().traj_record().size(); ++ s)
  {
    const TrajPoint& point = core().traj_record().point(s);
    text.Format("%g", mthRound(point.time));
    m_points.InsertItem(s, text);
    
    text.Format("%g", mthRound(point.x));
    m_points.SetItemText(s, 1, text);

    text.Format("%g", mthRound(point.y));
    m_points.SetItemText(s, 2, text);

    text.Format("%g", mthRound(point.z));
    m_points.SetItemText(s, 3, text);
  }
}

int CTrajDlg::selected_point() const
{
  POSITION pos = m_points.GetFirstSelectedItemPosition();
  if (!pos)
    return -1;
  else
    return m_points.GetNextSelectedItem(pos);
}

void CTrajDlg::OnBnClickedAdd()
{
  TrajPoint tp;
  tp.time = core().traj_record().finish_time() + 1.0f;
  tp.x = 0.0f;
  tp.y = 0.0f;
  tp.z = 0.0f;
  CEditTrajDlg dlg;
  dlg.traj_point = &tp;

  if (dlg.DoModal() != IDOK)
    return;

  core().traj_record().add_point(tp);
  redraw_given();

  m_start_c.EnableWindow(core().traj_record().size() > 1);
}

void CTrajDlg::OnBnClickedEdit()
{
  int pos = selected_point();
  if (pos == -1)
    return;
  CEditTrajDlg dlg;
  dlg.traj_point = &core().traj_record().point(pos);
  if (dlg.DoModal() != IDOK)
    return;
  core().traj_record().point(pos) = *dlg.traj_point;
  core().traj_record().update_data();
  redraw_given();
}

void CTrajDlg::OnBnClickedRemove()
{
  int pos = selected_point();
  if (pos == -1)
    return;
  core().traj_record().del_point(pos);
  redraw_given();

  m_start_c.EnableWindow(core().traj_record().size() > 1);
}

void CTrajDlg::OnBnClickedStart()
{
  data.clear();
  m_progr.SetRange(0, 100);
  
  int progr = 0;
  while (!core().drTrajectory(data))
  {
    if (progr != data.progress)
    {
      progr = data.progress;
      m_progr.SetPos(progr);
    }
  }

  m_view.EnableWindow(core().traj_record().traj_ready());
}

void CTrajDlg::OnBnClickedView()
{
  int sel = m_vars_c.GetCurSel();
  if (sel < 0)
    return;

  CGraphsDlg dlg;
  if (sel < core().link_count() - 1)
  {
    MVar var = core().var_by_number(sel);
    dlg.func = &core().traj_record().spline(var);
    if (dlg.func->empty())
    {
      MessageBox("Эта координата не влияет на положение манипулятора", "Внимание!");
      return;
    }
    dlg.x_units = "сек";
    dlg.y_units = core().link_det_unit(sel).c_str();
    dlg.caption = CString("Траектория обобщенной координаты ") + var.GetName().c_str() + ".";
  }
  else
  {
    int c = sel - (core().link_count() - 1);
    String xyz[] = {"x", "y", "z"};
    String var1 = xyz[c] + "t";
    String var2 = xyz[c] + "r";
    dlg.func = &core().traj_record().spline(var2.c_str());
    dlg.func_2 = &core().traj_record().spline(var1.c_str());
    dlg.x_units = "сек";
    dlg.y_units = "м";
    dlg.caption = CString("Траектория координаты (") + xyz[c].c_str() + ") схвата в ССК. (Красная - теоретическая).";
  }
  dlg.DoModal();
}
