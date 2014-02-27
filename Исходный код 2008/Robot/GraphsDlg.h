#pragma once
#include "afxwin.h"


// CGraphsDlg dialog

class CGraphsDlg : public CDialog
{
	DECLARE_DYNAMIC(CGraphsDlg)

public:
	CGraphsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGraphsDlg();

// Dialog Data
	enum { IDD = IDD_GRAPHS_DLG };

  virtual BOOL OnInitDialog();

  Spline* func;
  Spline* func_2;
  
  CString x_units;
  CString y_units;
  CString caption;

protected:
	void draw_graph();
    
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  //float m_max_x;
  //float m_max_y;

  CWnd m_graph;
  CStatic m_caption;

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedGo();
  afx_msg void OnPaint();
};
