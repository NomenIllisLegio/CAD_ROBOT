#include "afxwin.h"
#if !defined(AFX_CHANGEGIVENDLG_H__288A3C94_5C89_47CA_A214_1A937F61003C__INCLUDED_)
#define AFX_CHANGEGIVENDLG_H__288A3C94_5C89_47CA_A214_1A937F61003C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChangeGivenDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChangeGivenDlg dialog

class CChangeGivenDlg : public CDialog
{
// Construction
public:
	CChangeGivenDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CChangeGivenDlg)
	enum { IDD = IDD_EDITGIVEN_DLG };
	CComboBox	m_param;
	CComboBox	m_syst;
	CComboBox	m_insyst;
	int		m_iv;
	int		m_pv;
	int		m_sv;
	float	m_pen;
	float	m_val;
	//}}AFX_DATA
	//int par,sys,ins;
	//float val,pen;
	//MSymb expr;
  RevArg arg;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChangeGivenDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChangeGivenDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
  CComboBox m_type_c;
  int m_type;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHANGEGIVENDLG_H__288A3C94_5C89_47CA_A214_1A937F61003C__INCLUDED_)
