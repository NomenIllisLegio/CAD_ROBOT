#pragma once


// CEditPosDlg dialog

class CEditPosDlg : public CDialog
{
	DECLARE_DYNAMIC(CEditPosDlg)

public:
	CEditPosDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditPosDlg();

// Dialog Data
	enum { IDD = IDD_EDIT_POS_DLG };

  float pos, vel, acc;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
  float m_pos;
  float m_vel;
  float m_acc;
public:
  virtual BOOL OnInitDialog();
protected:
  virtual void OnOK();
};
