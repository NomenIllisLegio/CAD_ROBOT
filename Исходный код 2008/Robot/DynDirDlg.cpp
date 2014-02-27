// DynDirDlg.cpp : implementation file
//

#include "stdafx.h"
#include "robot.h"
#include "DynDirDlg.h"
#include "FlyEditDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDynDirDlg dialog
extern CRobotDoc * pRD;
extern CRobotView * pRV;

CDynDirDlg::CDynDirDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDynDirDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDynDirDlg)
	m_iv = -1;
	m_sv = -1;
	m_substate = FALSE;
	//}}AFX_DATA_INIT
}


void CDynDirDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDynDirDlg)
	DDX_Control(pDX, IDC_COORDINATES, m_move);
	DDX_Control(pDX, IDC_REPORT, m_rep);
	DDX_Control(pDX, IDC_SYST, m_syst);
	//DDX_Control(pDX, IDC_INSYST, m_insyst);
	DDX_Control(pDX, IDC_GIVEN, m_giv);
	//DDX_CBIndex(pDX, IDC_INSYST, m_iv);
	DDX_CBIndex(pDX, IDC_SYST, m_sv);
	DDX_Check(pDX, IDC_SUBSTATE, m_substate);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDynDirDlg, CDialog)
	//{{AFX_MSG_MAP(CDynDirDlg)
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CLICK, IDC_GIVEN, OnClickGiven)
	ON_BN_CLICKED(IDC_DEFAULT, OnDefault)
	ON_CBN_SELCHANGE(IDC_SYST, OnSelchangeSyst)
	//ON_CBN_SELCHANGE(IDC_INSYST, OnSelchangeInsyst)
	ON_BN_CLICKED(IDC_COORDINATES, OnCoordinates)
	ON_BN_CLICKED(IDC_CURRENT, OnCurrent)
	ON_BN_CLICKED(IDC_SUBSTATE, OnClickSubstate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDynDirDlg message handlers

BOOL CDynDirDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	pPT=core().rParamsTable();
	pAi=core().rMatricesAi();
	pGiven = new MSymb[pPT->pVs.size()];
	mt.Init(4,4);
	///GIVEN TABLE
	m_giv.InsertColumn(0,"Скорость",LVCFMT_LEFT,85);
	m_giv.InsertColumn(1,"Значение",LVCFMT_LEFT,158);
	
	///COMBOS
	int lq=core().link_count();
	for (int l=1;l<core().link_count();l++)
		m_syst.AddString(core().link(l).Name.c_str());
	m_syst.SetCurSel(lq - 3);
	OnCurrent();
	UpdateData(false);
	return TRUE;
}

void CDynDirDlg::UpdateGiven()
{
	m_giv.DeleteAllItems();
	LVITEM lvi;
	lvi.mask=LVIF_TEXT;
	CString st;
	for (Size g=0;g<pPT->pVs.size();g++)
	{
		st="d(" + CString(pPT->pVs[g].var.GetName().c_str()) + ")/dt";
		int ai=m_giv.InsertItem(g,st);
		lvi.iItem=ai;
		lvi.iSubItem=1;
		st=pGiven[g].GetAsStr().c_str();
		lvi.pszText=(char*)(LPCSTR)st;
		m_giv.SetItem(&lvi);
	}
}

void CDynDirDlg::LoadDefaultGiven()
{
	/*
	for (Size g=0;g<pPT->pVs.size();g++)
		pGiven[g] ;
	UpdateGiven();	
	*/
}

void CDynDirDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	delete[] pGiven;
}

void CDynDirDlg::OnClickGiven(NMHDR* pNMHDR, LRESULT* pResult) 
{
	POSITION pos = m_giv.GetFirstSelectedItemPosition();
	if (pos)
	{	
		int num=m_giv.GetNextSelectedItem(pos);
		CRect r,r1;
		m_giv.GetSubItemRect(num,1,LVIR_BOUNDS,r);
		m_giv.GetWindowRect(&r1);
		r.left+=r1.left;
		r.right+=r1.left;
		r.top+=r1.top;
		r.bottom+=r1.top;
		CFlyEditDlg dlg(this);
		dlg.rect=r;
		dlg.pPW=this;
		dlg.Data=pGiven[num].GetAsStr().c_str();
		if (dlg.DoModal()!=IDOK)
			return;
		pGiven[num]=dlg.Data;
		pGiven[num].PostProcess();
		UpdateGiven();
		UpdateReport();
	}
}

