// FormulaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "robot.h"
#include "FormulaDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFormulaDlg dialog


CFormulaDlg::CFormulaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFormulaDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormulaDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFormulaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormulaDlg)
	DDX_Control(pDX, IDC_FORM_EDIT, m_edit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormulaDlg, CDialog)
	//{{AFX_MSG_MAP(CFormulaDlg)
	ON_BN_CLICKED(IDC_B_MX, OnBMx)
	ON_BN_CLICKED(IDC_B_RX, OnBRx)
	ON_BN_CLICKED(IDC_B_MY, OnBMy)
	ON_BN_CLICKED(IDC_B_RY, OnBRy)
	ON_BN_CLICKED(IDC_B_MZ, OnBMz)
	ON_BN_CLICKED(IDC_B_RZ, OnBRz)
	ON_BN_CLICKED(IDC_B_DEL, OnBDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormulaDlg message handlers

BOOL CFormulaDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	Fq=0;
	UpdateEdit();
	return TRUE;
}

void CFormulaDlg::UpdateEdit()
{
	static char ms[]="об";
	static char as[]="xyz";
	CString frm="";
	for (Size f=0;f<Fq;f++)
	{
		if (f!=0) frm+=" - ";
		frm+=ms[Fs[f].LinkType];
		frm+=as[Fs[f].Axis];
	}
	m_edit.SetWindowText(frm);
}

void CFormulaDlg::AddForm(int a, int t)
{
	if (Fq>=FORM_MAX_FORM) return;
	Fs[Fq].Axis=a;
	Fs[Fq].LinkType=t;
	Fq++;
	UpdateEdit();
}

void CFormulaDlg::OnBMx() 
{
	AddForm(0,0);
}

void CFormulaDlg::OnBRx() 
{
	AddForm(0,1);
}

void CFormulaDlg::OnBMy() 
{
	AddForm(1,0);	
}

void CFormulaDlg::OnBRy() 
{
	AddForm(1,1);	
}

void CFormulaDlg::OnBMz() 
{
	AddForm(2,0);		
}

void CFormulaDlg::OnBRz() 
{
	AddForm(2,1);		
}

void CFormulaDlg::OnBDel() 
{
	if (Fq>0) Fq--;
	UpdateEdit();
}

void CFormulaDlg::OnOK() 
{
	core().remove_all_links();
	static XVector<3,float> as[3];
	as[0]=-1,0,0;
	as[1]=0,1,0;
	as[2]=0,0,1;
	
	Size l;
  for (Size f=0;f<Fq;f++)
	{
		l=core().AddLink(f-1,Fs[f].LinkType);
		core().link(l).Orient=as[Fs[f].Axis];
	}
	l=core().AddLink(Fq-1,2);
	pRD->on_modify(modify::big);
	CDialog::OnOK();
}
