// RobotView.h : interface of the CRobotView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROBOTVIEW_H__512EB6C2_E6D5_4AA7_A085_4891146FA883__INCLUDED_)
#define AFX_ROBOTVIEW_H__512EB6C2_E6D5_4AA7_A085_4891146FA883__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CKinDirDlg;
class CPosDlg;

class CRobotView : public CView
{
protected: // create from serialization only
	CRobotView();
	DECLARE_DYNCREATE(CRobotView)

// Attributes
public:
	CRobotDoc* GetDocument();
	XMatrix<4,4,double> mProj;//projection matrix
	MMatrix<double> sTrans;//init trans;
	SRCoordinateSystems * pRCSs;
	SRStaticTMs * pTMs;
	SRStaticTMs * pLTMs;
	SRParamsTable * pPTs;
// Operations
public:
//pointers
	CRobotDoc * pRD;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRobotView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	void InitialModelView();
	void RenderCoordSystems();
	void DrawCoordSystem(bool selected) const;
  void DrawArrow() const;
  void DrawOrt() const;
	void DrawElement(int type, int number) const;
	void Draw();
	//void RenderLink(int Link);
	void RenderLinks();
	void Prepare();
	virtual ~CRobotView();

  void on_update(int power);
  void on_tick();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
  struct NMDialogs
  {
    CKinDirDlg* kin_dir;
    bool        kin_dir_shown;
    CPosDlg*    pos;
    bool        pos_shown;
  };

  void update_dlgs(int power);
  DWORD cur_time() const;

  NMDialogs m_dlgs;

  bool        m_first_start;
  bool        m_playing;
  float       m_time;
  DWORD       m_old_time;

// Generated message map functions
protected:
	//{{AFX_MSG(CRobotView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnDestroy();
  afx_msg void OnAnlsKinDir();
  afx_msg void OnUpdateAnlsKinDir(CCmdUI *pCmdUI);
  afx_msg void OnAnlsKinRev();
  afx_msg void OnUpdateAnlsKinRev(CCmdUI *pCmdUI);
  afx_msg void OnAnlsTrajectory();
  afx_msg void OnUpdateAnlsTrajectory(CCmdUI *pCmdUI);
  afx_msg void OnAnlsMatrInert();
  afx_msg void OnCtrlDoTraj();
  afx_msg void OnUpdateCtrlDoTraj(CCmdUI *pCmdUI);
  afx_msg void OnAnlsDynRev();
  afx_msg void OnUpdateAnlsAxis(CCmdUI *pCmdUI);
  afx_msg void OnUpdateAnlsDerMatrices(CCmdUI *pCmdUI);
  afx_msg void OnUpdateAnlsMatrInert(CCmdUI *pCmdUI);
  afx_msg void OnUpdateAnlsMatrices(CCmdUI *pCmdUI);
  afx_msg void OnUpdateAnlsDynRev(CCmdUI *pCmdUI);
  afx_msg void OnAnlsEnergy();
  afx_msg void OnUpdateAnlsEnergy(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // debug version in RobotView.cpp
inline CRobotDoc* CRobotView::GetDocument()
   { return (CRobotDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROBOTVIEW_H__512EB6C2_E6D5_4AA7_A085_4891146FA883__INCLUDED_)
