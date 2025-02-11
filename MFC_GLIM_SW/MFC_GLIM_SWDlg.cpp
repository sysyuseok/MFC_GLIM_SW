// MFC_GLIM_SWDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MFC_GLIM_SW.h"
#include "MFC_GLIM_SWDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <cmath>
#include <ctime>
#include "resource.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

#define IMAGE_WIDTH 700
#define IMAGE_HEIGHT 400
#define IMAGE_BPP 8

#define WM_UPDATE_RANDOM_RING (WM_USER + 101)


class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_ABOUTBOX };
#endif
protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}
BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

CMFCGLIMSWDlg::CMFCGLIMSWDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_MFC_GLIM_SW_DIALOG, pParent)
    , OuterCircleThickness(8)
    , ClickPointCircleRadius(15)
    , numOfCircle(0)
    , m_bDragging(false)
    , m_nDraggingIndex(-1)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCGLIMSWDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT2, OuterCircleThickness);
    DDV_MinMaxFloat(pDX, OuterCircleThickness, 0.0, 100.0);
    DDX_Text(pDX, IDC_EDIT1, ClickPointCircleRadius);
    DDV_MinMaxFloat(pDX, ClickPointCircleRadius, 0.0, 100.0);
}

BEGIN_MESSAGE_MAP(CMFCGLIMSWDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON1, &CMFCGLIMSWDlg::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON2, &CMFCGLIMSWDlg::OnBnClickedButton2)
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
    ON_BN_CLICKED(IDC_BUTTON_RESET, &CMFCGLIMSWDlg::OnBnClickedButtonReset)
    ON_BN_CLICKED(IDC_BUTTON_RANDOM, &CMFCGLIMSWDlg::OnBnClickedButtonRandom)
    ON_MESSAGE(WM_UPDATE_RANDOM_RING, &CMFCGLIMSWDlg::OnUpdateRandomRing)
END_MESSAGE_MAP()

BOOL CMFCGLIMSWDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    ModifyStyle(0, WS_CLIPCHILDREN);

    srand((unsigned)time(NULL));

    m_image.Create(IMAGE_WIDTH, -IMAGE_HEIGHT, IMAGE_BPP);
    if (IMAGE_BPP == 8)
    {
        static RGBQUAD rgb[256];
        for (int i = 0; i < 256; i++)
            rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
        m_image.SetColorTable(0, 256, rgb);
    }
    {
        int nPitch = m_image.GetPitch();
        unsigned char* fm = (unsigned char*)m_image.GetBits();
        memset(fm, 0xff, nPitch * IMAGE_HEIGHT);
    }
    UpdateDisplay();
    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);
    return TRUE;
}

void CMFCGLIMSWDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialogEx::OnSysCommand(nID, lParam);
    }
}


void CMFCGLIMSWDlg::OnPaint()
{
    PAINTSTRUCT ps;
    HWND hwnd = GetSafeHwnd();
    HDC hdc = ::BeginPaint(hwnd, &ps);
    m_image.Draw(hdc, 0, 50);
    ::EndPaint(hwnd, &ps);
}

HCURSOR CMFCGLIMSWDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CMFCGLIMSWDlg::OnBnClickedButton1()
{
    CString editValue;
    GetDlgItemText(IDC_EDIT1, editValue);
    ClickPointCircleRadius = _ttof(editValue);
}

void CMFCGLIMSWDlg::OnBnClickedButton2()
{
    CString editValue;
    GetDlgItemText(IDC_EDIT2, editValue);
    OuterCircleThickness = _ttof(editValue);
}


bool CMFCGLIMSWDlg::isInCircle(int i, int j, int nCenterX, int nCenterY, int nRadius)
{
    double dX = j - nCenterX;
    double dY = i - nCenterY;
    return (dX * dX + dY * dY < nRadius * nRadius);
}

void CMFCGLIMSWDlg::drawCircle(unsigned char* fm, int x, int y, int nRadius, int nGray)
{
    int nCenterX = x + nRadius;
    int nCenterY = y + nRadius;
    int nPitch = m_image.GetPitch();
    for (int i = y; i < y + nRadius * 2; i++)
    {
        for (int j = x; j < x + nRadius * 2; j++)
        {
            if (isInCircle(i, j, nCenterX, nCenterY, nRadius))
                fm[i * nPitch + j] = nGray;
        }
    }
}

