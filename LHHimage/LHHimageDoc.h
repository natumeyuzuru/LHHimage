
// LHHimageDoc.h : interface of the CLHHimageDoc class
//


#pragma once
#include<opencv.hpp>
#include<opencv2/highgui/highgui_c.h>
#include<opencv2/imgproc.hpp>
#include<opencv2/core.hpp>
#include<opencv2/cvconfig.h>
#include<opencv2/core/core_c.h>
#include<opencv/highgui.h>
#include<opencv.hpp>
#include<opencv/cxcore.h>
#include<opencv/cv.h>
#include<opencv/highgui.h>
using namespace cv;

class CLHHimageDoc : public CDocument
{
protected: // create from serialization only
	CLHHimageDoc() noexcept;
	DECLARE_DYNCREATE(CLHHimageDoc)

// Attributes
public:
	BITMAPINFO* lpbmi;
	unsigned char* m_pBits;
	BITMAPFILEHEADER bmpFH;
	int imageWidth;
	int imageHeight;
	int m_nColorBits;//8 24
	int m_nOpenMode;//=0 RAW =1 BMP =2 OpenCV

// Operations
public:
	Mat image;//OpenCVÕºœÒ÷∏’Î
	void ReadRAW(LPCTSTR lpszPathName, int nWidth, int nHeight);
	BOOL ReadBMP(LPCTSTR lpszPathName);
	BOOL SaveBMP(LPCTSTR lpszPathName);
// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CLHHimageDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
};





