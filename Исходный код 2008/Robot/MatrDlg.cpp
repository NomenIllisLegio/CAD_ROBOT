// MatrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "robot.h"
#include "MatrDlg.h"
#include ".\matrdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMatrDlg dialog

extern CRobotDoc * pRD;

CMatrDlg::CMatrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMatrDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMatrDlg)
	m_fval = -1;
	m_tval = -1;
  m_mean_val = -1;
	//}}AFX_DATA_INIT
  m_mt.Init(4, 4);
}


void CMatrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMatrDlg)
	DDX_Control(pDX, IDC_TO, m_tctr);
	DDX_Control(pDX, IDC_FROM, m_fctr);
  DDX_Control(pDX, IDC_MEAN, m_mean_ctr);
	DDX_Control(pDX, IDC_MATR, m_mtr);
	DDX_CBIndex(pDX, IDC_FROM, m_fval);
	DDX_CBIndex(pDX, IDC_TO, m_tval);
  DDX_CBIndex(pDX, IDC_MEAN, m_mean_val);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMatrDlg, CDialog)
	//{{AFX_MSG_MAP(CMatrDlg)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_FROM, OnSelchangeFrom)
	ON_CBN_SELCHANGE(IDC_TO, OnSelchangeTo)
	//}}AFX_MSG_MAP
  ON_CBN_SELCHANGE(IDC_MEAN, OnCbnSelchangeMean)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMatrDlg message handlers

BOOL CMatrDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	pAi = core().rMatricesAi();

  //////////////////////////////////////////////////////////////////////////
  ////preparing table
	m_mtr.InsertColumn(0,"Инд.",LVCFMT_CENTER,40);
  m_mtr.InsertColumn(1,"Смысл",LVCFMT_CENTER,85);
  m_mtr.InsertColumn(2,"Выражение",LVCFMT_LEFT,2000);
	
  CString s;
  for (int r = 0; r < 16; ++ r)
  {
    int x = core().data().LOs[r];
    s.Format("%i x %i", int(x / 4) + 1, int(x % 4) + 1);
    m_mtr.InsertItem(r, s, -1);
    m_mtr.SetItemText(r, 1, core().data().MLs[x].c_str());
  }

  //mean
  m_mean_ctr.AddString("Положение");
  m_mean_ctr.AddString("Скорость");
  m_mean_ctr.AddString("Ускорение");
  m_mean_ctr.SetCurSel(0);
  
  ////From
	int lq=core().link_count();
	m_fctr.AddString("Cтатической");
	if (lq>0) m_fctr.AddString("Инерциальной");
	for (int l=1;l<core().link_count()-1;l++)
		m_fctr.AddString(core().link(l).Name.c_str());
	
  ////To
	if (lq>0) m_tctr.AddString("Инерциальной");
	for (int l=1;l<core().link_count();l++)
		m_tctr.AddString(core().link(l).Name.c_str());
	
  ////
	m_fval=(lq > 0)? 1 : 0;
	m_tval=core().link_count() - 1;
  m_mean_val = 0;
  
  UpdateData(false);

  create_matrix();
  draw_matrix();
  return TRUE;
}

void CMatrDlg::create_matrix()
{
  int s = m_fval - 1;
  int t = m_tval;
  if (t == -1 || s >= t)
  {
    m_m_valid = false;
    return;
  }
  switch (m_mean_val)
  {
  case 0:
    core().drGetMatrixT(s, t, m_mt);
    break;
  case 1:
    core().drGetMatrixV(s, t, m_mt);
    break;
  case 2:
    core().drGetMatrixC(s, t, m_mt);
    break;
  }
  m_m_valid = true;
}


void CMatrDlg::draw_matrix()
{
  for (int r = 0; r < 16; ++ r)
  {
    int x = core().data().LOs[r];
    m_mtr.SetItemText(r, 2, (m_m_valid) ? m_mt[x].GetAsStr().c_str() : "");
  }
}

void CMatrDlg::UpdateMatrix(int s, int e)
{
  return;
	m_mtr.DeleteAllItems();
	if (s==-1 || e==-1 || s>=e) return;
	MMatrix<MSymb> rm(4,4);
	//computing
	rm=pAi->pAis[s];
	for (int m = s + 1; m < e; m ++)
		rm*=pAi->pAis[m];
	//drawing
	LVITEM lvi;
	lvi.mask=LVIF_TEXT;
	CString st;
	for (Size y=0;y<4;y++)
	{
		st.Format("%i",y+1);
		int ai=m_mtr.InsertItem(y,st);
		lvi.iItem=ai;
		for (Size x=0;x<4;x++)
		{
			lvi.iSubItem=x+1;
			st=rm(y,x).GetAsStr().c_str();
			lvi.pszText=(char*)(LPCSTR)st;
			m_mtr.SetItem(&lvi);
		}
	}
}

void CMatrDlg::UpdateFirst()
{
	return;
  UpdateData(false);
	int s=-1,e=-1;
	if (m_fval>0)
	{
		s=m_fval-1;
		if (m_tval==0) 
			e=s+1;
	}
	if (m_tval>0)
	{
		e=m_tval;
		if (m_fval==0 && e>0)
			s=e-1;
	}
	UpdateMatrix(s,e);
}

void CMatrDlg::OnSelchangeFrom() 
{
  UpdateData();
  create_matrix();
  draw_matrix();
}

void CMatrDlg::OnSelchangeTo() 
{
  UpdateData();
  create_matrix();
  draw_matrix();
}

void CMatrDlg::OnCbnSelchangeMean()
{
  UpdateData();
  create_matrix();
  draw_matrix();
}
