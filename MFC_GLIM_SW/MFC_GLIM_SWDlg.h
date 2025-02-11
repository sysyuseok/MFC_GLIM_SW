#pragma once
#include <atlimage.h>  // CImage 사용

// CMFCGLIMSWDlg dialog
class CMFCGLIMSWDlg : public CDialogEx
{
private:
    CImage m_image;
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
    // 기본 메시지 처리
    afx_msg BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();

    // 편집 컨트롤 관련 버튼 핸들러
    afx_msg void OnBnClickedButton1();
    afx_msg void OnBnClickedButton2();

    // 원 및 링 관련 멤버 변수
    double OuterCircleThickness;    // 링(정원) 두께
    double ClickPointCircleRadius;  // 원의 반지름
    int numOfCircle;                // 생성된 원의 개수 (최대 3)
    CPoint m_circleCenters[3];      // 3개 원의 중심 좌표

    // 그리기 함수들
    void UpdateDisplay();
    void RedrawAll();
    void drawCircle(unsigned char* fm, int x, int y, int nRadius, int nGray);
    void drawRing(unsigned char* fm, double centerX, double centerY, double R, double thickness, int nGray);
    bool isInCircle(int i, int j, int nCenterX, int nCenterY, int nRadius);

    // Reset, Random 버튼 핸들러
    afx_msg void OnBnClickedButtonReset();
    afx_msg void OnBnClickedButtonRandom();

    // [추가] 드래그 관련 (기능1,2)
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    bool m_bDragging;      // 드래그 진행 여부
    int  m_nDraggingIndex; // 드래그 중인 원의 인덱스 (0 ~ 2)

    // [추가] 자동 랜덤 이동/링 갱신 (기능3, 별도 쓰레드 이용)
    static UINT AutoMoveRingThreadProc(LPVOID pParam);
    afx_msg LRESULT OnUpdateRandomRing(WPARAM wParam, LPARAM lParam);
};
