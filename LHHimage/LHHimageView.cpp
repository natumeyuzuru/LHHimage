
// LHHimageView.cpp : implementation of the CLHHimageView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "LHHimage.h"
#endif

#include "LHHimageDoc.h"
#include "LHHimageView.h"
#include"MainFrm.h"
#include "CHistogramDisplay.h"
#include"smoothDlg.h"
#include<time.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLHHimageView

IMPLEMENT_DYNCREATE(CLHHimageView, CScrollView)

BEGIN_MESSAGE_MAP(CLHHimageView, CScrollView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CLHHimageView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_VIEW_IMAGEBIGGER, &CLHHimageView::OnViewImagebigger)
	ON_COMMAND(ID_VIEW_IMAGESMALL, &CLHHimageView::OnViewImagesmall)
//	ON_WM_MOUSEMOVE()
ON_WM_MOUSEMOVE()
ON_COMMAND(ID_IMAGELINECHANGES_ONELINE, &CLHHimageView::ontran)
ON_COMMAND(ID_VIEW_CREATHISGRAPH, &CLHHimageView::OnViewCreathisgraph)
ON_COMMAND(ID_VIEW_IMAGESMOOTH, &CLHHimageView::OnViewImagesmooth)
ON_COMMAND(ID_VIEW_CREATTHISGRAPH2, &CLHHimageView::OnViewCreatthisgraph2)
END_MESSAGE_MAP()

// CLHHimageView construction/destruction

CLHHimageView::CLHHimageView() noexcept
{
	// TODO: add construction code here
	m_dZoom = 1.0;

}

CLHHimageView::~CLHHimageView()
{
}

BOOL CLHHimageView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

// CLHHimageView drawing

void CLHHimageView::OnDraw(CDC* pDC)
{
	CLHHimageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	CSize sizeTotal;
	sizeTotal.cx = int(m_dZoom * pDoc->imageWidth + 0.5);
	sizeTotal.cy = int(m_dZoom * pDoc->imageHeight + 0.5);
	SetScrollSizes(MM_TEXT, sizeTotal);

	if (pDoc->m_nOpenMode == 1)//BMP
	{
		StretchDIBits(pDC->m_hDC, 0, 0,
			int(m_dZoom * pDoc->imageWidth + 0.5), int(m_dZoom * pDoc->imageHeight + 0.5), 0, 0, pDoc->imageWidth, pDoc->imageHeight,
			pDoc->m_pBits, pDoc->lpbmi, DIB_RGB_COLORS, SRCCOPY);
	}


	if (pDoc->m_nOpenMode == 2)//JPG
	{
		StretchDIBits(pDC->m_hDC, 0, 0, int(m_dZoom * pDoc->image.cols + 0.5), int(m_dZoom * pDoc->image.rows + 0.5),
			0, pDoc->image.rows, pDoc->image.cols, -pDoc->image.rows,
			pDoc->image.data, pDoc->lpbmi,
			DIB_RGB_COLORS, SRCCOPY);
	}
}

void CLHHimageView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CLHHimageView printing


void CLHHimageView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CLHHimageView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLHHimageView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLHHimageView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CLHHimageView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CLHHimageView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CLHHimageView diagnostics

#ifdef _DEBUG
void CLHHimageView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CLHHimageView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CLHHimageDoc* CLHHimageView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLHHimageDoc)));
	return (CLHHimageDoc*)m_pDocument;
}
#endif //_DEBUG


// CLHHimageView message handlers


void CLHHimageView::OnViewImagebigger()
{
	// TODO: Add your command handler code here
	m_dZoom = m_dZoom * 2;
	Invalidate();

}


void CLHHimageView::OnViewImagesmall()
{
	// TODO: Add your command handler code here
	m_dZoom = m_dZoom / 2;
	Invalidate();
}


