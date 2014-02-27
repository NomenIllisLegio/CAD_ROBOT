// KinDirDlg.cpp : implementation file
//

#include "stdafx.h"
#include "robot.h"
#include "KinDirDlg.h"
#include "FlyEditDlg.h"
#include ".\kindirdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKinDirDlg dialog
extern CRobotDoc * pRD;
extern CRobotView * pRV;

CKinDirDlg::CKinDirDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKinDirDlg::IDD, pParent)
  , m_mt(4, 4)
  , m_mt_c(4, 4)
  , m_mt_v(4, 4)
{
	//{{AFX_DATA_INIT(CKinDirDlg)
	m_iv = -1;
	m_sv = -1;
	//}}AFX_DATA_INIT
}


void CKinDirDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKinDirDlg)
	DDX_Control(pDX, IDC_REPORT, m_rep);
	DDX_Control(pDX, IDC_SYST, m_syst);
	DDX_Control(pDX, IDC_INSYST, m_insyst);
	DDX_CBIndex(pDX, IDC_INSYST, m_iv);
	DDX_CBIndex(pDX, IDC_SYST, m_sv);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKinDirDlg, CDialog)
	//{{AFX_MSG_MAP(CKinDirDlg)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_SYST, OnSelchangeSyst)
	ON_CBN_SELCHANGE(IDC_INSYST, OnSelchangeInsyst)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKinDirDlg message handlers

BOOL CKinDirDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

  m_rep.InsertColumn(0, "Парам.", LVCFMT_CENTER, 80);
  m_rep.InsertColumn(1, "Полож.", LVCFMT_CENTER, 58);
  m_rep.InsertColumn(2, "Скор.", LVCFMT_CENTER, 50);
  m_rep.InsertColumn(3, "Ускор.", LVCFMT_CENTER, 50);

  for (int r = 0; r < 12; ++ r)
  {
    int x = core().data().LOs[r];
    m_rep.InsertItem(r, core().data().MLs[x].c_str(), -1);
  }

  m_mt_valid = false;
  big_update();
  
	return TRUE;
}

void CKinDirDlg::form_matrix(int s, int e)
{
  if (e==-1 || s >= e)
  {
    m_mt_valid = false;
    return;
  }
	
  core().drGetMatrixT(s, e, m_mt);
  core().drGetMatrixV(s, e, m_mt_v);
  core().drGetMatrixC(s, e, m_mt_c);
  
  m_mt_valid = true;
  return;
}

void CKinDirDlg::OnSelchangeSyst() 
{
  UpdateData();
  form_matrix(m_iv - 1, m_sv);
  small_update();
}

void CKinDirDlg::OnSelchangeInsyst() 
{
  UpdateData();
  form_matrix(m_iv - 1, m_sv);
  small_update();
}

void CKinDirDlg::big_update()
{
  pPT = core().rParamsTable();
	pAi = core().rMatricesAi();

	//COMBOS
	m_syst.ResetContent();
  m_insyst.ResetContent();

  int lq = core().link_count();
	m_syst.AddString("Инерциальная");
	for (int l=1;l<core().link_count();l++)
		m_syst.AddString(core().link(l).Name.c_str());
	m_insyst.AddString("Cтатической");
	if (lq>0) m_insyst.AddString("Инерциальной");
	for (int l=1;l<core().link_count()-1;l++)
		m_insyst.AddString(core().link(l).Name.c_str());
	
  m_sv = lq - 1;
	m_iv = 1;

  UpdateData(false);
  form_matrix(m_iv - 1, m_sv);
  small_update();
}

void CKinDirDlg::update(int power)
{
  if (power >= modify::big)
    big_update();
  else
    small_update();
}

void CKinDirDlg::small_update()
{
  if (!m_mt_valid)
  {
    for (int t = 0; t < 12; ++ t)
      m_rep.SetItemText(t, 1, "");
    return;
  }
    
  for (Size v = 0; v < pPT->pVs.size(); v ++)
  {
    const Link& link = core().link(v);
    
    MVar& p_var = pPT->pVs[v].var;
    MVar v_var = ("d" + p_var.GetName()).c_str();
    MVar a_var = ("d" + v_var.GetName()).c_str();
    
    float p_val = link.m_curd + pPT->pVs[v].def;
    float v_val = link.m_cur_speed;
    float a_val = link.m_cur_acc;
    
    m_given.values[p_var] = p_val;
    m_given.values[v_var] = v_val;
    m_given.values[a_var] = a_val;
  }

  CString text;
  for (int r = 0; r < 12; ++ r)
  {
    int x = core().data().LOs[r];
    float v;
    
    //Value
    v = m_mt[x].solve(m_given);
    if (r > 2)
      v = acosf(v) * DEG_K;
    text.Format("%g", mthRound(v));
    if (m_rep.GetItemText(r, 1) != text)
      m_rep.SetItemText(r, 1, text);

    //Speed
    v = m_mt_v[x].solve(m_given);
    text.Format("%g", mthRound(v));
    if (m_rep.GetItemText(r, 2) != text)
      m_rep.SetItemText(r, 2, text);

    //Acceleration
    v = m_mt_c[x].solve(m_given);
    text.Format("%g", mthRound(v));
    if (m_rep.GetItemText(r, 3) != text)
      m_rep.SetItemText(r, 3, text);
  }
}
