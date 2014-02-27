#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CInertMatrDlg dialog

struct SRMatricesHi;

class CInertMatrDlg : public CDialog
{
	DECLARE_DYNAMIC(CInertMatrDlg)

public:
	CInertMatrDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInertMatrDlg();

// Dialog Data
	enum { IDD = IDD_INERT_MATR_DLG };

	virtual BOOL OnInitDialog();
	void UpdateMatrix(int n);
	afx_msg void OnCbnSelchangeLink();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	MMatrix<float> matrix;
	CComboBox     m_link_c;
	CListCtrl     m_mtr;
  SRMatricesHi* m_hi;

  DECLARE_MESSAGE_MAP()
};
