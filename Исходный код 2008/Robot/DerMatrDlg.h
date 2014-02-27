#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDerMatrDlg dialog

class CDerMatrDlg : public CDialog
{
	DECLARE_DYNAMIC(CDerMatrDlg)

public:
	CDerMatrDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDerMatrDlg();

// Dialog Data
	enum { IDD = IDD_DERIV_MATR_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void UpdateMatrix(int n, int m);
protected:
	//MMatrix<MSymb> *CRobotDoc::rMatriceUij(int n, int m);// * pfMatriceUij(int n, int m);
	//CRobotDoc::SRMatricesAi * pAi;
	MMatrix<MSymb> matrix;
public:
	//CSpinButtonCtrl m_spin1;
	//CSpinButtonCtrl m_spin2;
	CComboBox m_link_c;
	CComboBox m_coord_c;
	
//	afx_msg void OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl m_mtr;
	afx_msg void OnCbnSelchangeCoordinate();
	afx_msg void OnCbnSelchangeLink();
};
