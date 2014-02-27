// PosDlg.cpp : implementation file
//

#include "stdafx.h"
#include "robot.h"
#include "PosDlg.h"
#include "robotDoc.h"
#include "ValueDlg.h"
#include ".\posdlg.h"
#include "EditPosDlg.h"

extern CRobotDoc* pRD;

// CPosDlg dialog

IMPLEMENT_DYNAMIC(CPosDlg, CDialog)
CPosDlg::CPosDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPosDlg::IDD, pParent)
{
}

CPosDlg::~CPosDlg()
{
}

void CPosDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_POS, m_pos);
  DDX_Control(pDX, IDC_STORE, m_store);
}


BEGIN_MESSAGE_MAP(CPosDlg, CDialog)
  ON_BN_CLICKED(IDC_APPLY, OnBnClickedApply)
  ON_BN_CLICKED(IDC_RENAME, OnBnClickedRename)
  ON_BN_CLICKED(IDC_SAVE, OnBnClickedSave)
  ON_BN_CLICKED(IDC_DELETE, OnBnClickedDelete)
  ON_NOTIFY(NM_DBLCLK, IDC_POS, OnNMDblclkPos)
END_MESSAGE_MAP()


// CPosDlg message handlers

BOOL CPosDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_pos.InsertColumn(0, "Звено", LVCFMT_LEFT, 55);
  m_pos.InsertColumn(1, "Полож.", LVCFMT_CENTER, 50);
  m_pos.InsertColumn(2, "Скор.", LVCFMT_CENTER, 50);
  m_pos.InsertColumn(3, "Уск.", LVCFMT_CENTER, 40);

  big_update();

  return TRUE;
}

void CPosDlg::update(int power)
{
  if (power >= modify::big)
    big_update();
  else
    small_update();
}

void CPosDlg::small_update()
{
  r_params = core().rParamsTable();
  for (int l = 0; l < core().link_count() - 1; ++ l)
  {
    const Link& link = core().link(l);
    CString text;
    
    text.Format("%g", link.m_curd + ((l != core().link_count() - 1) ? r_params->pVs[l].def : 0));
    if (m_pos.GetItemText(l, 1) != text)
      m_pos.SetItemText(l, 1, text);
    
    text.Format("%g", link.m_cur_speed);
    if (m_pos.GetItemText(l, 2) != text)
      m_pos.SetItemText(l, 2, text);
    
    text.Format("%g", link.m_cur_acc);
    if (m_pos.GetItemText(l, 3) != text)
      m_pos.SetItemText(l, 3, text);
  }
}

void CPosDlg::big_update()
{
  m_pos.DeleteAllItems();

  for (int l = 0; l < core().link_count() - 1; ++ l)
  {
    const Link& link = core().link(l);
    m_pos.InsertItem(l, link.Name.c_str(), -1);
  }

  m_store.ResetContent();
  for (int r = 0; r < core().records_count(); ++ r)
  {
    if (core().record(r).consists("positions"))
      m_store.AddString(core().record(r).name().c_str());
  }

  small_update();
}

void CPosDlg::OnBnClickedApply()
{
  int pos = m_store.GetCurSel();
  if (pos < 0)
    return;
  CString str;
  m_store.GetText(pos, str);
  core().apply_record(str);
  pRD->on_modify(modify::light);
}

void CPosDlg::OnBnClickedRename()
{
  int pos = m_store.GetCurSel();
  if (pos < 0)
    return;
  CString str;
  m_store.GetText(pos, str);
  Record* record = core().find_record(str);
  if (record)
  {
    CValueDlg dlg;
    dlg.data = record->name().c_str();
    if (dlg.DoModal() == IDOK)
    {
      record->set_name(dlg.data);
      big_update();
    }
  }
}

void CPosDlg::OnBnClickedSave()
{
  CString str;
  str.Format("Позиция %i", core().records_count());
  core().record_positions(str);
  big_update();
}

void CPosDlg::OnBnClickedDelete()
{
  int pos = m_store.GetCurSel();
  if (pos < 0)
    return;
  CString str;
  m_store.GetText(pos, str);
  core().delete_record(str);  
  big_update();
}

void CPosDlg::OnNMDblclkPos(NMHDR *pNMHDR, LRESULT *pResult)
{
  POSITION p = m_pos.GetFirstSelectedItemPosition();
  if (p == NULL)
    return;
  int pos = m_pos.GetNextSelectedItem(p);
  
  CEditPosDlg dlg;
  Link& link = core().link(pos);
  dlg.pos = link.m_curd + ((pos != core().link_count() - 1) ? r_params->pVs[pos].def : 0);
  dlg.vel = link.m_cur_speed;
  dlg.acc = link.m_cur_acc;
  
  if (dlg.DoModal() != IDOK)
    return;
  
  link.m_curd = dlg.pos - ((pos != core().link_count() - 1) ? r_params->pVs[pos].def : 0);
  link.m_cur_speed = dlg.vel;
  link.m_cur_acc = dlg.acc;
  
  pRD->on_modify(modify::light);
  *pResult = 0;
}
