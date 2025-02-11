#pragma once
#include <atlimage.h>  // CImage ���

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
    // �⺻ �޽��� ó��
    afx_msg BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();

    // ���� ��Ʈ�� ���� ��ư �ڵ鷯
    afx_msg void OnBnClickedButton1();
    afx_msg void OnBnClickedButton2();

    // �� �� �� ���� ��� ����
    double OuterCircleThickness;    // ��(����) �β�
    double ClickPointCircleRadius;  // ���� ������
    int numOfCircle;                // ������ ���� ���� (�ִ� 3)
    CPoint m_circleCenters[3];      // 3�� ���� �߽� ��ǥ

    // �׸��� �Լ���
    void UpdateDisplay();
    void RedrawAll();
    void drawCircle(unsigned char* fm, int x, int y, int nRadius, int nGray);
    void drawRing(unsigned char* fm, double centerX, double centerY, double R, double thickness, int nGray);
    bool isInCircle(int i, int j, int nCenterX, int nCenterY, int nRadius);

    // Reset, Random ��ư �ڵ鷯
    afx_msg void OnBnClickedButtonReset();
    afx_msg void OnBnClickedButtonRandom();

    // [�߰�] �巡�� ���� (���1,2)
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    bool m_bDragging;      // �巡�� ���� ����
    int  m_nDraggingIndex; // �巡�� ���� ���� �ε��� (0 ~ 2)

    // [�߰�] �ڵ� ���� �̵�/�� ���� (���3, ���� ������ �̿�)
    static UINT AutoMoveRingThreadProc(LPVOID pParam);
    afx_msg LRESULT OnUpdateRandomRing(WPARAM wParam, LPARAM lParam);
};
