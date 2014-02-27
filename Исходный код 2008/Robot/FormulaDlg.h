#if !defined(AFX_FORMULADLG_H__E5E7CF74_6C81_4BA5_AE6F_1DD86F6BA084__INCLUDED_)
#define AFX_FORMULADLG_H__E5E7CF74_6C81_4BA5_AE6F_1DD86F6BA084__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FormulaDlg.h : header file
//

#define FORM_MAX_FORM 32

/////////////////////////////////////////////////////////////////////////////
// CFormulaDlg dialog

class CFormulaDlg : public CDialog
{
// Construction
public:
	void AddForm(int a, int t);
	void UpdateEdit();
	CFormulaDlg(CWnd* pParent = NULL);   // standard constructor
	struct SForm
	{
		int Axis;
		int LinkType;
	} Fs[FORM_MAX_FORM];
	Size Fq;
	CRobotDoc * pRD;
// Dialog Data
	//{{AFX_DATA(CFormulaDlg)
	enum { IDD = IDD_FORMULA_DLG };
	CEdit	m_edit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormulaDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFormulaDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBMx();
	afx_msg void OnBRx();
	afx_msg void OnBMy();
	afx_msg void OnIdcBRy();
	afx_msg void OnBRy();
	afx_msg void OnBMz();
	afx_msg void OnBRz();
	afx_msg void OnBDel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMULADLG_H__E5E7CF74_6C81_4BA5_AE6F_1DD86F6BA084__INCLUDED_)
