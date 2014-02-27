// KinRevDlg.cpp : implementation file
//

#include "stdafx.h"
#include "robot.h"
#include "KinRevDlg.h"
#include "ChangeGivenDlg.h"
#include "ParamsDlg.h"
#include "KinDirDlg.h"
#include "EditVarsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKinRevDlg dialog
extern CRobotDoc * pRD;

CKinRevDlg::CKinRevDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKinRevDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKinRevDlg)
	m_func = _T("");
	//}}AFX_DATA_INIT
}


void CKinRevDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKinRevDlg)
	DDX_Control(pDX, IDC_VALUE, m_val);
	DDX_Control(pDX, IDC_ANSWER, m_answer);
	DDX_Control(pDX, IDC_PRG, m_prg);
	DDX_Control(pDX, IDC_VARS, m_var);
	DDX_Control(pDX, IDC_GIVEN, m_giv);
	DDX_Text(pDX, IDC_FUNC, m_func);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKinRevDlg, CDialog)
	//{{AFX_MSG_MAP(CKinRevDlg)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_BN_CLICKED(IDC_DEL, OnDel)
	ON_NOTIFY(NM_DBLCLK, IDC_GIVEN, OnDblclkGiven)
	ON_BN_CLICKED(IDC_LIST_BTN, OnListBtn)
	ON_BN_CLICKED(IDC_CURR_BTN, OnCurrBtn)
	ON_BN_CLICKED(IDC_EDITVAR, OnEditvar)
	ON_NOTIFY(NM_DBLCLK, IDC_VARS, OnDblclkVars)
	ON_BN_CLICKED(IDC_SOLVE, OnSolve)
	ON_BN_CLICKED(IDC_MOVE, OnMove)
	ON_BN_CLICKED(IDC_DIRECT, OnDirect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKinRevDlg message handlers

BOOL CKinRevDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	Gq = 0;
	///GIVEN TABLE
	m_giv.InsertColumn(0,"Выражение",LVCFMT_LEFT,226);
	m_giv.InsertColumn(1,"Значение",LVCFMT_LEFT,64);
	m_giv.InsertColumn(2,"Штраф",LVCFMT_LEFT,48);
	///VAR TABLE
	m_var.InsertColumn(0,"Переменная",LVCFMT_LEFT,88);
	m_var.InsertColumn(1,"Значение",LVCFMT_LEFT,64);
	m_var.InsertColumn(2,"Штраф",LVCFMT_LEFT,48);
	///ANSWER TABLE
	m_answer.InsertColumn(0,"Переменная",LVCFMT_LEFT,88);
	m_answer.InsertColumn(1,"Значение",LVCFMT_LEFT,64);

	UpdateAll();
	return TRUE;
}

void CKinRevDlg::OnAdd() 
{
  /*
	CChangeGivenDlg dlg;
	dlg.par=3;
	dlg.sys=core().link_count();
	dlg.ins=1;
	dlg.pen=10.0f;
	dlg.val=0;
	if (dlg.DoModal()!=IDOK) return;
	given.push_back(SGiven());
  given[Gq].expr=dlg.expr;
	given[Gq].pen=dlg.pen;
	given[Gq].val=dlg.val;
	given[Gq].par=dlg.par;
	given[Gq].sys=dlg.sys;
	given[Gq].ins=dlg.ins;
	Gq++;	
	UpdateAll();
  */
}

void CKinRevDlg::UpdateGiven()
{
	m_giv.DeleteAllItems();
	LVITEM lvi;
	lvi.mask=LVIF_TEXT;
	CString st;
	for (Size g=0;g<Gq;g++)
	{
		st=given[g].expr.GetAsStr().c_str();
		int ai=m_giv.InsertItem(g,st);
		lvi.iItem=ai;
		lvi.iSubItem=1;
		st.Format("%g",given[g].val);
		lvi.pszText=(char*)(LPCSTR)st;
		m_giv.SetItem(&lvi);
		lvi.iSubItem=2;
		st.Format("%g",given[g].pen);
		lvi.pszText=(char*)(LPCSTR)st;
		m_giv.SetItem(&lvi);
	}
}

void CKinRevDlg::OnEdit() 
{
/*
  POSITION pos = m_giv.GetFirstSelectedItemPosition();
	if (!pos) return;
	int num=m_giv.GetNextSelectedItem(pos);	
	CChangeGivenDlg dlg;
	dlg.par=given[num].par;
	dlg.sys=given[num].sys;
	dlg.ins=given[num].ins;
	dlg.pen=given[num].pen;
	dlg.val=given[num].val;
	if (dlg.DoModal()!=IDOK) return;
	given[num].expr=dlg.expr;
	given[num].pen=dlg.pen;
	given[num].val=dlg.val;
	given[num].par=dlg.par;
	given[num].sys=dlg.sys;
	given[num].ins=dlg.ins;
	UpdateAll();
  */
}

void CKinRevDlg::OnDel() 
{
	POSITION pos = m_giv.GetFirstSelectedItemPosition();
	if (!pos) return;
	int num=m_giv.GetNextSelectedItem(pos);	
  std::vector<SGiven>::iterator it = given.begin();
  std::advance(it, num);
  given.erase(it);
	Gq--;
	UpdateAll();
}

void CKinRevDlg::OnDblclkGiven(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnEdit();	
	*pResult = 0;
}

void CKinRevDlg::OnListBtn() 
{
	CParamsDlg dlg;
	dlg.DoModal();
}

