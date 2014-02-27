// ParamsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "robot.h"
#include "ParamsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParamsDlg dialog

extern CRobotDoc * pRD;
extern CRobotView * pRV;
extern CMainFrame * pMF;

CParamsDlg::CParamsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CParamsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CParamsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CParamsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CParamsDlg)
	DDX_Control(pDX, IDC_VALUES, m_val);
	DDX_Control(pDX, IDC_PARAMS, m_pt);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParamsDlg, CDialog)
	//{{AFX_MSG_MAP(CParamsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParamsDlg message handlers

BOOL CParamsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SRParamsTable * pPT=core().rParamsTable();

	/////PARAMS TABLE
	///organize table
	m_pt.InsertColumn(0,"Звено",LVCFMT_LEFT,45);
	m_pt.InsertColumn(1,"Кин. пара",LVCFMT_LEFT,63);
	m_pt.InsertColumn(2,"Th, град",LVCFMT_LEFT,64);
	m_pt.InsertColumn(3,"S, мм",LVCFMT_LEFT,64);
	m_pt.InsertColumn(4,"A, мм",LVCFMT_LEFT,64);
	m_pt.InsertColumn(5,"Al, град",LVCFMT_LEFT,64);

	////filling table
	LVITEM lvi;
	lvi.mask=LVIF_TEXT;
	CString s;
	for (Size i=1;i<pPT->pPLs.size();i++)
	{
		///Num
		s.Format("%i",i);
		int ai=m_pt.InsertItem(i,s);
		lvi.iItem=ai;
		///Pair
		lvi.iSubItem=1;
		s.Format("(%i, %i)",i-1,i);
		lvi.pszText=(char*)(LPCSTR)s;
		m_pt.SetItem(&lvi);
		///Th
		lvi.iSubItem=2;
		s=pPT->pPLs[i].Th.GetAsStr().c_str();
		lvi.pszText=(char*)(LPCSTR)s;
		m_pt.SetItem(&lvi);
		///S
		lvi.iSubItem=3;
		s=pPT->pPLs[i].S.GetAsStr().c_str();
		lvi.pszText=(char*)(LPCSTR)s;
		m_pt.SetItem(&lvi);
		///A
		lvi.iSubItem=4;
		s=pPT->pPLs[i].A.GetAsStr().c_str();
		lvi.pszText=(char*)(LPCSTR)s;
		m_pt.SetItem(&lvi);
		///Al
		lvi.iSubItem=5;
		s=pPT->pPLs[i].Al.GetAsStr().c_str();
		lvi.pszText=(char*)(LPCSTR)s;
		m_pt.SetItem(&lvi);
	}
	
	/////VALUES TABLE
	///organize table
	m_val.InsertColumn(0,"Параметр",LVCFMT_LEFT,75);
	m_val.InsertColumn(1,"Минимум",LVCFMT_LEFT,70);
	m_val.InsertColumn(2,"Максимум",LVCFMT_LEFT,70);
	m_val.InsertColumn(3,"Исходное",LVCFMT_LEFT,66);

	////filling table
	for (Size i=0;i<pPT->pVs.size();i++)
	{
		///Name
    s=pPT->pVs[i].var.GetName().c_str();
		int ai=m_val.InsertItem(i,s);
		lvi.iItem=ai;
		///Min
		lvi.iSubItem=1;
		s.Format("%g",pPT->pVs[i].min);
		lvi.pszText=(char*)(LPCSTR)s;
		m_val.SetItem(&lvi);
		///Max
		lvi.iSubItem=2;
		s.Format("%g",pPT->pVs[i].max);
		lvi.pszText=(char*)(LPCSTR)s;
		m_val.SetItem(&lvi);
		///Def
		lvi.iSubItem=3;
		s.Format("%g",pPT->pVs[i].def);
		lvi.pszText=(char*)(LPCSTR)s;
		m_val.SetItem(&lvi);
	}
	
	return TRUE;
}