void CDynDirDlg::OnDefault() 
{
	LoadDefaultGiven();
	UpdateReport();
}

void CDynDirDlg::UpdateReport()
{
	bool bEn=true;
	for (Size g=0;g<pPT->pVs.size();g++)
		bEn&=(pGiven[g].IsNumber());

	//m_move.EnableWindow(bEn);
	
	m_rep.DeleteAllItems();
	int nColumnCount = m_rep.GetHeaderCtrl()->GetItemCount();
	for (int i=0;i < nColumnCount;i++)
		m_rep.DeleteColumn(0);
	if (!FormMatrix(m_sv+1))
		return;	
	
	m_rep.InsertColumn(0,"Индексы",LVCFMT_LEFT,60);
	m_rep.InsertColumn(1,"1",LVCFMT_LEFT,105);
	m_rep.InsertColumn(2,"2",LVCFMT_LEFT,105);
	m_rep.InsertColumn(3,"3",LVCFMT_LEFT,105);
	m_rep.InsertColumn(4,"4",LVCFMT_LEFT,105);

	LVITEM lvi;
	lvi.mask=LVIF_TEXT;
	CString st;
	for (Size y=0;y<4;y++)
	{
	st.Format("%i",y+1);
		int ai=m_rep.InsertItem(y,st);
		lvi.iItem=ai;
		for (Size x=0;x<4;x++)
		{
			lvi.iSubItem=x+1;
			st=(y<3)?GetLabelForMember(y*4+x,m_sv+1)+"=":"";
			st+=mt(y,x).GetAsStr().c_str();
			lvi.pszText=(char*)(LPCSTR)st;
			m_rep.SetItem(&lvi);
		}
	}
}

bool CDynDirDlg::FormMatrix(int s)
{
	if (m_substate)
	{
		WS.Release();
		for (Size v=0;v<pPT->pVs.size();v++)
			WS.Assign(pPT->pVs[v].var.GetName(), core().link(v+1).m_curd);
	}

	mt.ZeroThis();
	MMatrix<MSymb> U(4, 4);
	for (Size v = 0; v < pPT->pVs.size(); v ++)
	{
		core().drMatriceUij(U, s, v+1);
		if (m_substate)
		{
			for (Size u=0;u<16;u++)
				U[u].Substate(&WS);
		}
		
		U *= pGiven[v];
		mt += U;
	}

	return true;
}

CString CDynDirDlg::GetLabelForMember(int m, int s)
{
	
	int x,y;
	y=m/4;
	x=m%4;
	char buffer[2];  
	buffer[1] = 0;
	CString rv;
	if (x==3)
	{
		buffer[0] = y + 'X';
		rv = buffer;
		rv.Format(rv+"%i",s);
	}
	else
	{
		buffer[0] = y + 'i';
		rv.Format("COS(%s0^%s%i)", buffer, buffer, s);
	}
	return rv;
}

void CDynDirDlg::OnSelchangeSyst() 
{
	UpdateData();
	UpdateReport();
}

void CDynDirDlg::OnSelchangeInsyst() 
{
	UpdateData();
	UpdateReport();
}

void CDynDirDlg::OnCoordinates() 
{
	for (Size g=0;g<pPT->pVs.size();g++)
		core().link(g).m_cur_speed=pGiven[g].GetAsNumber()-pPT->pVs[g].def;
	pRD->UpdateAllViews(NULL);
}

void CDynDirDlg::OnCurrent() 
{
	for (Size g=0;g<pPT->pVs.size();g++)
		pGiven[g]=core().link(g).m_cur_speed;
	UpdateGiven();
	UpdateReport();
	UpdateData(false);
}

void CDynDirDlg::OnClickSubstate()
{
	UpdateData();
	UpdateReport();
	UpdateData(false);
}
