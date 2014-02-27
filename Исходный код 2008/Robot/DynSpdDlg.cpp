// DynSpdDlg.cpp : implementation file
//

#include "stdafx.h"
#include "robot.h"
#include "DynSpdDlg.h"
#include "FlyEditDlg.h"
#include "EditLinkDlg.h"

extern CRobotDoc * pRD;
extern CRobotView * pRV;
//extern SLinkData LDs[3];
extern MWorkspace WS;

// CDynSpdDlg dialog

IMPLEMENT_DYNAMIC(CDynSpdDlg, CDialog)
CDynSpdDlg::CDynSpdDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDynSpdDlg::IDD, pParent)
{
}

CDynSpdDlg::~CDynSpdDlg()
{
}

void CDynSpdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SYST, m_syst);
	DDX_Control(pDX, IDC_SOL, m_sol);
	DDX_Control(pDX, IDC_REPORT, m_rep);
}


BEGIN_MESSAGE_MAP(CDynSpdDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_SYST, OnCbnSelchangeSyst)
	ON_BN_CLICKED(IDC_EDIT_LINKS, OnBnClickedEditLinks)
END_MESSAGE_MAP()


// CDynSpdDlg message handlers

BOOL CDynSpdDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	///SOLUTION TABLE
	m_sol.InsertColumn(0,"Переменная",LVCFMT_LEFT,85);
	m_sol.InsertColumn(1,"Значение",LVCFMT_LEFT,158);

	//REPORT TABLE
	m_rep.SetExtendedStyle(LVS_EX_GRIDLINES  /*LVS_NOCOLUMNHEADER | LVS_REPORT*/);
	/*LV_COLUMN lvColumn;
	lvColumn.mask = LVCF_FMT || LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_CENTER;
	lvColumn.cx = 400;
	m_rep.InsertColumn(0, &lvColumn);*/
	m_rep.InsertColumn(0, "rwe", LVCFMT_CENTER, 440);
	///COMBOS
	Size lq=core().link_count();
	for (Size l=1;l<lq;l++)
		m_syst.AddString(core().link(l).Name.c_str());
	m_syst.SetCurSel(lq-2);

	UpdateReport();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDynSpdDlg::OnCbnSelchangeSyst()
{
	UpdateData();
	UpdateReport();
}

void CDynSpdDlg::UpdateReport()
{
	/*
	m_rep.DeleteAllItems();
	m_sol.DeleteAllItems();
	int link = m_syst.GetCurSel()+1;
	MMatrix<MSymb> vm(4,1), um(4,4);
	CString l;
	CLink* pL;
	for (Size i=0; i<4; i++)
	{
		um = core().MatriceUij(link, i+1);
		pL=core().Ls[link];
		for (Size j=0; j<link; j++)
		{
			//pL=core().Ls[j];
			int pmot=LDs[core().Ls[j]->Type].Motion;
			switch(pmot) 
			{
			case 0:
				l.Format("dS%i",j+1);
				break;
			case 1:
				l.Format("dTh%i",j+1);
				break;
			}
			
			vm(j, 0) += um(j, 3)*l;
		}
	}

	CString st;
	for (Size y=0;y<4;y++)
	{
		st = vm(y, 0).GetAsStr();
		m_rep.InsertItem(y, st);
	}
	
	for (Size j=0; j<link; j++)
	{
		int pmot=LDs[core().Ls[j]->Type].Motion;
		pL=core().Ls[j];
		switch(pmot) 
		{
		case 0:
			l.Format("dS%i",j+1);
			break;
		case 1:
			l.Format("dTh%i",j+1);
			break;
		}
		WS.Assign(l,pL->m_speed);
	}
	for (Size m=0;m<4;m++)
		vm(m, 0).Substate(&WS);

		//WS.Assign(l,pL->m_speed);
	//for (Size i=0; i<link; i++)
	//{
	//	WS.Assign("Th",pPT->pPLs[a].Th);
	//	WS.Assign("S",pPT->pPLs[a].S);
	//	WS.Assign("Al",pPT->pPLs[a].Al);
	//	WS.Assign("A",pPT->pPLs[a].A);
	//	pAi->pAis[a]=prod;
	//	//pAi->pAis[a].SubState(&WS);
	//	for (Size m=0;m<16;m++)
	//		pAi->pAis[a][m].Substate(&WS);
	//}
	//WS.Release();
*/
}

void CDynSpdDlg::OnBnClickedEditLinks()
{
	CEditLinkDlg eld;
	eld.link=0;
	eld.DoModal();
}