//void CLHHimageView::OnMouseMove(UINT nFlags, CPoint point)
//{
//	// TODO: Add your message handler code here and/or call default
//	/*CLHHimageDoc* pDoc = GetDocument();
//	if (pDoc->m_pBits != NULL)
//		if (!pDoc->image.data)
//		{
//			CPoint scrPoint = GetScrollPosition();
//			int nX = int(0.5 + (scrPoint.x + point.x) / m_dZoom);//计算到图像的坐标
//			int nY = int(0.5 + (scrPoint.y + point.y) / m_dZoom);
//			int nR, nG, nB;
//
//			if (nX >= 0 && nX < pDoc->imageWidth && nY >= 0 && nY < pDoc->imageHeight)
//			{
//				CString strPrompt;
//				if (pDoc->m_nOpenMode == 1)//bmp文件
//				{
//					if (pDoc->m_nColorBits == 8)//黑白
//					{
//						nB = *(pDoc->m_pBits + (pDoc->imageHeight - nY - 1) * pDoc->imageWidth + nX);
//						strPrompt.Format("X:%4d-Y:%4d Gray:%3d", nX, nY, nB);
//					}
//					if (pDoc->m_nColorBits == 24)//彩色
//					{
//						nB = *(pDoc->m_pBits + (pDoc->imageHeight - nY - 1) * pDoc->imageWidth * 3 + nX * 3 + 0);
//						nG = *(pDoc->m_pBits + (pDoc->imageHeight - nY - 1) * pDoc->imageWidth * 3 + nX * 3 + 1);
//						nR = *(pDoc->m_pBits + (pDoc->imageHeight - nY - 1) * pDoc->imageWidth * 3 + nX * 3 + 2);
//						strPrompt.Format("X:%4d-Y:%4d R:%3d G:%3d B:%3d", nX, nY, nR, nG, nB);
//					}
//				}
//				if (pDoc->m_nOpenMode == 2)//jpg
//				{
//					pDoc->m_pBits = (unsigned char*)pDoc->image.data;
//					if (pDoc->image.channels() == 1)//黑白图像
//					{
//						nB = *(pDoc->m_pBits + nY * pDoc->imageWidth + nX);
//						strPrompt.Format("X:%4d-Y:%4d Gray:%3d", nX, nY, nB);
//					}
//					if (pDoc->image.channels() == 3)//彩色
//					{
//						nB = *(pDoc->m_pBits + nY * pDoc->imageWidth * 3 + nX * 3 + 0);
//						nG = *(pDoc->m_pBits + nY * pDoc->imageWidth * 3 + nX * 3 + 1);
//						nR = *(pDoc->m_pBits + nY * pDoc->imageWidth * 3 + nX * 3 + 2);
//						strPrompt.Format("X:%4d-Y:%4d R:%3d G:%3d B:%3d", nX, nY, nR, nG, nB);
//					}
//				}
//				((CMainFrame*)AfxGetMainWnd())->SetWindowTextA(strPrompt);
//			}
//		}*/
//
//
//	CScrollView::OnMouseMove(nFlags, point);
//}


