#pragma once


// CHistogramDisplay dialog

class CHistogramDisplay : public CDialogEx
{
	DECLARE_DYNAMIC(CHistogramDisplay)

public:
	CHistogramDisplay(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CHistogramDisplay();

	double m_dValue[256]; //�����ֱ��ͼ
	double m_dValueG[256];//�̷���ֱ��ͼ
	double m_dValueB[256]; //������ֱ��ͼ
	double dMax, dMaxG, dMaxB;//�졢�̡���ֱ��ͼ�����ֵ����ͼʱ���ƻ�ͼ��Χ
	int m_nColorbits;//���ֺڰ׺����ɫͼ��

	void DrawGraph(CDC* pDC);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_histogram };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnBnClickedOk();
};