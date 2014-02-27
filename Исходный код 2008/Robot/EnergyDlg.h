#pragma once
#include "afxcmn.h"


// CEnergyDlg dialog

class CEnergyDlg : public CDialog
{
	DECLARE_DYNAMIC(CEnergyDlg)

public:
	CEnergyDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEnergyDlg();

// Dialog Data
	enum { IDD = IDD_DYN_ENERGY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
  CListCtrl m_eners;
public:
  virtual BOOL OnInitDialog();
};
