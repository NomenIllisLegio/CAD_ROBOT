// DerMatrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "robot.h"
#include "InertMatrDlg.h"


// CInertMatrDlg dialog
extern CRobotDoc * pRD;

IMPLEMENT_DYNAMIC(CInertMatrDlg, CDialog)
CInertMatrDlg::CInertMatrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInertMatrDlg::IDD, pParent)
{
}

CInertMatrDlg::~CInertMatrDlg()
{
}

void CInertMatrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MATR, m_mtr);
	DDX_Control(pDX, IDC_LINK, m_link_c);
}


BEGIN_MESSAGE_MAP(CInertMatrDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_LINK, OnCbnSelchangeLink)
END_MESSAGE_MAP()


// CInertMatrDlg message handlers

BOOL CInertMatrDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	////preparing table
	m_mtr.InsertColumn(0,"Индексы",LVCFMT_LEFT,60);
	m_mtr.InsertColumn(1,"1",LVCFMT_LEFT,141);
	m_mtr.InsertColumn(2,"2",LVCFMT_LEFT,141);
	m_mtr.InsertColumn(3,"3",LVCFMT_LEFT,142);
	m_mtr.InsertColumn(4,"4",LVCFMT_LEFT,142);

	////
	m_hi = core().rMatricesHi();

	for (int l=1;l<core().link_count();l++)
		m_link_c.AddString(core().link(l).Name.c_str());
	
	m_link_c.SetCurSel(0);

	UpdateMatrix(m_link_c.GetCurSel() + 1);
	UpdateData(false);
	return TRUE;
}

void CInertMatrDlg::UpdateMatrix(int n)
{
	m_mtr.DeleteAllItems();
	
	matrix = m_hi->Hi[n];
	//drawing
	LVITEM lvi;
	lvi.mask=LVIF_TEXT;
	CString st;
	for (Size y=0;y<4;y++)
	{
		st.Format("%i",y+1);
		int ui=m_mtr.InsertItem(y,st);
		lvi.iItem=ui;
		for (Size x=0;x<4;x++)
		{
			lvi.iSubItem=x+1;
			
      st.Format("%g", matrix(y,x));
			lvi.pszText=(char*)(LPCSTR)st;
			m_mtr.SetItem(&lvi);
		}
	}
}

void CInertMatrDlg::OnCbnSelchangeLink()
{
	UpdateMatrix(m_link_c.GetCurSel() + 1);
}
