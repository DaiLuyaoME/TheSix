
// THESIXDlg.cpp : 实现文件
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
//这里用<Windows.h>是为了在主函数中调用GetTickCount()
//借此来计算博弈算法的时间消耗（单位为毫秒）

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/////////控件代码//////////////
CBitmap m_BoardBmp;
CImageList m_Pieces;
int m_nBoardWidth;
int m_nBoardHeight;

//////源代码//////


//***********************************************//
//***********************************************//
//******************注意***************************//
//从效率与设计方便等角度考虑，取消了本来有的Piece类，而将其成员改为了全局的


//下面是两个关键的栈

stack<Point>piecekilled;//吃子栈，存储被吃子的坐标
stack<Move>Movestack;//走子栈，存储棋子的移动情况

//下面是重要的数据
int side=0;//表示走棋方的变量，0表示玩家，1表示电脑

int number[2]= {11,11}; //剩余棋子数，number[0]表示玩家剩余棋子数，number[1]表示电脑剩余棋子数

//棋盘数组，表示棋盘
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
//方向数组，依次 表示 东北西南四个方向，第一维度表示对应row还是对应col，第二维度表示哪个方向
//如向东走一步，则row=row+direction[0][0],col=col+direction[1][0]
//向第K个方向走一步则表示为，row=row+direction[0][k],col=col+direction[1][k](注意，k从零开始）
int direction[2][4]= {0,-1,0,1,
	1,0,-1,0
};

//换手函数
void changeside()
{
	side = 1 - side;
}


//吃子函数，返回吃子个数
//注意，目前的吃子函数只实现了两子吃一子的规则，还没有实现三子隔空吃一字的规则
int killpiece(Move& mk)
{
	int K=0;//记录吃子的个数
	int R=mk.to.row,C=mk.to.col;
	//遍历四个方向
	for(int i=0; i!=4; ++i)
	{
		//取得可能被吃子坐标
		int KR=R+2*direction[0][i];
		int KC=C+2*direction[1][i];
		//判定吃子的条件
		if(Board[R+direction[0][i]][C+direction[1][i]]==side)
		{
			if(Board[KR][KC]==1-side&&Board[KR+direction[0][i]][KC+direction[1][i]]!=1-side)
			{
				if(Board[R-direction[0][i]][C-direction[1][i]]!=side)
				{

					Board[KR][KC]=2;
					number[1-side]-=1;
					//将吃掉的棋子的坐标压入吃子栈
					piecekilled.push(Point(KR,KC));
					K+=1;//累积吃子的个数
				}
			}
		}
	}
	for(int i=0; i!=4; ++i)
	{
		//取得可能被吃子坐标
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
					//将吃掉的棋子的坐标压入吃子栈
					piecekilled.push(Point(KR,KC));
					K+=1;//累积吃子的个数
				}
			}
		}
	}



	//返回吃子数
	return K;
}

//复活棋子函数的实现
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

//走法撤销函数的实现
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
//走棋函数的实现
bool Movepiece(Move& newmove)
{


	//走棋
	Board[newmove.from.row][newmove.from.col]=2;
	Board[newmove.to.row][newmove.to.col]=side;
	//吃子,将吃子的个数存入newmove的数据成员中
	newmove.killingnumber=killpiece(newmove);
	Movestack.push(newmove);
	changeside();
	return true;


}


//搜索当前局面可能走法函数
//参数类型为vector<Move>的引用,  其本质就是Move数组
//函数返回当前局面可能的走法数
int possibleMove(vector<Move>&PM)
{
	if(!PM.empty())
	{
		PM.clear();
	}

	//遍历棋盘
	for(int i=1; i<7; i++)
	{
		for(int j=1; j<6; j++)
		{
			if(Board[i][j]==side)
			{
				//遍历四个方向
				for(int k=0; k!=4; ++k)
				{
					int DR=i+direction[0][k];
					int DC=j+direction[1][k];
					if(Board[DR][DC]==2)
					{
						//将该种可能的走法放入Move数组中，Push_back（）是stl为vector定义好的操作，实现在末尾插入的功能
						PM.push_back(Move(Point(i,j),Point(DR,DC)));
					}
				}
			}
		}
	}
	//返回走法数
	return PM.size();
}

