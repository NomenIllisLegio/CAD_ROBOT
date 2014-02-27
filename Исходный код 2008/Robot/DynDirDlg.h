#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DynDirDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDynDirDlg dialog

class CDynDirDlg : public CDialog
{
// Construction
public:
	CString GetLabelForMember(int m, int s);
	bool FormMatrix(int s);
	void UpdateReport();
	MSymb * pGiven;
	MMatrix<MSymb> mt;
	MWorkspace WS;
	SRParamsTable * pPT;
	SRMatricesAi * pAi;
	void LoadDefaultGiven();
	void UpdateGiven();
	CDynDirDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDynDirDlg)
	enum { IDD = IDD_DYNDIR_DLG };
	CButton	m_move;
	CListCtrl	m_rep;
	CComboBox	m_syst;
	CListCtrl	m_giv;
	int		m_iv;
	int		m_sv;
	BOOL	m_substate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDynDirDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDynDirDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnClickGiven(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDefault();
	afx_msg void OnSelchangeSyst();
	afx_msg void OnSelchangeInsyst();
	afx_msg void OnCoordinates();
	afx_msg void OnCurrent();
	afx_msg void OnClickSubstate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

