
// MFC_GLIM_SWDlg.h : header file
//

#pragma once


// CMFCGLIMSWDlg dialog
class CMFCGLIMSWDlg : public CDialogEx
{
private:
	CImage m_image;
// Construction
public:
	CMFCGLIMSWDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_GLIM_SW_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	double OuterCircleThickness;
	double ClickPointCircleRadius;
	int numOfCircle;
	void UpdateDisplay();
	CPoint m_circleCenters[3];
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void drawCircle(unsigned char* fm, int x, int y, int nRadius, int nGray);
	afx_msg void drawRing(unsigned char* fm, double centerX, double centerY, double R, double thickness, int nGray);
	afx_msg bool isInCircle(int i, int j, int nCenterX, int nCenterY, int nRadius);
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnBnClickedButtonRandom();
};