//局面评估函数，返回值为当前局面估值
//评估规则：每个棋子值15，一个棋子周围如有己方棋子则己方加4
//如是对方的棋子则对方加4，如果是空位则己方加2
//局面评估函数还需要经过后期的大量测试逐步修改才能得以完善
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

				player+=20;//每个子值为20；
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

//定义全局变量bestmove 用于保存最好的一步走法
Move bestmove; int maxdepth=6;



//AlphaBeta搜索算法，返回Alpha的最终值
//注意，alpha的值只是在该函数进行自我迭代时使用
//在函数执行过程中，会把找到的最好的走法保存到上面定义的全局变量bestmove中
//bestmove才是我们最终要使用的变量

int AlphaBetaSearch(int depth,int alpha=-10000,int beta=10000)//alpha和beta的初始值应为负无穷和正无穷，应为局面的估值是有限的，所以这里-10000和10000实际上就相当于负无穷和正无穷
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



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
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


// CTHESIXDlg 对话框



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


// CTHESIXDlg 消息处理程序

BOOL CTHESIXDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
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


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTHESIXDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{

	}
	


		CDialogEx::OnPaint();

}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTHESIXDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTHESIXDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if(m_isbegun)
	{
		Point mypoint=transform(point);
		int row=mypoint.row;
		int col=mypoint.col;
		vector<Move> AM;
		if(!m_ispicked)
		{
			if(Board[row][col]==0)//选中本方棋子
			{
				m_ispicked=true;
				m_lastpoint=mypoint;
			}
			else if(Board[row][col]==1)
			{
				MessageBeep(MB_ICONERROR);
				MessageBox(_T("这不是你的棋子"));
			}
			else
			{
				MessageBeep(MB_ICONERROR);
				MessageBox(_T("这里没有棋子"));
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

					//////////////电脑移动
					if(!possibleMove(AM) || (number[1]==1&&number[0]>1))
					{
						PlaySound((LPCTSTR)IDR_WIN, AfxGetInstanceHandle(), SND_RESOURCE | SND_ASYNC);
						if(IDYES==MessageBox(_T("您赢了！是否再来一局？"),_T("提示"), MB_YESNO))
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
							if(IDYES==MessageBox(_T("您输了！是否再来一局？"),_T("提示"), MB_YESNO))
								SendMessage(ID_NEW_GAME);
						}
					}
				}
				else
				{
					MessageBox(_T("一次只能移动一格！"));
				}
			}
			else if(Board[row][col]==0)//重置选中棋子
			{
				m_lastpoint=mypoint;
			}
			else if(Board[row][col]==1)
			{
				MessageBeep(MB_ICONERROR);
				MessageBox(_T("这不是您的棋子"));
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
	// TODO: 在此添加命令处理程序代码
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
		MessageBox(_T("保存成功！"));
}


void CTHESIXDlg::OnLoadGame()
{
	// TODO: 在此添加命令处理程序代码
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
	MessageBox(_T("读取成功！"));

}


void CTHESIXDlg::OnNewGame()
{
	// TODO: 在此添加命令处理程序代码
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
	// TODO: 在此添加命令处理程序代码
	MessageBeep(MB_ICONQUESTION);
	if ( IDYES == MessageBox( _T("确定要退出吗？"), _T("五子棋"), MB_ICONQUESTION | MB_YESNO | MB_DEFBUTTON2 ) )
		CDialog::OnCancel();
}


void CTHESIXDlg::OnUnmove()
{
	// TODO: 在此添加命令处理程序代码
	unMove();
	unMove();
	Invalidate();
	UpdateWindow();
}


void CTHESIXDlg::OnPause()
{
	// TODO: 在此添加命令处理程序代码
	if(m_isbegun)
		m_isbegun=false;
	else
		m_isbegun=true;
}


void CTHESIXDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
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
			//后期可以对棋子位置利用switch case语句进行微调。
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
