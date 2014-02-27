#pragma once

struct TrajPoint;

// CEditTrajDlg dialog

class CEditTrajDlg : public CDialog
{
	DECLARE_DYNAMIC(CEditTrajDlg)

public:
	CEditTrajDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditTrajDlg();

// Dialog Data
	enum { IDD = IDD_EDIT_TRAJ_DLG };

  TrajPoint* traj_point;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
  float m_x;
  float m_y;
  float m_z;
  float m_time;
public:
  virtual BOOL OnInitDialog();
protected:
  virtual void OnOK();
};
