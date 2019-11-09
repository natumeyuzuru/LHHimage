
// LHHimageView.h : interface of the CLHHimageView class
//

#pragma once



class CLHHimageView : public CScrollView
{
protected: // create from serialization only
	CLHHimageView() noexcept;
	DECLARE_DYNCREATE(CLHHimageView)

// Attributes
public:
	CLHHimageDoc* GetDocument() const;

// Operations
public:
	double m_dZoom;

	long m_lValue[256];
	double m_dValue[256];

	long m_lValueG[256];
	double m_dValueG[256];

	long m_lValueB[256];
	double m_dValueB[256];

	double dMAX, dMAXG, dMAXB;

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CLHHimageView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnViewImagebigger();
	afx_msg void OnViewImagesmall();
//	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void ontran();
	afx_msg void OnViewCreathisgraph();
	afx_msg void OnViewImagesmooth();
	afx_msg void OnViewCreatthisgraph2();
};

#ifndef _DEBUG  // debug version in LHHimageView.cpp
inline CLHHimageDoc* CLHHimageView::GetDocument() const
   { return reinterpret_cast<CLHHimageDoc*>(m_pDocument); }
#endif

