#if !defined(AFX_MATRDLG_H__FCFFBDC4_0EE6_4E2C_B08C_7904768DA31C__INCLUDED_)
#define AFX_MATRDLG_H__FCFFBDC4_0EE6_4E2C_B08C_7904768DA31C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MatrDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMatrDlg dialog

class CMatrDlg : public CDialog
{
// Construction
public:
	void UpdateFirst();
	void UpdateMatrix(int s, int e);
	CMatrDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMatrDlg)
	enum { IDD = IDD_MATR_DLG };
	CComboBox	m_tctr;
	CComboBox	m_fctr;
	CComboBox	m_mean_ctr;
  CListCtrl	m_mtr;
	int		m_fval;
	int		m_tval;
  int		m_mean_val;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMatrDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	SRMatricesAi * pAi;
	// Generated message map functions
	//{{AFX_MSG(CMatrDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeFrom();
	afx_msg void OnSelchangeTo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

  void create_matrix();
  void draw_matrix();

  MMatrix<MSymb>  m_mt;
  bool            m_m_valid;
public:
  afx_msg void OnCbnSelchangeMean();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MATRDLG_H__FCFFBDC4_0EE6_4E2C_B08C_7904768DA31C__INCLUDED_)
