#pragma once
#include "afxwin.h"


// CWideView dialog

class CWideView : public CDialog
{
	DECLARE_DYNAMIC(CWideView)

public:
	CWideView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWideView();

  CString text;

// Dialog Data
	enum { IDD = IDD_WIDE_VIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  virtual BOOL OnInitDialog();
private:
  CEdit m_edit;
};
