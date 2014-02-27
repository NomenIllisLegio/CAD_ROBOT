// EditLinkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "robot.h"
#include "EditLinkDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//extern SMotions MTs[2];
//extern SLinkData LDs[3];

extern CRobotDoc * pRD;
extern CRobotView * pRV;
extern CMainFrame * pMF;

/////////////////////////////////////////////////////////////////////////////
// CEditLinkDlg dialog


CEditLinkDlg::CEditLinkDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditLinkDlg::IDD, pParent)
  , m_length(0)
  , m_radius(0)
  , m_mass(0)
{
	//{{AFX_DATA_INIT(CEditLinkDlg)
	m_linkname = _T("");
	m_xe = 0.0f;
	m_ye = 0.0f;
	m_ze = 0.0f;
	m_lx = 0.0f;
	m_ly = 0.0f;
	m_lz = 0.0f;
	m_m_max_v = 0.0f;
	m_m_min_v = 0.0f;
	m_s_max_v = 0.0f;
	m_s_min_v = 0.0f;
	m_a_max_v = 0.0f;
	m_a_min_v = 0.0f;

	//}}AFX_DATA_INIT
}


void CEditLinkDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CEditLinkDlg)
  DDX_Control(pDX, IDC_M_MIN, m_m_min_c);
  DDX_Control(pDX, IDC_M_MAX, m_m_max_c);
  DDX_Control(pDX, IDC_S_MIN, m_s_min_c);
  DDX_Control(pDX, IDC_S_MAX, m_s_max_c);
  DDX_Control(pDX, IDC_A_MIN, m_a_min_c);
  DDX_Control(pDX, IDC_A_MAX, m_a_max_c);
  DDX_Control(pDX, IDC_M_LAB, m_m_lab);
  DDX_Control(pDX, IDC_PREV, m_prev);
  DDX_Control(pDX, IDC_NEXT, m_next);
  DDX_Control(pDX, IDC_LINK_INDEX, m_index);
  DDX_Control(pDX, IDC_LINK_Z, m_lzc);
  DDX_Control(pDX, IDC_LINK_Y, m_lyc);
  DDX_Control(pDX, IDC_LINK_X, m_lxc);
  DDX_Control(pDX, IDC_AXIS_Z_EDIT, m_zc);
  DDX_Control(pDX, IDC_AXIS_Y_EDIT, m_yc);
  DDX_Control(pDX, IDC_AXIS_X_EDIT, m_xc);
  DDX_Control(pDX, IDC_LINK_ICON, m_li);
  DDX_Control(pDX, IDC_LINK_TYPE, m_lt);
  DDX_Text(pDX, IDC_LINK_NAME, m_linkname);
  DDV_MaxChars(pDX, m_linkname, 32);
  DDX_Text(pDX, IDC_AXIS_X_EDIT, m_xe);
  DDV_MinMaxFloat(pDX, m_xe, -1.f, 1.f);
  DDX_Text(pDX, IDC_AXIS_Y_EDIT, m_ye);
  DDV_MinMaxFloat(pDX, m_ye, -1.f, 1.f);
  DDX_Text(pDX, IDC_AXIS_Z_EDIT, m_ze);
  DDV_MinMaxFloat(pDX, m_ze, -1.f, 1.f);
  DDX_Text(pDX, IDC_LINK_X, m_lx);
  DDX_Text(pDX, IDC_LINK_Y, m_ly);
  DDX_Text(pDX, IDC_LINK_Z, m_lz);
  DDX_Text(pDX, IDC_M_MAX, m_m_max_v);
  DDX_Text(pDX, IDC_M_MIN, m_m_min_v);
  DDX_Text(pDX, IDC_S_MAX, m_s_max_v);
  DDX_Text(pDX, IDC_S_MIN, m_s_min_v);
  DDX_Text(pDX, IDC_A_MAX, m_a_max_v);
  DDX_Text(pDX, IDC_A_MIN, m_a_min_v);
  //DDX_Text(pDX, IDC_LINK_SPEED, m_speed_v);
  //}}AFX_DATA_MAP

  DDX_Control(pDX, IDC_SPEED_LAB, m_speed_lab);
  DDX_Control(pDX, IDC_ACC_LAB, m_acc_lab);
  DDX_Text(pDX, IDC_LENGTH, m_length);
  DDX_Text(pDX, IDC_RADIUS, m_radius);
  DDX_Text(pDX, IDC_MASS, m_mass);
  DDX_Control(pDX, IDC_LENGTH, m_length_c);
  DDX_Control(pDX, IDC_RADIUS, m_radius_c);
  DDX_Control(pDX, IDC_MASS, m_mass_c);
}


BEGIN_MESSAGE_MAP(CEditLinkDlg, CDialog)
	//{{AFX_MSG_MAP(CEditLinkDlg)
	ON_BN_CLICKED(IDC_AXIS_X, OnAxisX)
	ON_BN_CLICKED(IDC_AXIS_Y, OnAxisY)
	ON_BN_CLICKED(IDC_AXIS_Z, OnAxisZ)
	ON_BN_CLICKED(IDC_AXIS_CUSTOM, OnAxisCustom)
	ON_BN_CLICKED(IDC_LINK_AUTO, OnLinkAuto)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_BN_CLICKED(IDC_PREV, OnPrev)
	ON_BN_CLICKED(IDC_NEXT, OnNext)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditLinkDlg message handlers

BOOL CEditLinkDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	Init();
	return TRUE;
}

void CEditLinkDlg::OnOK() 
{
	Apply();
	CDialog::OnOK();
}

