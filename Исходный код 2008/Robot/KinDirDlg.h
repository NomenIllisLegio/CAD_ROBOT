#if !defined(AFX_KINDIRDLG_H__2C02E1E6_48EC_4A3C_B3B6_3D3B531BBDED__INCLUDED_)
#define AFX_KINDIRDLG_H__2C02E1E6_48EC_4A3C_B3B6_3D3B531BBDED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KinDirDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKinDirDlg dialog

class CKinDirDlg : public CDialog
{
// Construction
public:
	void form_matrix(int s, int e);
	
	MMatrix<MSymb> m_mt;
  MMatrix<MSymb> m_mt_v;
  MMatrix<MSymb> m_mt_c;
	SRParamsTable * pPT;
	SRMatricesAi * pAi;
	CKinDirDlg(CWnd* pParent = NULL);   // standard constructor
  MGiven m_given;
  bool   m_mt_valid;

  //void full_update();
  void update(int power);
// Dialog Data
	//{{AFX_DATA(CKinDirDlg)
	enum { IDD = IDD_KINDIR_DLG };
	CListCtrl	m_rep;
	CComboBox	m_syst;
	CComboBox	m_insyst;
	int		m_iv;
	int		m_sv;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKinDirDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

  void small_update();
  void big_update();

	// Generated message map functions
	//{{AFX_MSG(CKinDirDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeSyst();
	afx_msg void OnSelchangeInsyst();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KINDIRDLG_H__2C02E1E6_48EC_4A3C_B3B6_3D3B531BBDED__INCLUDED_)