void CLHHimageView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CLHHimageDoc* pDoc = GetDocument();
	pDoc->image.cols;
		if (pDoc->m_pBits != NULL || pDoc->image.data )
			{
				CPoint scrPoint = GetScrollPosition();
				int nX = int(0.5 + (scrPoint.x + point.x) / m_dZoom);//计算到图像的坐标
				int nY = int(0.5 + (scrPoint.y + point.y) / m_dZoom);
				int nR, nG, nB;

				if (nX >= 0 && nX < pDoc->imageWidth && nY >= 0 && nY < pDoc->imageHeight)
				{
					CString strPrompt;
					if (pDoc->m_nOpenMode == 1)//bmp文件
					{
						if (pDoc->m_nColorBits == 8)//黑白
						{
							nB = *(pDoc->m_pBits + (pDoc->imageHeight - nY - 1) * pDoc->imageWidth + nX);
							strPrompt.Format("X:%4d-Y:%4d Gray:%3d", nX, nY, nB);
						}
						if (pDoc->m_nColorBits == 24)//彩色
						{
							nB = *(pDoc->m_pBits + (pDoc->imageHeight - nY - 1) * pDoc->imageWidth * 3 + nX * 3 + 0);
							nG = *(pDoc->m_pBits + (pDoc->imageHeight - nY - 1) * pDoc->imageWidth * 3 + nX * 3 + 1);
							nR = *(pDoc->m_pBits + (pDoc->imageHeight - nY - 1) * pDoc->imageWidth * 3 + nX * 3 + 2);
							strPrompt.Format("X:%4d-Y:%4d R:%3d G:%3d B:%3d", nX, nY, nR, nG, nB);
						}
					}
					if (pDoc->m_nOpenMode == 2)//jpg
					{
						pDoc->m_pBits = (unsigned char*)pDoc->image.data;
						if (pDoc->image.channels() == 1)//黑白图像
						{
							nB = *(pDoc->m_pBits + nY * pDoc->imageWidth + nX);
							strPrompt.Format("X:%4d-Y:%4d Gray:%3d", nX, nY, nB);
						}
						if (pDoc->image.channels() == 3)//彩色
						{
							nB = *(pDoc->m_pBits + nY * pDoc->imageWidth * 3 + nX * 3 + 0);
							nG = *(pDoc->m_pBits + nY * pDoc->imageWidth * 3 + nX * 3 + 1);
							nR = *(pDoc->m_pBits + nY * pDoc->imageWidth * 3 + nX * 3 + 2);
							strPrompt.Format("X:%4d-Y:%4d R:%3d G:%3d B:%3d", nX, nY, nR, nG, nB);
						}
					}
					((CMainFrame*)AfxGetMainWnd())->SetWindowTextA(strPrompt);
				}
			}

	CScrollView::OnMouseMove(nFlags, point);
}


void CLHHimageView::ontran()
{
	// TODO: Add your command handler code here
	CLHHimageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

 	pDoc->m_pBits = (unsigned  char*)pDoc->image.data;
	unsigned char* pBits = pDoc->m_pBits;
	int nWidth = pDoc->imageWidth;
	int nHeight = pDoc->imageHeight;
	int nValueR = 0;
	int nValueG = 0;
	int nValueB = 0;
	double dValueR = 0.0;
	double dValueB = 0.0;
	double dValueG = 0.0;
	long lTotal = 0;
	long lTotalD = 0;
	long lTotalL = 0;
	int ra = 150, rb = 255, ga = 100, gb = 200, ba = 25, bb = 125, a = 0, b = 255;
	if (pBits == NULL)return;
	int linebyte = pDoc->image.cols * pDoc->image.channels();
	for (int i = 0; i < nHeight; i++)
	{
		lTotalL = i * linebyte;
		for (int j = 0; j < nWidth; j++)
		{
			lTotalD = lTotalL + j*3;

			nValueR = pBits[lTotalD + 2];
			dValueR = ra + 1.0 * (rb - ra) / (b - a) * (nValueR - a);
			pBits[lTotalD + 2] = int(dValueR);

			nValueG = pBits[lTotalD + 1];
			dValueG = ga + 1.0 * (gb - ga) / (b - a) * (nValueG - a);
			pBits[lTotalD + 1] = int(dValueG);

			nValueB = pBits[lTotalD + 0];
			dValueB = ba + 1.0 * (bb - ba) / (b - a) * (nValueB - a);
			pBits[lTotalD + 0] = int(dValueB);

		}

	}
	Invalidate();
}


