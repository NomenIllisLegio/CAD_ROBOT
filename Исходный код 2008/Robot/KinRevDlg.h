#if !defined(AFX_KINREVDLG_H__87803450_3CFB_44ED_8A42_86B45E2330D0__INCLUDED_)
#define AFX_KINREVDLG_H__87803450_3CFB_44ED_8A42_86B45E2330D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KinRevDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKinRevDlg dialog

class CKinRevDlg : public CDialog
{
// Construction
public:
	void UpdateAnswer();
	bool IsVarPresentInGiven(MVar & v);
	void UpdateFunc();
	void UpdateAll();
	void UpdateVars();
	void UpdateGiven();
	CKinRevDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CKinRevDlg)
	enum { IDD = IDD_KINREV_DLG };
	CStatic	m_val;
	CListCtrl	m_answer;
	CProgressCtrl	m_prg;
	CListCtrl	m_var;
	CListCtrl	m_giv;
	CString	m_func;
	//}}AFX_DATA
	struct SGiven
	{
		MSymb expr;
		float val,pen;
		int par,sys,ins;
	};
  std::vector<SGiven> given;
  //std::vector<SOVar>  vars;
	//MSymb Func;
	Size Gq/*,Vq*/;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKinRevDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
  OptData m_data;

	// Generated message map functions
	//{{AFX_MSG(CKinRevDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnEdit();
	afx_msg void OnDel();
	afx_msg void OnDblclkGiven(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnListBtn();
	afx_msg void OnCurrBtn();
	afx_msg void OnEditvar();
	afx_msg void OnDblclkVars(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSolve();
	afx_msg void OnMove();
	afx_msg void OnDirect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KINREVDLG_H__87803450_3CFB_44ED_8A42_86B45E2330D0__INCLUDED_)