void CEditLinkDlg::UpdateAxis(bool allCustom)
{
	V3f o;
	
	m_xe=Orient[0];
	m_ye=Orient[1];
	m_ze=Orient[2];
	
	UINT RID=IDC_AXIS_CUSTOM;
	if (!allCustom)
	{
		o=-1,0,0;
		if (Orient==o) RID=IDC_AXIS_X;
		o=0,1,0;
		if (Orient==o) RID=IDC_AXIS_Y;
		o=0,0,1;
		if (Orient==o) RID=IDC_AXIS_Z;
	}
	CheckRadioButton(IDC_AXIS_X,IDC_AXIS_CUSTOM,RID);
	bool edits=(RID==IDC_AXIS_CUSTOM);
	m_xc.EnableWindow(edits);
	m_yc.EnableWindow(edits);
	m_zc.EnableWindow(edits);
	
	CString s;
	s.Format("%g",m_xe);
	m_xc.SetWindowText(s);
	s.Format("%g",m_ye);
	m_yc.SetWindowText(s);
	s.Format("%g",m_ze);
	m_zc.SetWindowText(s);
}

void CEditLinkDlg::OnAxisX() 
{
	Orient=-1,0,0;
	UpdateAxis(false);
}

void CEditLinkDlg::OnAxisY() 
{
	Orient=0,1,0;
	UpdateAxis(false);
}

void CEditLinkDlg::OnAxisZ() 
{
	Orient=0,0,1;
	UpdateAxis(false);
}

void CEditLinkDlg::OnAxisCustom() 
{
	UpdateAxis(true);
}

void CEditLinkDlg::UpdateGeo()
{
	m_lx=pL->Geo[0];
	m_ly=pL->Geo[1];
	m_lz=pL->Geo[2];
	bool edits=link!=0;
	m_lxc.EnableWindow(edits);
	m_lyc.EnableWindow(edits);
	m_lzc.EnableWindow(edits);
	UpdateData(false);
}

void CEditLinkDlg::OnLinkAuto() 
{
	V3f ag=core().GetAutoGeoFor(link);
	m_lx=ag[0];
	m_ly=ag[1];
	m_lz=ag[2];
	UpdateData(false);
}

void CEditLinkDlg::Apply()
{
	UpdateData();
	pL->Name=m_linkname;
	MMatrix<float> or(3);
	or=m_xe,m_ye,m_ze;
	pL->Orient.LoadData(or.norm().pData);
	pL->Geo=m_lx,m_ly,m_lz;

	pL->m_min=m_m_min_v;
	pL->m_max=m_m_max_v;
	pL->m_min_speed = m_s_min_v;
	pL->m_max_speed = m_s_max_v;
	pL->m_min_acc = m_a_min_v;
	pL->m_max_acc = m_a_max_v;
  pL->mass = m_mass;
  pL->length = m_length;
  pL->radius = m_radius;

  pRD->on_modify(modify::big);
}

void CEditLinkDlg::OnApply() 
{
	Apply();	
}

void CEditLinkDlg::Init()
{
	pL=&core().link(link);
	
	m_lt.SetWindowText(core().data().LDs[pL->Type].Name.c_str());
	CString s;s.Format("Индекс: %i",link);
	m_index.SetWindowText(s);
	m_linkname=pL->Name.c_str();
	m_li.SetIcon(AfxGetApp()->LoadIcon(core().data().LDs[pL->Type].Icon));

	//Orient
	Orient=pL->Orient;
	UpdateAxis(false);
	//Geo
	UpdateGeo();
	
	//buttons
	m_prev.EnableWindow(link>0);
	m_next.EnableWindow(link < int(core().link_count()) - 1);

	//motions
	UpdateMotions();

  update_inert();

	UpdateData(false);
}

void CEditLinkDlg::OnPrev() 
{
	Apply();
	link--;
	Init();
}

void CEditLinkDlg::OnNext() 
{
	Apply();
	link++;
	Init();
}

void CEditLinkDlg::UpdateMotions()
{
	bool ew;
	if (core().data().LDs[pL->Type].Motion!=-1)
	{
		SMotions& mt=core().data().MTs[core().data().LDs[pL->Type].Motion];
		m_m_lab.SetWindowText(mt.Capt.c_str());
		m_speed_lab.SetWindowText(mt.Speed_Capt.c_str());
    m_acc_lab.SetWindowText(mt.acc_capt.c_str());
		m_m_min_v=pL->m_min;
		m_m_max_v=pL->m_max;
		m_s_min_v=pL->m_min_speed;
		m_s_max_v=pL->m_max_speed;
  	m_a_min_v=pL->m_min_acc;
		m_a_max_v=pL->m_max_acc;

    ew=true;
	}
	else
	{
		m_m_lab.SetWindowText("");
		m_speed_lab.SetWindowText("");
    m_acc_lab.SetWindowText("");
		ew=false;
	}
	m_m_max_c.EnableWindow(ew);
	m_m_min_c.EnableWindow(ew);
	m_s_max_c.EnableWindow(ew);
	m_s_min_c.EnableWindow(ew);
	m_a_max_c.EnableWindow(ew);
	m_a_min_c.EnableWindow(ew);
}

void CEditLinkDlg::update_inert()
{
  bool e = link != 0;
  
  m_mass_c.EnableWindow(e);
  m_radius_c.EnableWindow(e);
  m_length_c.EnableWindow(e);

  m_mass = e ? pL->mass : 0;
  m_length = e ? pL->length : 0;
  m_radius = e ? pL->radius : 0;
}
