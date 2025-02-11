#pragma once
#include <atlimage.h>  // CImage »ç¿ë

// CMFCGLIMSWDlg dialog
class CMFCGLIMSWDlg : public CDialogEx
{
public:
    CMFCGLIMSWDlg(CWnd* pParent = nullptr);    // standard constructor

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_MFC_GLIM_SW_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    HICON m_hIcon;
    DECLARE_MESSAGE_MAP()

public:
    
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();

    afx_msg void OnBnClickedButton1();
    afx_msg void OnBnClickedButton2();

    
    double OuterCircleThickness;    
    double ClickPointCircleRadius;  
    int numOfCircle;               
    CImage m_image;                
    CPoint m_circleCenters[3];

    void UpdateDisplay();
    void RedrawAll();
    void drawCircle(unsigned char* fm, int x, int y, int nRadius, int nGray);
    void drawRing(unsigned char* fm, double centerX, double centerY, double R, double thickness, int nGray);
    bool isInCircle(int i, int j, int nCenterX, int nCenterY, int nRadius);

    afx_msg void OnBnClickedButtonReset();
    afx_msg void OnBnClickedButtonRandom();

    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    bool m_bDragging;      
    int  m_nDraggingIndex; 

    static UINT AutoMoveRingThreadProc(LPVOID pParam);
    afx_msg LRESULT OnUpdateRandomRing(WPARAM wParam, LPARAM lParam);
};