void CKinRevDlg::UpdateVars()
{
	//CRATING NEW
	SRParamsTable * pPT = core().rParamsTable();
  Vars vars_new;
	//vars_new.resize(core().link_count());
	for (Size v=0;v<pPT->pVs.size();v++)
	{
		if (IsVarPresentInGiven(pPT->pVs[v].var))
		{
			SOVar new_var;
      new_var.max = pPT->pVs[v].max;
			new_var.min = pPT->pVs[v].min;
			new_var.pen =0;
			new_var.start=core().link(v).m_curd+pPT->pVs[v].def;
			new_var.var=pPT->pVs[v].var;
      vars_new.push_back(new_var);
		}
	}
	
	//COPYNG PENALTIES
	for (int v = 0; v < vars_new.size(); v++)
	{
		for (Size vo = 0; vo < m_data.vars.size(); ++vo)
			if (vars_new[v].var == m_data.vars[vo].var)
			{
				vars_new[v].pen = m_data.vars[vo].pen;
				vars_new[v].start = m_data.vars[vo].start;
				break;
			}
	}

	//OVERWRITING	
  m_data.vars.swap(vars_new);
/*
  for (v=0;v<Vq;v++)
		delete pVars[v];
	memcpy(pVars,pVarsNew,4*MAX_LINKS);
  */
	
	//OUTPUT
	m_var.DeleteAllItems();
	LVITEM lvi;
	lvi.mask=LVIF_TEXT;
	CString st;
	for (int v = 0; v < m_data.vars.size(); v++)
	{
		st=m_data.vars[v].var.GetName().c_str();
		int ai=m_var.InsertItem(v,st);
		lvi.iItem=ai;
		lvi.iSubItem=1;
		st.Format("%g",m_data.vars[v].start);
		lvi.pszText=(char*)(LPCSTR)st;
		m_var.SetItem(&lvi);
		lvi.iSubItem=2;
		st.Format("%g",m_data.vars[v].pen);
		lvi.pszText=(char*)(LPCSTR)st;
		m_var.SetItem(&lvi);
	}
}

void CKinRevDlg::UpdateAll()
{
	UpdateGiven();
	UpdateVars();
	UpdateFunc();
}

void CKinRevDlg::UpdateFunc()
{
	m_data.func=0.0f;
	for (Size g=0;g<Gq;g++)
		m_data.func+=((given[g].expr-given[g].val).absoluted())*given[g].pen;
	
	MSymb s;
	for (Size v=0;v<m_data.vars.size();v++)
	{
		s = ((MSymb(m_data.vars[v].var) - MSymb(m_data.vars[v].start))).absoluted() * m_data.vars[v].pen;
		m_data.func += s;
	}
	
	m_func=CString(m_data.func.GetAsStr().c_str())+" -> min";
	UpdateData(false);
}

void CKinRevDlg::OnCurrBtn() 
{
	SRParamsTable * pPT = core().rParamsTable();
	for (Size v=0;v<m_data.vars.size();v++)
	{
		for (Size vo=0;vo<pPT->pVs.size();vo++)
			if (m_data.vars[v].var==pPT->pVs[vo].var)
			{
				m_data.vars[v].start=core().link(vo).m_curd+pPT->pVs[vo].def;
				break;
			}
	}
	UpdateAll();
}

bool CKinRevDlg::IsVarPresentInGiven(MVar &v)
{
	for (Size g=0;g<Gq;g++)
		if (given[g].expr.IsVarPresent(v))
			return true;
	return false;
}

void CKinRevDlg::OnEditvar() 
{
	POSITION pos = m_var.GetFirstSelectedItemPosition();
	if (pos)
	{	
		int num=m_var.GetNextSelectedItem(pos);
		CEditVarsDlg dlg(this);
		dlg.pen=m_data.vars[num].pen;
		dlg.val=m_data.vars[num].start;
		if (dlg.DoModal()!=IDOK)
			return;
		m_data.vars[num].pen=dlg.pen;
		m_data.vars[num].start=dlg.val;
		UpdateAll();
	}
}

void CKinRevDlg::OnDblclkVars(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnEditvar();	
	*pResult = 0;
}

void CKinRevDlg::OnSolve() 
{
	//float err;
	m_val.SetWindowText("Вычисляется ...");
	m_data.init();
  float max_e = m_data.func.solve(m_data.given);
  m_prg.SetRange32(0, int(max_e));
  m_prg.SetPos(0);
  while (!core().drOptimize(m_data))
  {
    m_prg.SetPos(int(max_e - m_data.e));
  }
  //core().drOptimize(Func,&vars.front(),Vq,err/*,&m_prg*/);	
	CString s;
	s.Format("Значение целевой функции %g",m_data.e);
	m_val.SetWindowText(s);
	UpdateAnswer();
}

void CKinRevDlg::UpdateAnswer()
{
	m_answer.DeleteAllItems();
	LVITEM lvi;
	lvi.mask=LVIF_TEXT;
	CString st;
	for (Size v=0;v<m_data.vars.size();v++)
	{
		st=m_data.vars[v].var.GetName().c_str();
		int ai=m_answer.InsertItem(v,st);
		lvi.iItem=ai;
		lvi.iSubItem=1;
		st.Format("%g",m_data.vars[v].end);
		lvi.pszText=(char*)(LPCSTR)st;
		m_answer.SetItem(&lvi);
	}
}

void CKinRevDlg::OnMove() 
{
	SRParamsTable * pPT = core().rParamsTable();
	for (Size v=0;v<m_data.vars.size();v++)
	{
		for (Size vo=0;vo<pPT->pVs.size();vo++)
			if (m_data.vars[v].var==pPT->pVs[vo].var)
			{
				core().link(vo).m_curd=m_data.vars[v].end-pPT->pVs[vo].def;
				break;
			}
	}	
	pRD->UpdateAllViews(NULL);	
}

void CKinRevDlg::OnDirect() 
{
	CKinDirDlg dlg;
	dlg.DoModal();
}
