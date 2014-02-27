// DerMatrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "robot.h"
#include "DerMatrDlg.h"


// CDerMatrDlg dialog
extern CRobotDoc * pRD;

IMPLEMENT_DYNAMIC(CDerMatrDlg, CDialog)
CDerMatrDlg::CDerMatrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDerMatrDlg::IDD, pParent)
{
}

CDerMatrDlg::~CDerMatrDlg()
{
}

void CDerMatrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MATR, m_mtr);
	DDX_Control(pDX, IDC_MATR, m_mtr);
	DDX_Control(pDX, IDC_LINK, m_link_c);
	DDX_Control(pDX, IDC_COORDINATE, m_coord_c);
}


BEGIN_MESSAGE_MAP(CDerMatrDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_COORDINATE, OnCbnSelchangeCoordinate)
	ON_CBN_SELCHANGE(IDC_LINK, OnCbnSelchangeLink)
END_MESSAGE_MAP()


// CDerMatrDlg message handlers

BOOL CDerMatrDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	////preparing table
	m_mtr.InsertColumn(0,"Индексы",LVCFMT_LEFT,60);
	m_mtr.InsertColumn(1,"1",LVCFMT_LEFT,141);
	m_mtr.InsertColumn(2,"2",LVCFMT_LEFT,141);
	m_mtr.InsertColumn(3,"3",LVCFMT_LEFT,142);
	m_mtr.InsertColumn(4,"4",LVCFMT_LEFT,142);

	////
	SRParamsTable* pPT = core().rParamsTable();

	for (int l=1;l<core().link_count();l++)
		m_link_c.AddString(core().link(l).Name.c_str());
	
	for (Size v = 0; v < pPT->pVs.size(); ++ v)
		m_coord_c.AddString(pPT->pVs[v].var.GetName().c_str());
	
	m_link_c.SetCurSel(core().link_count() - 2);
	m_coord_c.SetCurSel(0);	
	UpdateMatrix(m_link_c.GetCurSel() + 1, m_coord_c.GetCurSel() + 1);
	UpdateData(false);
	return TRUE;
}

void CDerMatrDlg::UpdateMatrix(int n , int m)
{
	m_mtr.DeleteAllItems();
	
	core().drMatriceUij(matrix, n, m - 1);
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
			st=matrix(y,x).GetAsStr().c_str();
			lvi.pszText=(char*)(LPCSTR)st;
			m_mtr.SetItem(&lvi);
		}
	}
}


void CDerMatrDlg::OnCbnSelchangeCoordinate()
{
	UpdateMatrix(m_link_c.GetCurSel() + 1, m_coord_c.GetCurSel() + 1);
}

void CDerMatrDlg::OnCbnSelchangeLink()
{
	UpdateMatrix(m_link_c.GetCurSel() + 1, m_coord_c.GetCurSel() + 1);
}
