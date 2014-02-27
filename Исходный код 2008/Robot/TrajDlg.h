#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CTrajDlg dialog

class CTrajDlg : public CDialog
{
	DECLARE_DYNAMIC(CTrajDlg)

public:
	CTrajDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTrajDlg();

// Dialog Data
	enum { IDD = IDD_TRAJ_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

  void redraw_given();
  int selected_point() const;

  CProgressCtrl m_progr;
  CListBox m_vars_c;
  CListCtrl m_points;

  TrajData data;

public:
  virtual BOOL OnInitDialog();
  afx_msg void OnBnClickedAdd();
  afx_msg void OnBnClickedEdit();
  afx_msg void OnBnClickedRemove();
  afx_msg void OnBnClickedStart();
  afx_msg void OnBnClickedView();
  CButton m_start_c;
  CButton m_view;
};