void CMFCGLIMSWDlg::drawRing(unsigned char* fm, double centerX, double centerY, double R, double thickness, int nGray)
{
    double inner = R - thickness / 2.0;
    double outer = R + thickness / 2.0;
    double inner2 = inner * inner;
    double outer2 = outer * outer;
    int nPitch = m_image.GetPitch();
    int ix = max(0, (int)floor(centerX - outer));
    int iy = max(0, (int)floor(centerY - outer));
    int ex = min(IMAGE_WIDTH - 1, (int)ceil(centerX + outer));
    int ey = min(IMAGE_HEIGHT - 1, (int)ceil(centerY + outer));
    for (int i = iy; i <= ey; i++)
    {
        for (int j = ix; j <= ex; j++)
        {
            double dx = j - centerX;
            double dy = i - centerY;
            double dist2 = dx * dx + dy * dy;
            if (dist2 >= inner2 && dist2 <= outer2)
                fm[i * nPitch + j] = nGray;
        }
    }
}

void CMFCGLIMSWDlg::RedrawAll()
{
    int nPitch = m_image.GetPitch();
    unsigned char* fm = (unsigned char*)m_image.GetBits();
    memset(fm, 0xff, nPitch * IMAGE_HEIGHT);
    int r = static_cast<int>(ClickPointCircleRadius);
    if (numOfCircle >= 1)
        drawCircle(fm, m_circleCenters[0].x - r, m_circleCenters[0].y - r, r, 60);
    if (numOfCircle >= 2)
        drawCircle(fm, m_circleCenters[1].x - r, m_circleCenters[1].y - r, r, 120);
    if (numOfCircle >= 3)
        drawCircle(fm, m_circleCenters[2].x - r, m_circleCenters[2].y - r, r, 180);
    if (numOfCircle == 3)
    {
        CPoint A = m_circleCenters[0];
        CPoint B = m_circleCenters[1];
        CPoint C = m_circleCenters[2];
        double Ax = A.x, Ay = A.y;
        double Bx = B.x, By = B.y;
        double Cx = C.x, Cy = C.y;
        double D = 2 * (Ax * (By - Cy) + Bx * (Cy - Ay) + Cx * (Ay - By));
        if (fabs(D) >= 1e-6)
        {
            double center_x = ((Ax * Ax + Ay * Ay) * (By - Cy) +
                (Bx * Bx + By * By) * (Cy - Ay) +
                (Cx * Cx + Cy * Cy) * (Ay - By)) / D;
            double center_y = ((Ax * Ax + Ay * Ay) * (Cx - Bx) +
                (Bx * Bx + By * By) * (Ax - Cx) +
                (Cx * Cx + Cy * Cy) * (Bx - Ax)) / D;
            double R = sqrt((Ax - center_x) * (Ax - center_x) +
                (Ay - center_y) * (Ay - center_y));
            drawRing(fm, center_x, center_y, R, OuterCircleThickness, 50);
        }
    }
    UpdateDisplay();
}

void CMFCGLIMSWDlg::UpdateDisplay()
{
    HWND hWnd = GetSafeHwnd();
    HDC hdc = ::GetDC(hWnd);
    m_image.Draw(hdc, 0, 50);
    ::ReleaseDC(hWnd, hdc);
}

void CMFCGLIMSWDlg::OnBnClickedButtonReset()
{
    int nPitch = m_image.GetPitch();
    unsigned char* fm = (unsigned char*)m_image.GetBits();
    memset(fm, 0xff, nPitch * IMAGE_HEIGHT);
    numOfCircle = 0;
    SetDlgItemText(IDC_EDIT_COORDS, _T(""));
    UpdateDisplay();
}

void CMFCGLIMSWDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    int imgX = point.x;
    int imgY = point.y - 50;
    int r = static_cast<int>(ClickPointCircleRadius);
    if (numOfCircle == 3)
    {
        for (int i = 0; i < 3; i++)
        {
            int dx = imgX - m_circleCenters[i].x;
            int dy = imgY - m_circleCenters[i].y;
            if (dx * dx + dy * dy <= r * r)
            {
                m_bDragging = true;
                m_nDraggingIndex = i;
                SetCapture();
                return;
            }
        }
        return;
    }
    else
    {
        if (imgX - r < 0 || imgY - r < 0 ||
            imgX + r >= IMAGE_WIDTH || imgY + r >= IMAGE_HEIGHT)
        {
            return;
        }
        int nGray = (numOfCircle == 0) ? 60 : (numOfCircle == 1 ? 120 : 180);
        unsigned char* fm = (unsigned char*)m_image.GetBits();
        drawCircle(fm, imgX - r, imgY - r, r, nGray);
        UpdateDisplay();
        m_circleCenters[numOfCircle] = CPoint(imgX, imgY);
        CString str;
        str.Format(_T("(%d, %d)\r\n"), imgX, imgY);
        CString currentText;
        GetDlgItemText(IDC_EDIT_COORDS, currentText);
        currentText += str;
        SetDlgItemText(IDC_EDIT_COORDS, currentText);
        numOfCircle++;
        if (numOfCircle == 3)
            RedrawAll();
    }
    CDialogEx::OnLButtonDown(nFlags, point);
}

