#pragma once


// smoothDlg dialog

class smoothDlg : public CDialog
{
	DECLARE_DYNAMIC(smoothDlg)

public:
	smoothDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~smoothDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SMOOTH_ };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_nSmooth1;
//	int m_nSmooth10;
	int m_nSmooth2;
	int m_nSmooth3;
	double m_nSmooth10;
	int m_nSmooth4;
	int m_nSmooth5;
	int m_nSmooth6;
	int m_nSmooth7;
	int m_nSmooth8;
	int m_nSmooth9;
};
