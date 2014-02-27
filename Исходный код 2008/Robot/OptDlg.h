#if !defined(AFX_OPTDLG_H__B70FF930_7CBB_4561_9DF8_BB29D57D073D__INCLUDED_)
#define AFX_OPTDLG_H__B70FF930_7CBB_4561_9DF8_BB29D57D073D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptDlg dialog

class COptDlg : public CDialog
{
// Construction
public:
	COptDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COptDlg)
	enum { IDD = IDD_OPT_DLG };
	float	m_mot;
	float	m_rot;
	BOOL	m_bord;
	BOOL	m_use;
	float	m_kr_pr;
	int		m_kr_div;
	BOOL	m_kr_use;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COptDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
  float m_x;
  float m_y;
  float m_z;
public:
  virtual BOOL OnInitDialog();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTDLG_H__B70FF930_7CBB_4561_9DF8_BB29D57D073D__INCLUDED_)