void CMFCGLIMSWDlg::OnMouseMove(UINT nFlags, CPoint point)
{
    if (m_bDragging && numOfCircle == 3 && m_nDraggingIndex >= 0 && m_nDraggingIndex < 3)
    {
        int newX = point.x;
        int newY = point.y - 50;
        m_circleCenters[m_nDraggingIndex] = CPoint(newX, newY);
        RedrawAll();
    }
    CDialogEx::OnMouseMove(nFlags, point);
}

void CMFCGLIMSWDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_bDragging)
    {
        m_bDragging = false;
        m_nDraggingIndex = -1;
        ReleaseCapture();

        if (numOfCircle == 3)
        {
            CString coordText;
            coordText.Format(_T("(%d, %d)\r\n(%d, %d)\r\n(%d, %d)"),
                m_circleCenters[0].x, m_circleCenters[0].y,
                m_circleCenters[1].x, m_circleCenters[1].y,
                m_circleCenters[2].x, m_circleCenters[2].y);
            SetDlgItemText(IDC_EDIT_COORDS, coordText);
        }
    }
    CDialogEx::OnLButtonUp(nFlags, point);
}


void CMFCGLIMSWDlg::OnBnClickedButtonRandom()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    srand((unsigned)(time(NULL) ^ li.LowPart));

    OnBnClickedButtonReset();
    int nPitch = m_image.GetPitch();
    unsigned char* fm = (unsigned char*)m_image.GetBits();
    int r = static_cast<int>(ClickPointCircleRadius);
    int circlesDrawn = 0;
    const int maxAttempts = 1000;
    int attempts = 0;
    while (circlesDrawn < 3 && attempts < maxAttempts)
    {
        int centerX = r + rand() % (IMAGE_WIDTH - 2 * r);
        int centerY = r + rand() % (IMAGE_HEIGHT - 2 * r);

        if (centerX - r < 0 || centerY - r < 0 ||
            centerX + r >= IMAGE_WIDTH || centerY + r >= IMAGE_HEIGHT)
        {
            attempts++;
            continue;
        }
        int nGray = (circlesDrawn == 0) ? 60 : (circlesDrawn == 1 ? 120 : 180);
        drawCircle(fm, centerX - r, centerY - r, r, nGray);
        m_circleCenters[circlesDrawn] = CPoint(centerX, centerY);
        circlesDrawn++;
        CString str;
        str.Format(_T("(%d, %d)\r\n"), centerX, centerY);
        CString currentText;
        GetDlgItemText(IDC_EDIT_COORDS, currentText);
        currentText += str;
        SetDlgItemText(IDC_EDIT_COORDS, currentText);
        attempts++;
    }
    numOfCircle = circlesDrawn;
    UpdateDisplay();
    if (numOfCircle == 3)
        RedrawAll();

    AfxBeginThread(AutoMoveRingThreadProc, this);
}

UINT CMFCGLIMSWDlg::AutoMoveRingThreadProc(LPVOID pParam)
{
    CMFCGLIMSWDlg* pDlg = (CMFCGLIMSWDlg*)pParam;
    if (!pDlg)
        return 0;
    int r = static_cast<int>(pDlg->ClickPointCircleRadius);
    
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    srand((unsigned)(time(NULL) ^ li.LowPart));
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            int x = r + rand() % (IMAGE_WIDTH - 2 * r);
            int y = r + rand() % (IMAGE_HEIGHT - 2 * r);
            pDlg->m_circleCenters[j] = CPoint(x, y);
        }
        pDlg->numOfCircle = 3;
        // UI thread
        pDlg->PostMessage(WM_UPDATE_RANDOM_RING, 0, 0);
        Sleep(500);
    }
    return 0;
}
LRESULT CMFCGLIMSWDlg::OnUpdateRandomRing(WPARAM wParam, LPARAM lParam)
{
    RedrawAll();
    if (numOfCircle == 3)
    {
        CString coordText;
        coordText.Format(_T("(%d, %d)\r\n(%d, %d)\r\n(%d, %d)"),
            m_circleCenters[0].x, m_circleCenters[0].y,
            m_circleCenters[1].x, m_circleCenters[1].y,
            m_circleCenters[2].x, m_circleCenters[2].y);
        SetDlgItemText(IDC_EDIT_COORDS, coordText);
    }
    return 0;
}
