#if !defined(AFX_EDITVARSDLG_H__A93B3410_BDBA_4CF0_B38A_076834840714__INCLUDED_)
#define AFX_EDITVARSDLG_H__A93B3410_BDBA_4CF0_B38A_076834840714__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditVarsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditVarsDlg dialog

class CEditVarsDlg : public CDialog
{
// Construction
public:
	CEditVarsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEditVarsDlg)
	enum { IDD = IDD_EDITVARS_DLG };
	float	m_pen;
	float	m_val;
	//}}AFX_DATA
	float val,pen;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditVarsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditVarsDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITVARSDLG_H__A93B3410_BDBA_4CF0_B38A_076834840714__INCLUDED_)
