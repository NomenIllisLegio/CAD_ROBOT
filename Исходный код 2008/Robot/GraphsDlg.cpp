// GraphsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "robot.h"
#include "GraphsDlg.h"
#include ".\graphsdlg.h"


// CGraphsDlg dialog

IMPLEMENT_DYNAMIC(CGraphsDlg, CDialog)
CGraphsDlg::CGraphsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGraphsDlg::IDD, pParent)
  , func_2(NULL)
{
}

CGraphsDlg::~CGraphsDlg()
{
}

void CGraphsDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_GRAPH, m_graph);
  DDX_Control(pDX, IDC_CAPT, m_caption);
}


BEGIN_MESSAGE_MAP(CGraphsDlg, CDialog)
  ON_WM_PAINT()
END_MESSAGE_MAP()


// CGraphsDlg message handlers

BOOL CGraphsDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_caption.SetWindowText(caption);

  return TRUE;
}

void CGraphsDlg::draw_graph()
{
  CDC* dc = m_graph.GetDC();
  RECT rect;
  m_graph.GetWindowRect(&rect);
  int size_x = rect.right - rect.left - 4;
  int size_y = rect.bottom - rect.top - 4;
  
  //Y bounds
  float height = func->max_y() - func->min_y() + 1.0f;
  float min_y = func->min_y() - height * 0.1f;
  float max_y = func->max_y() + height * 0.1f;
  height = max_y - min_y;

  //X bounds
  float width = func->max_x() - func->min_x();
  float min_x = func->min_x() - width * 0.1f;
  float max_x = func->max_x() + width * 0.1f;
  width = max_x - min_x;

  dc->Rectangle(0, 0, size_x, size_y); 
  bool in_draw = false;

  //Line
  for (float x = func->min_x(); x < func->max_x(); x += width / size_x)
  {
    float y = (*func)(x);

    int i_x = int((x - min_x) / width * size_x);
    int i_y = int(size_y - (y - min_y) / height * size_y);

    if (in_draw)
      dc->LineTo(i_x, i_y);
    else
    {
      dc->MoveTo(i_x, i_y);      
      in_draw = true;
    }
  }

  //Alternative line
  if (func_2)
  {
    for (float x = func->min_x(); x < func->max_x(); x += width / size_x * 3)
    {
      float y = (*func_2)(x);

      int i_x = int((x - min_x) / width * size_x);
      int i_y = int(size_y - (y - min_y) / height * size_y);

      dc->SetPixel(i_x, i_y, RGB(255, 0, 0));
    }
  }

  //Points
  for (Size p = 0; p < func->size(); ++ p)
  {
    const Point& pt = func->point(p);

    int i_x = int((pt.x - min_x) / width * size_x);
    int i_y = int(size_y - (pt.y - min_y) / height * size_y);

    dc->Ellipse(i_x - 4, i_y - 4, i_x + 4, i_y + 4);
    
    CString str;
    str.Format("%g%s, %g%s", mthRound(pt.x), x_units, mthRound(pt.y), y_units);
    
    dc->TextOut(i_x - 30, i_y + 5, str);
  }
}
void CGraphsDlg::OnBnClickedGo()
{
  draw_graph();
}

void CGraphsDlg::OnPaint()
{
  CPaintDC dc(this);
  draw_graph();
}
