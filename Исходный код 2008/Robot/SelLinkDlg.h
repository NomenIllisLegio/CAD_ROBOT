#if !defined(AFX_SELLINKDLG_H__9B7C59F0_2C6F_4F6E_9C50_1106BAC13E76__INCLUDED_)
#define AFX_SELLINKDLG_H__9B7C59F0_2C6F_4F6E_9C50_1106BAC13E76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelLinkDlg.h : header file
//

//extern SLinkData LDs[3];

/////////////////////////////////////////////////////////////////////////////
// CSelLinkDlg dialog

class CSelLinkDlg : public CDialog
{
// Construction
public:
	CSelLinkDlg(CWnd* pParent = NULL);   // standard constructor
	int iType;
	CImageList * pIL;
	CString * pLNs;
// Dialog Data
	//{{AFX_DATA(CSelLinkDlg)
	enum { IDD = IDD_SEL_LINK_DLG };
	CListCtrl	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelLinkDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelLinkDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkLinklist(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELLINKDLG_H__9B7C59F0_2C6F_4F6E_9C50_1106BAC13E76__INCLUDED_)
