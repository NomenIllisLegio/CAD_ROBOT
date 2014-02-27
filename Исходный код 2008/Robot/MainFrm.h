// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__BEFDA296_861C_4453_866D_AA90FCC8E275__INCLUDED_)
#define AFX_MAINFRM_H__BEFDA296_861C_4453_866D_AA90FCC8E275__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRobotDoc;

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	void EditLink(int Index);
	void ReleaseResources();
	bool UploadResources();
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndStdTB;
	CToolBar    m_wndConstructTB;

public:
	void UpdateFrameByDoc();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnAddLink();
	afx_msg void OnViewModes2d();
	afx_msg void OnViewModes3d();
	afx_msg void OnDelLink();
	afx_msg void OnPropsLink();
	afx_msg void OnMastLess();
	afx_msg void OnMastMore();
	afx_msg void OnMastNorm();
	afx_msg void OnViewCenter();
	afx_msg void OnEditFormula();
	afx_msg void OnAnlsParams();
	afx_msg void OnAnlsOptions();
	afx_msg void OnAnlsMatrices();
	afx_msg void OnAnlsKinDir();
//	afx_msg void OnAnlsKinRev();
	afx_msg void OnViewRefresh();
	afx_msg void OnEditSelect();
	afx_msg void OnCtrlMoveDefault();
	afx_msg void OnCtrlForv();
	afx_msg void OnCtrlBack();
	afx_msg void OnEditPrev();
	afx_msg void OnEditNext();
	afx_msg void OnAnlsAxis();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnAnlsDerMatrices();
	afx_msg void OnUpdateAnlsParams(CCmdUI *pCmdUI);
  afx_msg void OnUpdateViewModes3d(CCmdUI *pCmdUI);
  afx_msg void OnUpdateEditAdd(CCmdUI *pCmdUI);
  afx_msg void OnUpdateEditDel(CCmdUI *pCmdUI);
  afx_msg void OnUpdateEditNext(CCmdUI *pCmdUI);
  afx_msg void OnUpdateEditPrev(CCmdUI *pCmdUI);
  afx_msg void OnUpdateEditProps(CCmdUI *pCmdUI);
  afx_msg void OnUpdateViewCenter(CCmdUI *pCmdUI);
  afx_msg void OnUpdateCtrlBack(CCmdUI *pCmdUI);
  afx_msg void OnUpdateCtrlForv(CCmdUI *pCmdUI);
  afx_msg void OnUpdateCtrlMoveDefault(CCmdUI *pCmdUI);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__BEFDA296_861C_4453_866D_AA90FCC8E275__INCLUDED_)