void CLHHimageView::OnViewCreathisgraph()
{
	// TODO: Add your command handler code hereCLHHimageDoc* pDoc = GetDocument();
	CLHHimageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	if (pDoc->m_nOpenMode == 2)//jpg
	{
		pDoc->imageWidth = pDoc->image.cols;
		pDoc->imageHeight = pDoc->image.rows;
		//pDoc->m_nColorBits 
		pDoc->m_pBits = pDoc->image.data;
	}
	unsigned char* pBits = pDoc->m_pBits;
	int nWidth = pDoc->imageWidth;
	int nHeight = pDoc->imageHeight;
	//直方图统计
	int nValue = 0;
	long lTotal = 0;
	long lTotalD = 0;
	long lTotalL = 0;

	dMAX = 0; dMAXB = 0; dMAXG = 0;
    long nValueG = 0;
	long nValueB = 0;
	if (pBits == NULL) return;
	
	int linebyte = (pDoc->image.cols * pDoc->image.channels());
	if (pDoc->m_nColorBits == 8)
	{
		for (int k = 0; k < 256; k++)
		{
			m_lValue[k] = 0;
			m_dValue[k] = 0;
		}
		for (int i = 0; i < nHeight; i++)
		{
			
			lTotalL =  i*linebyte;
			for (int j = 0; j < nWidth; j++)
			{
				lTotalD = lTotalL + j;
				nValue = *(pBits + lTotalD);
				m_lValue[nValue] = m_lValue[nValue] + 1;

				nValueG = *(pBits + lTotalD + 1);
				m_lValueG[nValueG] = m_dValueG[nValueG] + 1;

				nValue = *(pBits + lTotalD + 2);
				m_lValue[nValue] = m_dValue[nValue] + 1;

			}
		}
		lTotal = nWidth * nHeight;
		for (int k = 0; k < 256; k++)
		{
			m_dValue[k] = 1.0 * m_lValue[k] / lTotal;
			if (dMAX < m_dValue[k]) dMAX = m_dValue[k];

			m_dValueG[k] = 1.0 * m_lValueG[k] / lTotal;
			if (dMAXG < m_dValueG[k])dMAXG = m_dValueG[k];

			m_dValueB[k] = 1.0 * m_lValueB[k] / lTotal;
			if (dMAXB < m_dValueB[k])dMAXB = m_dValueB[k];
		}
	}
	

	if (pDoc->m_nColorBits == 24)
	{
		for (int k = 0; k < 256; k++)
		{
			m_lValue[k] = 0;
			m_dValue[k] = 0.0;
			m_lValueG[k] = 0;
			m_dValueG[k] = 0.0;
			m_lValueB[k] = 0;
			m_dValueB[k] = 0.0;
		}
		for (int i = 0; i < nHeight; i++)
		{
			lTotalL = linebyte * i;
			for (int j = 0; j < nWidth; j++)
			{
				lTotalD = lTotalL + j * 3;

				nValueB = *(pBits + lTotalD);
				m_lValueB[nValueB] = m_dValueB[nValueB] + 1;

				nValueG = *(pBits + lTotalD+1);
				m_lValueG[nValueG] = m_dValueG[nValueG] + 1;

				nValue = *(pBits + lTotalD+2);
				m_lValue[nValue] = m_dValue[nValue] + 1;
			}

		}
		lTotal = nWidth * nHeight;
		for (int k = 0; k < 256; k++)
		{
			m_dValue[k] = 1.0 * m_lValue[k] / lTotal;
			if (dMAX < m_dValue[k])dMAX = m_dValue[k];

			m_dValueG[k] = 1.0 * m_lValueG[k] / lTotal;
			if (dMAXG < m_dValueG[k])dMAXG = m_dValueG[k];

			m_dValueB[k] = 1.0 * m_lValueB[k] / lTotal;
			if (dMAXB < m_dValueB[k])dMAXB = m_dValueB[k];

		}
	}
	Invalidate();
	CHistogramDisplay hisDlg;
	hisDlg.m_nColorbits = pDoc->m_nColorBits;
	hisDlg.dMax = dMAX;
	hisDlg.dMaxG = dMAXG;
	hisDlg.dMaxB = dMAXB;
	memcpy(hisDlg.m_dValue, m_dValue, 256*sizeof(double));
	memcpy(hisDlg.m_dValueG, m_dValueG, 256 * sizeof(double));
	memcpy(hisDlg.m_dValueB, m_dValueB, 256 * sizeof(double));
	hisDlg.DoModal();
}


