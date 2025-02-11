// MFC_GLIM_SWDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "MFC_GLIM_SW.h"
#include "MFC_GLIM_SWDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <cmath>
#include "resource.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

#define IMAGE_WIDTH 700
#define IMAGE_HEIGHT 400
#define IMAGE_BPP 8


class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_ABOUTBOX };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

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
    , OuterCircleThickness(20)
    , ClickPointCircleRadius(20)
    , numOfCircle(0)
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
    ON_BN_CLICKED(IDC_BUTTON_RESET, &CMFCGLIMSWDlg::OnBnClickedButtonReset)
    ON_BN_CLICKED(IDC_BUTTON_RANDOM, &CMFCGLIMSWDlg::OnBnClickedButtonRandom)
END_MESSAGE_MAP()


BOOL CMFCGLIMSWDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    ModifyStyle(0, WS_CLIPCHILDREN);

    // m_image 생성 및 초기화
    m_image.Create(IMAGE_WIDTH, -IMAGE_HEIGHT, IMAGE_BPP);
    if (IMAGE_BPP == 8) {
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
    CPaintDC dc(this);
    if (IsIconic())
    {
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        m_image.Draw(dc, 0, 50);
    }
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

// (x,y) : 원의 좌측 상단, nRadius : 원의 반지름, nGray : 색상
void CMFCGLIMSWDlg::drawCircle(unsigned char* fm, int x, int y, int nRadius, int nGray)
{
    int nCenterX = x + nRadius;
    int nCenterY = y + nRadius;
    int nPitch = m_image.GetPitch();

    for (int i = y; i < y + nRadius * 2; i++) {
        for (int j = x; j < x + nRadius * 2; j++) {
            if (isInCircle(i, j, nCenterX, nCenterY, nRadius))
                fm[i * nPitch + j] = nGray;
        }
    }
}

// centerX, centerY : 링 중심, R : 외접원의 반지름, thickness : 링 두께, nGray : 색상
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

    for (int i = iy; i <= ey; i++) {
        for (int j = ix; j <= ex; j++) {
            double dx = j - centerX;
            double dy = i - centerY;
            double dist2 = dx * dx + dy * dy;
            if (dist2 >= inner2 && dist2 <= outer2)
                fm[i * nPitch + j] = nGray;
        }
    }
}


void CMFCGLIMSWDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (numOfCircle == 3)
        return;

    int nPitch = m_image.GetPitch();
    unsigned char* fm = (unsigned char*)m_image.GetBits();

    int centerX = point.x;
    int centerY = point.y - 50;
    int r = static_cast<int>(ClickPointCircleRadius);

    
    if (centerX - r < 0 || centerY - r < 0 ||
        centerX + r >= IMAGE_WIDTH || centerY + r >= IMAGE_HEIGHT)
    {
        return;
    }

    //Change the color by order
    int nGray = 0;
    if (numOfCircle == 0)
        nGray = 60;
    else if (numOfCircle == 1)
        nGray = 120;
    else if (numOfCircle == 2)
        nGray = 180;

    drawCircle(fm, centerX - r, centerY - r, r, nGray);
    UpdateDisplay();

    m_circleCenters[numOfCircle] = CPoint(centerX, centerY);
    CString str;
    str.Format(_T("(%d, %d)\r\n"), centerX, centerY);
    CString currentText;
    GetDlgItemText(IDC_EDIT_COORDS, currentText);
    currentText += str;
    SetDlgItemText(IDC_EDIT_COORDS, currentText);

    numOfCircle++;

    if (numOfCircle == 3)
    {
        // 세 점 A, B, C
        CPoint A = m_circleCenters[0];
        CPoint B = m_circleCenters[1];
        CPoint C = m_circleCenters[2];

        double Ax = A.x, Ay = A.y;
        double Bx = B.x, By = B.y;
        double Cx = C.x, Cy = C.y;
        double D = 2 * (Ax * (By - Cy) + Bx * (Cy - Ay) + Cx * (Ay - By));
        if (fabs(D) < 1e-6)
            return;

        double center_x = ((Ax * Ax + Ay * Ay) * (By - Cy) +
            (Bx * Bx + By * By) * (Cy - Ay) +
            (Cx * Cx + Cy * Cy) * (Ay - By)) / D;
        double center_y = ((Ax * Ax + Ay * Ay) * (Cx - Bx) +
            (Bx * Bx + By * By) * (Ax - Cx) +
            (Cx * Cx + Cy * Cy) * (Bx - Ax)) / D;
        double R = sqrt((Ax - center_x) * (Ax - center_x) + (Ay - center_y) * (Ay - center_y));

        drawRing(fm, center_x, center_y, R, OuterCircleThickness, 50);
        UpdateDisplay();
    }
}


void CMFCGLIMSWDlg::UpdateDisplay()
{
    CClientDC dc(this);
    m_image.Draw(dc, 0, 50);
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


void CMFCGLIMSWDlg::OnBnClickedButtonRandom()
{
    OnBnClickedButtonReset();

    int nPitch = m_image.GetPitch();
    unsigned char* fm = (unsigned char*)m_image.GetBits();
    int r = static_cast<int>(ClickPointCircleRadius);
    int circlesDrawn = 0;
    const int maxAttempts = 1000;
    int attempts = 0;

    while (circlesDrawn < 3 && attempts < maxAttempts)
    {
        // centerX는 [r, IMAGE_WIDTH - r-1], centerY는 [r, IMAGE_HEIGHT - r-1]
        int centerX = r + rand() % (IMAGE_WIDTH - 2 * r);
        int centerY = r + rand() % (IMAGE_HEIGHT - 2 * r);

        // (추가 안전검사: 원이 범위를 벗어나면 continue)
        if (centerX - r < 0 || centerY - r < 0 ||
            centerX + r >= IMAGE_WIDTH || centerY + r >= IMAGE_HEIGHT)
        {
            attempts++;
            continue;
        }

        int nGray = 0;
        if (circlesDrawn == 0)
            nGray = 60;
        else if (circlesDrawn == 1)
            nGray = 120;
        else if (circlesDrawn == 2)
            nGray = 180;

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
    {
        CPoint A = m_circleCenters[0];
        CPoint B = m_circleCenters[1];
        CPoint C = m_circleCenters[2];

        double Ax = A.x, Ay = A.y;
        double Bx = B.x, By = B.y;
        double Cx = C.x, Cy = C.y;
        double D = 2 * (Ax * (By - Cy) + Bx * (Cy - Ay) + Cx * (Ay - By));
        if (fabs(D) < 1e-6)
            return;

        double center_x = ((Ax * Ax + Ay * Ay) * (By - Cy) +
            (Bx * Bx + By * By) * (Cy - Ay) +
            (Cx * Cx + Cy * Cy) * (Ay - By)) / D;
        double center_y = ((Ax * Ax + Ay * Ay) * (Cx - Bx) +
            (Bx * Bx + By * By) * (Ax - Cx) +
            (Cx * Cx + Cy * Cy) * (Bx - Ax)) / D;
        double R = sqrt((Ax - center_x) * (Ax - center_x) + (Ay - center_y) * (Ay - center_y));

        drawRing(fm, center_x, center_y, R, OuterCircleThickness, 50);
        UpdateDisplay();
    }
}