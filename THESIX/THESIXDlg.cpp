
// THESIXDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "THESIX.h"
#include "THESIXDlg.h"
#include "afxdialogex.h"
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
#include<stack>
#include<vector>
//������<Windows.h>��Ϊ�����������е���GetTickCount()
//��������㲩���㷨��ʱ�����ģ���λΪ���룩

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/////////�ؼ�����//////////////
CBitmap m_BoardBmp;
CImageList m_Pieces;
int m_nBoardWidth;
int m_nBoardHeight;

//////Դ����//////


//***********************************************//
//***********************************************//
//******************ע��***************************//
//��Ч������Ʒ���ȽǶȿ��ǣ�ȡ���˱����е�Piece�࣬�������Ա��Ϊ��ȫ�ֵ�


//�����������ؼ���ջ

stack<Point>piecekilled;//����ջ���洢�����ӵ�����
stack<Move>Movestack;//����ջ���洢���ӵ��ƶ����

//��������Ҫ������
int side=0;//��ʾ���巽�ı�����0��ʾ��ң�1��ʾ����

int number[2]= {11,11}; //ʣ����������number[0]��ʾ���ʣ����������number[1]��ʾ����ʣ��������

//�������飬��ʾ����
int Board[8][7]= {
	{3,3,3,3,3,3,3},
	{3,1,1,1,1,1,3},
	{3,2,1,1,1,2,3},
	{3,1,2,1,2,1,3},
	{3,0,2,0,2,0,3},
	{3,2,0,0,0,2,3},
	{3,0,0,0,0,0,3},
	{3,3,3,3,3,3,3}
};
//�������飬���� ��ʾ ���������ĸ����򣬵�һά�ȱ�ʾ��Ӧrow���Ƕ�Ӧcol���ڶ�ά�ȱ�ʾ�ĸ�����
//������һ������row=row+direction[0][0],col=col+direction[1][0]
//���K��������һ�����ʾΪ��row=row+direction[0][k],col=col+direction[1][k](ע�⣬k���㿪ʼ��
int direction[2][4]= {0,-1,0,1,
	1,0,-1,0
};

//���ֺ���
void changeside()
{
	side = 1 - side;
}


//���Ӻ��������س��Ӹ���
//ע�⣬Ŀǰ�ĳ��Ӻ���ֻʵ�������ӳ�һ�ӵĹ��򣬻�û��ʵ�����Ӹ��ճ�һ�ֵĹ���
int killpiece(Move& mk)
{
	int K=0;//��¼���ӵĸ���
	int R=mk.to.row,C=mk.to.col;
	//�����ĸ�����
	for(int i=0; i!=4; ++i)
	{
		//ȡ�ÿ��ܱ���������
		int KR=R+2*direction[0][i];
		int KC=C+2*direction[1][i];
		//�ж����ӵ�����
		if(Board[R+direction[0][i]][C+direction[1][i]]==side)
		{
			if(Board[KR][KC]==1-side&&Board[KR+direction[0][i]][KC+direction[1][i]]!=1-side)
			{
				if(Board[R-direction[0][i]][C-direction[1][i]]!=side)
				{

					Board[KR][KC]=2;
					number[1-side]-=1;
					//���Ե������ӵ�����ѹ�����ջ
					piecekilled.push(Point(KR,KC));
					K+=1;//�ۻ����ӵĸ���
				}
			}
		}
	}
	for(int i=0; i!=4; ++i)
	{
		//ȡ�ÿ��ܱ���������
		int KR=R+direction[0][i];
		int KC=C+direction[1][i];
		if(Board[R-direction[0][i]][C-direction[1][i]]==side)
		{
			if(Board[KR][KC]==1-side&&Board[KR+direction[0][i]][KC+direction[1][i]]!=1-side)
			{
				if(Board[R-2*direction[0][i]][C-2*direction[1][i]]!=side)
				{

					Board[KR][KC]=2;
					number[1-side]-=1;
					//���Ե������ӵ�����ѹ�����ջ
					piecekilled.push(Point(KR,KC));
					K+=1;//�ۻ����ӵĸ���
				}
			}
		}
	}



	//���س�����
	return K;
}

//�������Ӻ�����ʵ��
void resurrect(Move& mk)
{
	if(mk.killingnumber!=0)
	{
		for(int i=1; i<mk.killingnumber+1; i++)
		{
			Board[piecekilled.top().row][piecekilled.top().col]=side;
			number[side]+=1;
			piecekilled.pop();
		}
	}
}

