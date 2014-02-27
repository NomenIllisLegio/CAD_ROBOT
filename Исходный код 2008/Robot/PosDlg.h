#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CPosDlg dialog

class CPosDlg : public CDialog
{
	DECLARE_DYNAMIC(CPosDlg)

public:
	CPosDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPosDlg();

  void update(int power);

// Dialog Data
	enum { IDD = IDD_POS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  void small_update();
  void big_update();

  CListCtrl m_pos;
  CListBox m_store;
  SRParamsTable* r_params;

	DECLARE_MESSAGE_MAP()
public:
  virtual BOOL OnInitDialog();
  afx_msg void OnBnClickedApply();
  afx_msg void OnBnClickedRename();
  afx_msg void OnBnClickedSave();
  afx_msg void OnBnClickedDelete();
  afx_msg void OnNMDblclkPos(NMHDR *pNMHDR, LRESULT *pResult);
};
