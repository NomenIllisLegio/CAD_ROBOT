// robotDoc.h : interface of the CRobotDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROBOTDOC_H__577E24C8_947F_48A1_8697_7674CEC1F62C__INCLUDED_)
#define AFX_ROBOTDOC_H__577E24C8_947F_48A1_8697_7674CEC1F62C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../baselib/base.h"
#include "../CoreLib/core.h"

namespace modify
{
const int none =  0;
const int light = 1;
const int big =   2;
}

class CRobotDoc : public CDocument
{
protected: // create from serialization only
	CRobotDoc();
	DECLARE_DYNCREATE(CRobotDoc)

// Attributes
public:
	struct SHeader
	{
		int SelLink;	//выбранное звено
	} H;

  struct SSettings
	{
		int Mode;	//режим вывода 0-2D 1-3D
		int Scale;	//mastab;
		bool Center;//центровка
		bool aAxis;		 //показывть оси	 
	} S;
	////REPORTS
  
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRobotDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRobotDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
  void Init();
  bool selection_valid() const;
  Link& selected_link() const;
  void select_link(int link);
  int sel_index() const;
  void on_modify(int power);
protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRobotDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROBOTDOC_H__577E24C8_947F_48A1_8697_7674CEC1F62C__INCLUDED_)
