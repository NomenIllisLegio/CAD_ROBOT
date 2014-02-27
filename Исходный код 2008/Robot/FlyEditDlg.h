#if !defined(AFX_FLYEDITDLG_H__A49E8CA3_772B_4EAE_A071_CB53DF2A08CB__INCLUDED_)
#define AFX_FLYEDITDLG_H__A49E8CA3_772B_4EAE_A071_CB53DF2A08CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FlyEditDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFlyEditDlg dialog

class CFlyEditDlg : public CDialog
{
// Construction
public:
	CFlyEditDlg(CWnd* pParent = NULL);   // standard constructor
	CRect rect;
	CWnd * pPW;
	CString Data;
// Dialog Data
	//{{AFX_DATA(CFlyEditDlg)
	enum { IDD = IDD_FLYEDIT_DLG };
	CEdit	m_edit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlyEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFlyEditDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLYEDITDLG_H__A49E8CA3_772B_4EAE_A071_CB53DF2A08CB__INCLUDED_)
