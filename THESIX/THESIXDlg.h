
// THESIXDlg.h : 头文件
//

#pragma once
#include "gdiplustypes.h"
#include "afxext.h"
#include "afxwin.h"

struct Point
{
	//数据成员，分别表示行和列
	int row;
	int col;
	//Point的构造函数
	Point(int ROW=1, int COL=1):row(ROW), col(COL) {}
};


//Move类，记录每一步的移动情况
struct Move
{
	//数据成员
	int killingnumber;  //记录该步的吃子个数
	Point from;  //出发点坐标
	Point to;   //目的地坐标

	//构造函数，killingnumber默认值为0
	Move(Point& P1, Point& P2,int a=0):from(P1), to(P2),killingnumber(a) {}
	//默认构造函数
	Move() {
	from=Point(0,0);
	to=Point(0,0);
	killingnumber=0;};
};
// CTHESIXDlg 对话框
class CTHESIXDlg : public CDialogEx
{
// 构造
public:
	CTHESIXDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_THESIX_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	bool m_isbegun;
	bool m_ispicked;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	Point transform(CPoint mypoint);
	Point m_lastpoint;
	afx_msg void OnSaveGame();
	afx_msg void OnLoadGame();
	afx_msg void OnNewGame();
	afx_msg void OnQuitGame();
	afx_msg void OnUnmove();
	afx_msg void OnPause();
//	CBitmapButton m_btButton;
	afx_msg void OnBnClickedButton1();
	CButton m_sp;
	CBitmap buttonbit;
	 int m_maxdepth;
	 afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
