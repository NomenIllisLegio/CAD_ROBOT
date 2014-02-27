// robotDoc.cpp : implementation of the CRobotDoc class
//

#include "stdafx.h"
#include "robot.h"

#include "robotDoc.h"
#include "serializers.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRobotDoc

//extern SLinkData LDs[3];
//extern SMotions MTs[2];

extern CRobotDoc * pRD;
extern CRobotView * pRV;
extern CMainFrame * pMF;

extern MWorkspace WS;

IMPLEMENT_DYNCREATE(CRobotDoc, CDocument)

BEGIN_MESSAGE_MAP(CRobotDoc, CDocument)
	//{{AFX_MSG_MAP(CRobotDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////
// Doc's resuorces


/////////////////////////////////////////////////////////////////////////////
// CRobotDoc construction/destruction

CRobotDoc::CRobotDoc()
{
	pRD=this;
}

CRobotDoc::~CRobotDoc()
{
}

void CRobotDoc::Init()
{
  //Header
  H.SelLink = -1;
	//Settings
	S.Scale=3;
	S.Mode=0;
	S.Center=true;
  S.aAxis=true;
  //Core
  core().Init();
}

BOOL CRobotDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	Init();
  core().Init();
  core().data().LDs[0].Icon = IDI_ML5;
  core().data().LDs[1].Icon = IDI_RL5;
  core().data().LDs[2].Icon = IDI_GRIPE1;

	//////////////////////////////////////////////////////////////////////////
  /*
  MSymb x = "SIN(x) * COS(x * y)";
  String s = x.det("x").GetAsStr();
  */
  //////////////////////////////////////////////////////////////////////////

  pMF->UpdateFrameByDoc();
	
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CRobotDoc serialization

void CRobotDoc::Serialize(CArchive& ar)
{
  if (ar.IsStoring())
	{
		ar.Write(&H,sizeof(H));
    ar.Write(&S,sizeof(S));
    serializers::store(ar, core());
	}
  else
	{
		SHeader lH;
		ar.Read(&lH,sizeof(lH));
/*
    if (lH.VToRead>1)
		{
			MessageBox(NULL,"Для открытия этого документа необходима более новая версия программы","Ошибка открытия",MB_ICONERROR|MB_OK);
			return;
		}
*/
		core().Release();
		H=lH;
		ar.Read(&S,sizeof(S));

    serializers::load(ar, core());

		pMF->UpdateFrameByDoc();
  }
}

/////////////////////////////////////////////////////////////////////////////
// CRobotDoc diagnostics

#ifdef _DEBUG
void CRobotDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRobotDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRobotDoc commands

bool CRobotDoc::selection_valid() const
{
  return H.SelLink > -1 && H.SelLink < int(core().link_count());
}

Link& CRobotDoc::selected_link() const
{
  _ASSERTE(selection_valid());
  return core().link(H.SelLink);
}

void CRobotDoc::select_link(int link)
{
  H.SelLink = link;
}

int CRobotDoc::sel_index() const
{
  return H.SelLink;
}

void CRobotDoc::on_modify(int power)
{
  if (power == modify::big)
  {
    core().OnBigModify();
    SetModifiedFlag();
  }
  pRV->on_update(power);
}