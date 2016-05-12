
// THESIXDlg.h : ͷ�ļ�
//

#pragma once
#include "gdiplustypes.h"
#include "afxext.h"
#include "afxwin.h"

struct Point
{
	//���ݳ�Ա���ֱ��ʾ�к���
	int row;
	int col;
	//Point�Ĺ��캯��
	Point(int ROW=1, int COL=1):row(ROW), col(COL) {}
};


//Move�࣬��¼ÿһ�����ƶ����
struct Move
{
	//���ݳ�Ա
	int killingnumber;  //��¼�ò��ĳ��Ӹ���
	Point from;  //����������
	Point to;   //Ŀ�ĵ�����

	//���캯����killingnumberĬ��ֵΪ0
	Move(Point& P1, Point& P2,int a=0):from(P1), to(P2),killingnumber(a) {}
	//Ĭ�Ϲ��캯��
	Move() {
	from=Point(0,0);
	to=Point(0,0);
	killingnumber=0;};
};
// CTHESIXDlg �Ի���
class CTHESIXDlg : public CDialogEx
{
// ����
public:
	CTHESIXDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_THESIX_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
