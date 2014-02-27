// robot.h : main header file for the ROBOT application
//

#if !defined(AFX_ROBOT_H__045FA1E8_2662_4E19_A554_64B82E631F28__INCLUDED_)
#define AFX_ROBOT_H__045FA1E8_2662_4E19_A554_64B82E631F28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "mainfrm.h"
#include "robotdoc.h"
#include "robotview.h"
/////////////////////////////////////////////////////////////////////////////
// CRobotApp:
// See robot.cpp for the implementation of this class
//

class CRobotApp : public CWinApp
{
public:
	CRobotApp();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRobotApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CRobotApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
  virtual BOOL OnIdle(LONG lCount);
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROBOT_H__045FA1E8_2662_4E19_A554_64B82E631F28__INCLUDED_)
