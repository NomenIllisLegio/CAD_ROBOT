#include "afxwin.h"
#if !defined(AFX_EDITLINKDLG_H__053631F0_F197_4BFD_AE3C_54FB28063500__INCLUDED_)
#define AFX_EDITLINKDLG_H__053631F0_F197_4BFD_AE3C_54FB28063500__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditLinkDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditLinkDlg dialog

class CEditLinkDlg : public CDialog
{
// Construction
public:
	void update_inert();
  void UpdateMotions();
	void Init();
	void Apply();
	void UpdateGeo();
	void UpdateAxis(bool allCustom);
	CEditLinkDlg(CWnd* pParent = NULL);   // standard constructor
	int link;
	Link* pL;
	
	//Values
	V3f Orient;
// Dialog Data
	//{{AFX_DATA(CEditLinkDlg)
	enum { IDD = IDD_EDIT_LINK_DLG };
	CEdit	m_m_min_c;
	CEdit	m_m_max_c;
	CEdit	m_s_min_c;
	CEdit	m_s_max_c;
	CEdit	m_a_min_c;
	CEdit	m_a_max_c;
  CStatic	m_m_lab;
	CButton	m_prev;
	CButton	m_next;
	CStatic	m_index;
	CEdit	m_lzc;
	CEdit	m_lyc;
	CEdit	m_lxc;
	CEdit	m_zc;
	CEdit	m_yc;
	CEdit	m_xc;
	CStatic	m_li;
	CStatic	m_lt;
	CString	m_linkname;
	float	m_xe;
	float	m_ye;
	float	m_ze;
	float	m_lx;
	float	m_ly;
	float	m_lz;
	float	m_m_max_v;
	float	m_m_min_v;
	float	m_s_max_v;
	float	m_s_min_v;
	float	m_a_max_v;
	float	m_a_min_v;
  float m_length;
  float m_radius;
  float m_mass;
  CEdit m_length_c;
  CEdit m_radius_c;
  CEdit m_mass_c;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditLinkDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditLinkDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnAxisX();
	afx_msg void OnAxisY();
	afx_msg void OnAxisZ();
	afx_msg void OnAxisCustom();
	afx_msg void OnLinkAuto();
	afx_msg void OnApply();
	afx_msg void OnPrev();
	afx_msg void OnNext();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CStatic m_speed_lab;
  CStatic m_acc_lab;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITLINKDLG_H__053631F0_F197_4BFD_AE3C_54FB28063500__INCLUDED_)