//�߷�����������ʵ��
void unMove()
{

	if(!Movestack.empty())
	{

		Move lastMove=Movestack.top();
		Movestack.pop();
		Board[lastMove.from.row][lastMove.from.col]=1-side;
		Board[lastMove.to.row][lastMove.to.col]=2;
		resurrect(lastMove);
		changeside();
	}

	//else
	//{
	//	cout<<"You haven't taken a step."<<endl;
	//}

}
//���庯����ʵ��
bool Movepiece(Move& newmove)
{


	//����
	Board[newmove.from.row][newmove.from.col]=2;
	Board[newmove.to.row][newmove.to.col]=side;
	//����,�����ӵĸ�������newmove�����ݳ�Ա��
	newmove.killingnumber=killpiece(newmove);
	Movestack.push(newmove);
	changeside();
	return true;


}


//������ǰ��������߷�����
//��������Ϊvector<Move>������,  �䱾�ʾ���Move����
//�������ص�ǰ������ܵ��߷���
int possibleMove(vector<Move>&PM)
{
	if(!PM.empty())
	{
		PM.clear();
	}

	//��������
	for(int i=1; i<7; i++)
	{
		for(int j=1; j<6; j++)
		{
			if(Board[i][j]==side)
			{
				//�����ĸ�����
				for(int k=0; k!=4; ++k)
				{
					int DR=i+direction[0][k];
					int DC=j+direction[1][k];
					if(Board[DR][DC]==2)
					{
						//�����ֿ��ܵ��߷�����Move�����У�Push_back������stlΪvector����õĲ�����ʵ����ĩβ����Ĺ���
						PM.push_back(Move(Point(i,j),Point(DR,DC)));
					}
				}
			}
		}
	}
	//�����߷���
	return PM.size();
}

//������������������ֵΪ��ǰ�����ֵ
//��������ÿ������ֵ15��һ��������Χ���м��������򼺷���4
//���ǶԷ���������Է���4������ǿ�λ�򼺷���2
//����������������Ҫ�������ڵĴ����������޸Ĳ��ܵ�������
int evaluate()
{
	int player=0;
	int computer=0;

	for(int i=1; i<7; i++)
	{
		for(int j=1; j<6; j++)
		{
			if(Board[i][j]==0)
			{

				player+=20;//ÿ����ֵΪ20��
				for(int k=0,space=0; k!=4; k++)
				{
					if(Board[i+direction[0][k]][j+direction[1][k]]==1)
						computer+=4;
					else if(Board[i+direction[0][k]][j+direction[1][k]]==2)
					{player+=2;space+=1;}
					else
						player+=4;
					if(!space)
						player-=5;
				}



			}
			else if(Board[i][j]==1)
			{
				computer+=15;

				for(int k=0,space=0; k!=4; k++)
				{
					if(Board[i+direction[0][k]][j+direction[1][k]]==1)
						computer+=4;
					else if(Board[i+direction[0][k]][j+direction[1][k]]==2)
					{computer+=2;space+=1;}
					else
						player+=4;
					if(!space)
						computer-=5;
				}
			}
		}
	}


	if(side)
	{

		return computer-player;
	}
	else
	{
		return player-computer;
	}


}

//����ȫ�ֱ���bestmove ���ڱ�����õ�һ���߷�
Move bestmove; int maxdepth=6;



//AlphaBeta�����㷨������Alpha������ֵ
//ע�⣬alpha��ֵֻ���ڸú����������ҵ���ʱʹ��
//�ں���ִ�й����У�����ҵ�����õ��߷����浽���涨���ȫ�ֱ���bestmove��
//bestmove������������Ҫʹ�õı���

int AlphaBetaSearch(int depth,int alpha=-10000,int beta=10000)//alpha��beta�ĳ�ʼֵӦΪ������������ӦΪ����Ĺ�ֵ�����޵ģ���������-10000��10000ʵ���Ͼ��൱�ڸ������������
{
	int value;
	
	vector<Move>PM;
	Move mv;
	

	if(depth==0)
		return evaluate();
	int number=possibleMove(PM);
	for(int i=0; i!=number; ++i)
	{
		mv=PM[i];
		Movepiece(mv);
		value=-AlphaBetaSearch(depth-1,-beta,-alpha);
		unMove();
		if(value>=beta)
		{
			return beta;
		}
		if(value>alpha)
		{
			alpha=value;
			if(depth==maxdepth)
			{
				bestmove=mv;
				
			}



		}
		

	}
	return alpha;
}

//////////////////////////////
/////////////////////////
///////////////////////////////



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTHESIXDlg �Ի���



