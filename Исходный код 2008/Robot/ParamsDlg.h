#if !defined(AFX_PARAMSDLG_H__7F8B99C4_7487_4AFC_B38B_9AC709DAF0AA__INCLUDED_)
#define AFX_PARAMSDLG_H__7F8B99C4_7487_4AFC_B38B_9AC709DAF0AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParamsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CParamsDlg dialog

class CParamsDlg : public CDialog
{
// Construction
public:
	CParamsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CParamsDlg)
	enum { IDD = IDD_PARAMS_DLG };
	CListCtrl	m_val;
	CListCtrl	m_pt;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParamsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CParamsDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARAMSDLG_H__7F8B99C4_7487_4AFC_B38B_9AC709DAF0AA__INCLUDED_)
