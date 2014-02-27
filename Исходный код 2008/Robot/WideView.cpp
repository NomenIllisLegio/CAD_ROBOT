// WideView.cpp : implementation file
//

#include "stdafx.h"
#include "robot.h"
#include "WideView.h"
#include ".\wideview.h"


// CWideView dialog

IMPLEMENT_DYNAMIC(CWideView, CDialog)
CWideView::CWideView(CWnd* pParent /*=NULL*/)
	: CDialog(CWideView::IDD, pParent)
{
}

CWideView::~CWideView()
{
}

void CWideView::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_EDIT, m_edit);
}


BEGIN_MESSAGE_MAP(CWideView, CDialog)
END_MESSAGE_MAP()


// CWideView message handlers

BOOL CWideView::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_edit.SetWindowText(text);

  return TRUE;
}