CTHESIXDlg::CTHESIXDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTHESIXDlg::IDD, pParent)
	, m_isbegun(false)
	, m_ispicked(false)
	, m_lastpoint()
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_maxdepth = 3;
}

void CTHESIXDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_BUTTON1, m_btButton);
	DDX_Control(pDX, IDC_BUTTON1, m_sp);
}

BEGIN_MESSAGE_MAP(CTHESIXDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_SAVE_GAME, &CTHESIXDlg::OnSaveGame)
	ON_COMMAND(ID_LOAD_GAME, &CTHESIXDlg::OnLoadGame)
	ON_COMMAND(ID_NEW_GAME, &CTHESIXDlg::OnNewGame)
	ON_COMMAND(ID_QUIT_GAME, &CTHESIXDlg::OnQuitGame)
	ON_COMMAND(ID_UNMOVE, &CTHESIXDlg::OnUnmove)
	ON_COMMAND(ID_PAUSE, &CTHESIXDlg::OnPause)
	ON_BN_CLICKED(IDC_BUTTON1, &CTHESIXDlg::OnBnClickedButton1)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CTHESIXDlg ��Ϣ�������

BOOL CTHESIXDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	BITMAP BitMap;
	m_BoardBmp.LoadBitmapW(IDB_Board);
	m_BoardBmp.GetBitmap(&BitMap);
	m_nBoardWidth = BitMap.bmWidth;
	m_nBoardHeight = BitMap.bmHeight;
	m_BoardBmp.DeleteObject();

	m_Pieces.Create(45, 43, ILC_COLORDDB|ILC_MASK|ILC_COLOR24, 5, 0);

	CBitmap bitmap;   
	bitmap.LoadBitmap(IDB_PIECES);   
	m_Pieces.Add(&bitmap, RGB(255,255,255));

	buttonbit.LoadBitmapW(IDB_Start);
	m_sp.SetBitmap((HBITMAP)buttonbit);


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CTHESIXDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTHESIXDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{

	}
	


		CDialogEx::OnPaint();

}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CTHESIXDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTHESIXDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if(m_isbegun)
	{
		Point mypoint=transform(point);
		int row=mypoint.row;
		int col=mypoint.col;
		vector<Move> AM;
		if(!m_ispicked)
		{
			if(Board[row][col]==0)//ѡ�б�������
			{
				m_ispicked=true;
				m_lastpoint=mypoint;
			}
			else if(Board[row][col]==1)
			{
				MessageBeep(MB_ICONERROR);
				MessageBox(_T("�ⲻ���������"));
			}
			else
			{
				MessageBeep(MB_ICONERROR);
				MessageBox(_T("����û������"));
			}
		}
		else
		{
			if(Board[row][col]==2)
			{
				if(
					(m_lastpoint.row==row && (m_lastpoint.col==col-1 || m_lastpoint.col==col+1)) || 
					(m_lastpoint.col==col && (m_lastpoint.row==row+1 || m_lastpoint.row==row-1))
					)
				{	

					Board[row][col]=0;
					Board[m_lastpoint.row][m_lastpoint.col]=2;
					PlaySound((LPCTSTR)IDR_PUT, AfxGetInstanceHandle(), SND_RESOURCE | SND_SYNC);
					Movepiece(Move(m_lastpoint,mypoint));
					m_ispicked=false;
					
					Invalidate();
					UpdateWindow();

					//////////////�����ƶ�
					if(!possibleMove(AM) || (number[1]==1&&number[0]>1))
					{
						PlaySound((LPCTSTR)IDR_WIN, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
						if(IDYES==MessageBox(_T("��Ӯ�ˣ��Ƿ�����һ�֣�"),_T("��ʾ"), MB_YESNO))
							SendMessage(ID_NEW_GAME);
						m_isbegun=false;
					}
					else
					{
						AlphaBetaSearch(maxdepth);
						Movepiece(bestmove);
						PlaySound((LPCTSTR)IDR_PUT, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
						Invalidate();
						UpdateWindow();
						if(!possibleMove(AM) || (number[0]==1 && number[1]>1))
						{
							PlaySound((LPCTSTR)IDR_LOST, AfxGetInstanceHandle(),SND_RESOURCE | SND_ASYNC);
							if(IDYES==MessageBox(_T("�����ˣ��Ƿ�����һ�֣�"),_T("��ʾ"), MB_YESNO))
								SendMessage(ID_NEW_GAME);
						}
					}
				}
				else
				{
					MessageBox(_T("һ��ֻ���ƶ�һ��"));
				}
			}
			else if(Board[row][col]==0)//����ѡ������
			{
				m_lastpoint=mypoint;
			}
			else if(Board[row][col]==1)
			{
				MessageBeep(MB_ICONERROR);
				MessageBox(_T("�ⲻ����������"));
			}
		}
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}


Point CTHESIXDlg::transform(CPoint mypoint)
{

	return Point((mypoint.y-165)/50+1,(mypoint.x-150)/47+1);
}


void CTHESIXDlg::OnSaveGame()
{
	// TODO: �ڴ���������������
	CFile save(_T("Save.txt"),CFile::modeReadWrite | CFile::modeCreate);
	int board[8*7];
	for(int i=0;i<8;i++)
		for(int j=0;j<7;j++)
		{
			board[i*7+j]=Board[i][j];
		}
		save.Write(board,sizeof(board));
		save.Flush();
		MessageBeep(MB_ICONINFORMATION);
		MessageBox(_T("����ɹ���"));
}


void CTHESIXDlg::OnLoadGame()
{
	// TODO: �ڴ���������������
	CFile load(_T("Save.txt"),CFile::modeReadWrite);
	load.SeekToBegin();
	int board[8*7];
	load.Read(board, sizeof(board));
	for(int i=0;i<8;i++)
		for(int j=0;j<7;j++)
			Board[i][j]=board[i*7+j];		
	Invalidate();
	UpdateWindow();
	MessageBeep(MB_ICONINFORMATION);
	MessageBox(_T("��ȡ�ɹ���"));

}


void CTHESIXDlg::OnNewGame()
{
	// TODO: �ڴ���������������
	Board[1][1]=Board[1][2]=Board[1][3]=Board[1][4]=Board[1][5]=Board[2][2]=Board[2][3]=Board[2][4]=Board[3][1]=Board[3][3]=Board[3][5]=1;
	Board[2][1]=Board[2][5]=Board[3][2]=Board[3][4]=Board[5][1]=Board[5][5]=Board[4][2]=Board[4][4]=2;
	Board[6][1]=Board[6][2]=Board[6][3]=Board[6][4]=Board[6][5]=Board[5][2]=Board[5][3]=Board[5][4]=Board[4][1]=Board[4][3]=Board[4][5]=0;
	side=0;
	CBitmap my;

	my.LoadBitmapW(IDB_Start);
	m_sp.SetBitmap((HBITMAP)my);
	m_isbegun=false;
	Invalidate();
	UpdateWindow();
}


void CTHESIXDlg::OnQuitGame()
{
	// TODO: �ڴ���������������
	MessageBeep(MB_ICONQUESTION);
	if ( IDYES == MessageBox( _T("ȷ��Ҫ�˳���"), _T("������"), MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2 ) )
		CDialog::OnCancel();
}


void CTHESIXDlg::OnUnmove()
{
	// TODO: �ڴ���������������
	unMove();
	unMove();
	Invalidate();
	UpdateWindow();
}


void CTHESIXDlg::OnPause()
{
	// TODO: �ڴ���������������
	if(m_isbegun)
		m_isbegun=false;
	else
		m_isbegun=true;
}


void CTHESIXDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CBitmap my;

	if(m_isbegun)
	{
		my.LoadBitmapW(IDB_Start);
		m_sp.SetBitmap((HBITMAP)my);
		m_isbegun=false;
	}


	else
	{my.LoadBitmapW(IDB_Pause);
	m_sp.SetBitmap((HBITMAP)my);
	m_isbegun=true;}
}


BOOL CTHESIXDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	CDC MemDC;
	CBitmap *pOldBmp;
	POINT pt;
	int z;
	MemDC.CreateCompatibleDC(pDC);
	m_BoardBmp.LoadBitmapW(IDB_Board);
	pOldBmp = MemDC.SelectObject(&m_BoardBmp);
	for(int x=1; x<7; ++x)
		for(int y=1; y<6; ++y)
		{

			if(Board[x][y]==2)
				continue;
			else if(Board[x][y]==0)
			{
				z=2;
			}
			else if(Board[x][y]==1)
			{
				z=1;
			}			
			//���ڿ��Զ�����λ������switch case������΢����
			pt.x=150+(y-1)*47;
			pt.y=165+(x-1)*50;

			m_Pieces.Draw(&MemDC, z, pt, ILD_TRANSPARENT);

		}
		pDC->BitBlt( 12, 13, m_nBoardWidth, m_nBoardHeight, &MemDC, 0, 0, SRCCOPY);
		MemDC.SelectObject(&pOldBmp);
		MemDC.DeleteDC(); 
		m_BoardBmp.DeleteObject();

	return true;
}
