#pragma once
#include "afxcmn.h"


// CDinRevDlg dialog

class CDinRevDlg : public CDialog
{
	DECLARE_DYNAMIC(CDinRevDlg)

public:
	CDinRevDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDinRevDlg();

// Dialog Data
	enum { IDD = IDD_DYN_REV };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
  CListCtrl m_powers;
public:
  virtual BOOL OnInitDialog();
};