void CLHHimageView::OnViewImagesmooth()
{
	// TODO: Add your command handler code here
	smoothDlg sdlg;
	if (sdlg.DoModal())
	{
		double H[3][3];//申明模板变量
		double K = sdlg.m_nSmooth10;//H 与 K组合为8-领域平均法
		H[0][0] = sdlg.m_nSmooth1;// 申明模板变量赋值
		H[0][1] = sdlg.m_nSmooth2;
		H[0][2] = sdlg.m_nSmooth3;
		H[1][0] = sdlg.m_nSmooth4;
		H[1][1] = sdlg.m_nSmooth5;
		H[1][2] = sdlg.m_nSmooth6;
		H[2][0] = sdlg.m_nSmooth7;
		H[2][1] = sdlg.m_nSmooth8;
		H[2][2] = sdlg.m_nSmooth9;
		/*CLHHimageDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		unsigned char* pBits = pDoc->m_pBits;
		if (!pBits)
			return;
		int nWidth = pDoc->imageWidth;
		int nHeight = pDoc->imageHeight;
		long lTotalR, lTotal;
		int m, n;
		double dValue;
		double Value[3][3];
		unsigned char* pOldBits = new unsigned char[nWidth * nHeight];//分配内存
		CopyMemory(pOldBits, pBits, nWidth * nHeight);//把图像复制到pOldBit
		for (int i = 0; i < nHeight; i++)
		{
			lTotalR = i * nWidth;
			for (int j = 0; j < nWidth; j++)
			{
				lTotal = lTotalR + j;
				if (i == 0 || i == nHeight - 1 || j == 0 || j == nWidth - 1)//边缘点不作处理，直接
					
				{
				pBits[lTotal] = pOldBits[lTotal];
				}
				else//内部点作平滑处理，
				{
					Value[0][0] = pOldBits[lTotal - nWidth - 1];
					Value[0][1] = pOldBits[lTotal - nWidth - 0];
					Value[0][2] = pOldBits[lTotal - nWidth + 1];
					Value[1][0] = pOldBits[lTotal - 0 - 1];
					Value[1][1] = pOldBits[lTotal - 0 - 0];
					Value[1][2] = pOldBits[lTotal - 0 + 1];
					Value[2][0] = pOldBits[lTotal + nWidth - 1];
					Value[2][1] = pOldBits[lTotal + nWidth - 0];
					Value[2][2] = pOldBits[lTotal + nWidth + 1];
					dValue = 0.0;
					for (m = 0; m < 3; m++)
						for (n = 0; n < 3; n++)
							dValue = dValue + H[m][n] * Value[m][n];
					dValue = dValue * K;
					pBits[lTotal] = int(dValue + 0.5);
				}
			}
		}
		Invalidate();
		delete pOldBits;//删除零时分配内存*/

		//jpg
		CLHHimageDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		unsigned char* pBits = pDoc->m_pBits;
		if (!pBits)
			return;
		int nWidth = pDoc->imageWidth;
		int nHeight = pDoc->imageHeight;
		long lTotalR, lTotal;
		int m, n;
		double dValue;
		double Value[3][3];
		unsigned char* pOldBits = NULL;//分配内存
		if (pDoc->m_nColorBits == 8)
		{
			pOldBits = new unsigned char[nWidth * nHeight];//分配内存
			CopyMemory(pOldBits, pBits, nWidth * nHeight);//把图像复制到pOldBit
			for (int i = 0; i < nHeight; i++)
			{
				lTotalR = i * nWidth;
				for (int j = 0; j < nWidth; j++)
				{
					lTotal = lTotalR + j;
					if (i == 0 || i == nHeight - 1 || j == 0 || j == nWidth - 1)//边缘点不作处
						
					{
					pBits[lTotal] = pOldBits[lTotal];
					}
					else//内部点作平滑处理，
					{
						Value[0][0] = pOldBits[lTotal - nWidth - 1];
						Value[0][1] = pOldBits[lTotal - nWidth - 0];
						Value[0][2] = pOldBits[lTotal - nWidth + 1];
						Value[1][0] = pOldBits[lTotal - 0 - 1];
						Value[1][1] = pOldBits[lTotal - 0 - 0];
						Value[1][2] = pOldBits[lTotal - 0 + 1];
						Value[2][0] = pOldBits[lTotal + nWidth - 1];
						Value[2][1] = pOldBits[lTotal + nWidth - 0];
						Value[2][2] = pOldBits[lTotal + nWidth + 1];
						dValue = 0.0;
						for (m = 0; m < 3; m++)
							for (n = 0; n < 3; n++)
								dValue = dValue + H[m][n] * Value[m][n];
						dValue = dValue * K;
						pBits[lTotal] = int(dValue + 0.5);
					}
				}
			}
			Invalidate();
			delete pOldBits;//删除零时分配内存
		}
		else if (pDoc->m_nColorBits == 24)
		{
			pOldBits = new unsigned char[3 * nWidth * nHeight];//分配内存
			CopyMemory(pOldBits, pBits, 3 * nWidth * nHeight);//把图像复制到pOldBit
			for (int i = 0; i < nHeight; i++)
			{
				lTotalR = i * nWidth;
				for (int j = 0; j < nWidth; j++)
				{
					lTotal = (lTotalR + j) * 3;
					if (i == 0 || i == nHeight - 1 || j == 0 || j == nWidth - 1)//边缘点不作处
						
					{
					pBits[lTotal + 0] = pOldBits[lTotal + 0];
					pBits[lTotal + 1] = pOldBits[lTotal + 1];
					pBits[lTotal + 2] = pOldBits[lTotal + 2];
					}
					else//内部点作平滑处理，
					{
						Value[0][0] = pOldBits[lTotal - 3 * nWidth - 3 * 1 + 0];
						Value[0][1] = pOldBits[lTotal - 3 * nWidth - 3 * 0 + 0];
						Value[0][2] = pOldBits[lTotal - 3 * nWidth + 3 * 1 + 0];
						Value[1][0] = pOldBits[lTotal - 3 * 0 - 3 * 1 + 0];
						Value[1][1] = pOldBits[lTotal - 3 * 0 - 3 * 0 + 0];
						Value[1][2] = pOldBits[lTotal - 3 * 0 + 3 * 1 + 0];
						Value[2][0] = pOldBits[lTotal + 3 * nWidth - 3 * 1 + 0];
						Value[2][1] = pOldBits[lTotal + 3 * nWidth - 3 * 0 + 0];
						Value[2][2] = pOldBits[lTotal + 3 * nWidth + 3 * 1 + 0];
						dValue = 0.0;
						for (m = 0; m < 3; m++)
							for (n = 0; n < 3; n++)
								dValue = dValue + H[m][n] * Value[m][n];
						dValue = dValue * K;
						pBits[lTotal + 0] = int(dValue + 0.5);
						Value[0][0] = pOldBits[lTotal - 3 * nWidth - 3 * 1 + 1];
						Value[0][1] = pOldBits[lTotal - 3 * nWidth - 3 * 0 + 1];
						Value[0][2] = pOldBits[lTotal - 3 * nWidth + 3 * 1 + 1];
						Value[1][0] = pOldBits[lTotal - 3 * 0 - 3 * 1 + 1];
						Value[1][1] = pOldBits[lTotal - 3 * 0 - 3 * 0 + 1];
						Value[1][2] = pOldBits[lTotal - 3 * 0 + 3 * 1 + 1];
						Value[2][0] = pOldBits[lTotal + 3 * nWidth - 3 * 1 + 1];
						Value[2][1] = pOldBits[lTotal + 3 * nWidth - 3 * 0 + 1];
						Value[2][2] = pOldBits[lTotal + 3 * nWidth + 3 * 1 + 1];
						dValue = 0.0;
						for (m = 0; m < 3; m++)
							for (n = 0; n < 3; n++)
								dValue = dValue + H[m][n] * Value[m][n];
						dValue = dValue * K;
						pBits[lTotal + 1] = int(dValue + 0.5);
						Value[0][0] = pOldBits[lTotal - 3 * nWidth - 3 * 1 + 2];
						Value[0][1] = pOldBits[lTotal - 3 * nWidth - 3 * 0 + 2];
						Value[0][2] = pOldBits[lTotal - 3 * nWidth + 3 * 1 + 2];
						Value[1][0] = pOldBits[lTotal - 3 * 0 - 3 * 1 + 2];
						Value[1][1] = pOldBits[lTotal - 3 * 0 - 3 * 0 + 2];
						Value[1][2] = pOldBits[lTotal - 3 * 0 + 3 * 1 + 2];
						Value[2][0] = pOldBits[lTotal + 3 * nWidth - 3 * 1 + 2];
						Value[2][1] = pOldBits[lTotal + 3 * nWidth - 3 * 0 + 2];
						Value[2][2] = pOldBits[lTotal + 3 * nWidth + 3 * 1 + 2];
						dValue = 0.0;
						for (m = 0; m < 3; m++)
							for (n = 0; n < 3; n++)
								dValue = dValue + H[m][n] * Value[m][n];
						dValue = dValue * K;
						pBits[lTotal + 2] = int(dValue + 0.5);
					}
				}
			}
			Invalidate();
			delete pOldBits;//删除零时分配内存
		}
	}
}
	



