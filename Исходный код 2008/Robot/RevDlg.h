#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CRevDlg dialog

class CRevDlg : public CDialog
{
	DECLARE_DYNAMIC(CRevDlg)

public:
	CRevDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRevDlg();

// Dialog Data
	enum { IDD = IDD_REV_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
  
  void redraw_given();
  int selected_given() const;
  void form_given();
  void on_change_given();

  OptData   m_data;
  bool      m_given_actual;

  CListCtrl m_given;
  CProgressCtrl m_progress;
  CStatic m_info;
  CButton m_move;
  CButton m_save;

public:
  virtual BOOL OnInitDialog();
  afx_msg void OnBnClickedPosition();
  afx_msg void OnBnClickedEdit();
  afx_msg void OnBnClickedRemove();
  afx_msg void OnNMDblclkGiven(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnBnClickedSolve();
  afx_msg void OnBnClickedMove();
  afx_msg void OnBnClickedView();
};
