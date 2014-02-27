#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDynSpdDlg dialog

class CDynSpdDlg : public CDialog
{
	DECLARE_DYNAMIC(CDynSpdDlg)

public:
	CDynSpdDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDynSpdDlg();

// Dialog Data
	enum { IDD = IDD_DYN_SPEED_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void UpdateReport();
	CComboBox m_syst;
	CListCtrl m_sol;
	afx_msg void OnCbnSelchangeSyst();
	CListCtrl m_rep;
	afx_msg void OnBnClickedEditLinks();
};
