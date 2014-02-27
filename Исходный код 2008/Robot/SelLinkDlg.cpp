// SelLinkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "robot.h"
#include "SelLinkDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelLinkDlg dialog


CSelLinkDlg::CSelLinkDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelLinkDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelLinkDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSelLinkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelLinkDlg)
	DDX_Control(pDX, IDC_LINKLIST, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelLinkDlg, CDialog)
	//{{AFX_MSG_MAP(CSelLinkDlg)
	ON_NOTIFY(NM_DBLCLK, IDC_LINKLIST, OnDblclkLinklist)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelLinkDlg message handlers

BOOL CSelLinkDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	pIL=new CImageList;
	pIL->Create(32, 32, ILC_COLOR8, 2, 2);
	pIL->Add(AfxGetApp()->LoadIcon(core().data().LDs[0].Icon));
	pIL->Add(AfxGetApp()->LoadIcon(core().data().LDs[1].Icon));
	pIL->Add(AfxGetApp()->LoadIcon(core().data().LDs[2].Icon));
	
	m_list.SetImageList(pIL,LVSIL_NORMAL);
	m_list.InsertItem(0,core().data().LDs[0].Name.c_str(),0);
	m_list.InsertItem(1,core().data().LDs[1].Name.c_str(),1);
	m_list.InsertItem(2,core().data().LDs[2].Name.c_str(),2);
	
	m_list.SetItemState(0,LVIS_SELECTED, LVIS_SELECTED);
	
	return TRUE;
}

void CSelLinkDlg::OnDblclkLinklist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if (pos)
	{
		iType=m_list.GetNextSelectedItem(pos);
		CDialog::OnOK();
	}
	*pResult = 0;
}

void CSelLinkDlg::OnOK() 
{
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if (pos)
	{
		iType=m_list.GetNextSelectedItem(pos);
		CDialog::OnOK();
	}
	else
	{
		CDialog::OnCancel();
	}
}

void CSelLinkDlg::OnClose() 
{
	if (pIL) delete pIL;	
	CDialog::OnClose();
}
