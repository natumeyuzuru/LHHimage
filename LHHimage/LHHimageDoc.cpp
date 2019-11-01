
// LHHimageDoc.cpp : implementation of the CLHHimageDoc class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "LHHimage.h"
#endif

#include "LHHimageDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CLHHimageDoc

IMPLEMENT_DYNCREATE(CLHHimageDoc, CDocument)

BEGIN_MESSAGE_MAP(CLHHimageDoc, CDocument)
END_MESSAGE_MAP()


// CLHHimageDoc construction/destruction

CLHHimageDoc::CLHHimageDoc() noexcept
{
	// TODO: add one-time construction code here
	m_pBits = NULL;
	lpbmi = NULL;
	imageWidth = 768;

	imageHeight = 576;
	m_nOpenMode = 0;
	m_nColorBits = 8;
	image = 0;
}

CLHHimageDoc::~CLHHimageDoc()
{
}

BOOL CLHHimageDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CLHHimageDoc serialization

void CLHHimageDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CLHHimageDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CLHHimageDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CLHHimageDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CLHHimageDoc diagnostics

#ifdef _DEBUG
void CLHHimageDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CLHHimageDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CLHHimageDoc commands

BOOL CLHHimageDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  在此添加您专用的创建代码
	CString str = lpszPathName;
	str.MakeLower();
	if (str.Find(_TEXT(".bmp")) != -1)
	{
		m_nOpenMode = 1;
		if (!ReadBMP(lpszPathName)) return FALSE;
	}

	if (str.Find(_TEXT(".jpg")) != -1)
	{
		m_nOpenMode = 2;
		image = imread(lpszPathName);
		
		int ndepth = image.depth();
		int nchan = image.channels();
		lpbmi = (LPBITMAPINFO)new char[sizeof(BITMAPINFO) + 4 * (1 << 8)];
		
		if (ndepth == CV_8U) ndepth = 8;//cv-8u数值为 0，是位深度标识
		lpbmi->bmiHeader.biBitCount = ndepth * nchan;
		lpbmi->bmiHeader.biClrUsed = 0;
		lpbmi->bmiHeader.biHeight = image.rows;
		lpbmi->bmiHeader.biWidth = image.cols;
		lpbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		lpbmi->bmiHeader.biSizeImage = image.cols * image.rows * image.channels();
		lpbmi->bmiHeader.biCompression = 0;
		lpbmi->bmiHeader.biPlanes = 1;
		imageWidth = lpbmi->bmiHeader.biWidth;
		imageHeight = lpbmi->bmiHeader.biHeight;

		for (int i = 0; i < (1 << 8); i++)//不知道但是干啥的循环
		{
			RGBQUAD & o = lpbmi->bmiColors[i];
			o.rgbBlue = i;
			o.rgbGreen = i;
			o.rgbRed = i;
		}
		imshow("the jpg", image);//两个读出来的图像不一样。用上面的读出来是扭的//
		
		

	}




	return TRUE;
}
void CLHHimageDoc::ReadRAW(LPCTSTR lpszPathName, int nWidth, int nHeight)
{
}
//读取图像
BOOL CLHHimageDoc::ReadBMP(LPCTSTR lpszPathName)
{
	long lTotal = 0;
	CFile file;
	file.Open(lpszPathName, CFile::modeRead);
	file.Read(&bmpFH, sizeof(BITMAPFILEHEADER));
	lpbmi = (LPBITMAPINFO)new char[sizeof(BITMAPINFO) + 4 * (1 << 8)];
	// lpbmi->bmiHeader.biBitCount=8;
	// lpbmi->bmiHeader.biClrUsed=0;
	// lpbmi->bmiHeader.biHeight=imageHeight;
	// lpbmi->bmiHeader.biWidth=imageWidth;
	// lpbmi->bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	// lpbmi->bmiHeader.biSizeImage=imageWidth*imageHeight;
	// lpbmi->bmiHeader.biClrImportant=0;
	// lpbmi->bmiHeader.biCompression=0;
	// lpbmi->bmiHeader.biPlanes=1;
	// lpbmi->bmiColors[i]
	file.Read(lpbmi, sizeof(BITMAPINFOHEADER));
	m_nColorBits = lpbmi->bmiHeader.biBitCount;
	imageHeight = lpbmi->bmiHeader.biHeight;
	imageWidth = lpbmi->bmiHeader.biWidth;
	if (m_nColorBits == 8)
	{
		lTotal = imageWidth * imageHeight;
		file.Read(&(lpbmi->bmiColors[0]), 256 * 4);
	}
	else if (m_nColorBits == 24)
	{
		lTotal = imageWidth * imageHeight * 3;
	}
	else
	{
		file.Close();
		return FALSE;
	}
	m_pBits = new unsigned char[lTotal];
	file.Read(m_pBits, lTotal);
	file.Close();
	return TRUE;
}

BOOL CLHHimageDoc::SaveBMP(LPCTSTR lpszPathName)
{
	long lTotal = 0;
	CFile file;
	file.Open(lpszPathName, CFile::modeCreate || CFile::modeReadWrite);
	file.Write(&bmpFH, sizeof(BITMAPFILEHEADER));
	file.Write(lpbmi, sizeof(BITMAPINFOHEADER));
	if (m_nColorBits == 8)//黑白
	{
		lTotal = imageWidth * imageHeight;//the daxiao
		file.Write(&(lpbmi->bmiColors[0]), 256 * 4);
	}
	else if (m_nColorBits == 24)//colorful
	{
		lTotal = imageWidth * imageHeight * 3;
	}
	else {
		file.Close();
		return FALSE;
	}
	file.Write(m_pBits, lTotal);
	file.Close();
	return TRUE;

}

BOOL CLHHimageDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_nOpenMode == 1 && m_pBits != NULL) SaveBMP(lpszPathName);
	if (m_nOpenMode == 2 && !image.data) imwrite(lpszPathName, image);
	return TRUE;
	return CDocument::OnSaveDocument(lpszPathName);
}
