#pragma once


// CValueDlg dialog

class CValueDlg : public CDialog
{
	DECLARE_DYNAMIC(CValueDlg)

public:
	CValueDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CValueDlg();

// Dialog Data
	enum { IDD = IDD_VALUE_DLG };
  CString data;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
  CString m_value;
  CEdit   m_value_c;
public:
  virtual BOOL OnInitDialog();
protected:
  virtual void OnOK();
};