void CLHHimageView::OnViewCreatthisgraph2()
{
	// TODO: Add your command handler code here
	CLHHimageDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	
	system("color 2F");

	
	imshow("素材图", pDoc->image);
	if (!pDoc->image.data) {
		printf("图像读取失败!");
		
	}

	int bins = 256;
	int hist_size[] = { bins };
	float range[] = { 0,256 };
	const float* ranges[] = { range };
	MatND redHist, greenHist, blueHist;
	int channels_r[] = { 0 };

	//进行直方图的计算（红色分量部分）
	calcHist(&pDoc->image, 1, channels_r, Mat(),	//不使用掩膜
		redHist, 1, hist_size, ranges, true, false);

	//进行直方图计算（绿色分量部分）
	int channels_g[] = { 1 };
	calcHist(&pDoc->image, 1, channels_g, Mat(),
		greenHist, 1, hist_size, ranges, true, false);

	//进行直方图计算（蓝色分量部分）
	int channels_b[] = { 2 };
	calcHist(&pDoc->image, 1, channels_b, Mat(),
		blueHist, 1, hist_size, ranges, true, false);

	//准备参数绘制三色直方图
	double maxValue_red, maxValue_green, maxValue_blue;
	minMaxLoc(redHist, 0, &maxValue_red, 0, 0);
	minMaxLoc(greenHist, 0, &maxValue_green, 0, 0);
	minMaxLoc(blueHist, 0, &maxValue_blue, 0, 0);
	int scale = 1;
	int histHeight = 256;
	//bins * 3 是因为要绘制三个通道,每个通道的像素取值在 0-bins
	Mat histImage = Mat::zeros(histHeight, bins * 3, CV_8UC3);

	//开始绘制
	for (int i = 0; i < bins; i++) {
		float binValue_red = redHist.at<float>(i);
		float binValue_green = greenHist.at<float>(i);
		float binValue_blue = blueHist.at<float>(i);

		//计算高度时的乘除与下面绘图的 histHeight - intensity 是为了便于显示，否则有的色度很低
		//要绘制的高度
		int intensity_red = cvRound(binValue_red * histHeight / maxValue_red);
		int intensity_green = cvRound(binValue_green * histHeight / maxValue_green);
		int intensity_blue = cvRound(binValue_blue * histHeight / maxValue_blue);
		//绘制红色分量直方图
		rectangle(histImage, Point(i * scale, histHeight - 1),
			Point((i + 1) * scale - 1, histHeight - intensity_red),
			Scalar(255, 0, 0));

		//绘制绿色分量直方图
		rectangle(histImage, Point((i + bins) * scale, histHeight - 1),
			Point((i + bins + 1) * scale - 1, histHeight - intensity_green),
			Scalar(0, 255, 0));

		//绘制分量直方图
		rectangle(histImage, Point((i + bins * 2) * scale, histHeight - 1),
			Point((i + bins * 2 + 1) * scale - 1, histHeight - intensity_blue),
			Scalar(0, 0, 255));
	}

	imshow("图像的 RGB 直方图", histImage);

	waitKey(0);
	
	
}
