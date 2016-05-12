
#include<iostream>
#include<stack>
#include<vector>
#include<Windows.h>
using namespace std;
//Point类，记录坐标用
struct Point{
			//数据成员，分别表示行和列
             int row;
             int col;
			 //Point的构造函数
             Point(int ROW=1, int COL=1):row(ROW), col(COL){}
             };
 //Move类，记录每一步的移动情况         
struct Move{
       int killingnumber;
       Point from;
       Point to;
       //构造函数，killingnumber默认值为0      
	   Move(Point& P1, Point& P2,int a=0):from(P1), to(P2),killingnumber(a){}
	   Move(){};
             };

stack<Point>piecekilled;//吃子栈，存储被吃子的坐标 
stack<Move>Movestack;//走子栈，存储棋子的移动情况

         
     


int side=1;
int number[2]={11,11};

int Board[8][7]={{3,3,3,3,3,3,3}, 
                        {3,1,1,1,1,1,3}, 
                        {3,2,1,1,1,2,3}, 
                        {3,1,2,1,2,1,3}, 
                        {3,0,2,0,2,0,3}, 
                        {3,2,0,0,0,2,3},
                        {3,0,0,0,0,0,3},
                        {3,3,3,3,3,3,3}};
int direction[2][4]={0,-1,0,1,
								1,0,-1,0};
void changeside(){
     side = 1 - side;
     }

			

int killpiece(Move& mk){
    int K=0;//记录吃子的个数
    int R=mk.to.row,C=mk.to.col;
    for(int i=0;i!=4;++i)
	{
		//取得被吃子坐标
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
                            K+=1;//返回吃子的个数
				}
			}
		}
	}
	 //返回吃子数
    return K;
}         

//复活棋子函数的实现
void resurrect(Move& mk){
     if(mk.killingnumber!=0){
                             for(int i=1;i<mk.killingnumber+1;i++)
							 {
                                Board[piecekilled.top().row][piecekilled.top().col]=side;
                                number[side]+=1;
                                piecekilled.pop();  
                                }                              
	 }                              
}   

//走法撤销函数的实现
void unMove(){
     
     if(!Movestack.empty()){
                            
         Move lastMove=Movestack.top();
         Movestack.pop();
         Board[lastMove.from.row][lastMove.from.col]=1-side;
         Board[lastMove.to.row][lastMove.to.col]=2;
         resurrect(lastMove);
         changeside();                             
         }                   
                                                               
     else{
          cout<<"You haven't taken a step."<<endl;}
               
               }
//走棋函数的实现
bool Movepiece(Move& newmove){
     bool flagyourpiece=0,flagspace=0;
    //判断是否是己方的棋子  
     if(Board[newmove.from.row][newmove.from.col]==side){
                                           flagyourpiece=1;}
     else{
          cout<<" This is NOT your piece"<<endl;
          }
	 //判断目的地是否是空位
     if(Board[newmove.to.row][newmove.to.col]==2){
                                                       flagspace=1;
                                                       }
     else{
          cout<<"Your step is fault"<<endl;
          }
     //移动棋子      
     if(flagyourpiece&&flagspace){
								//走棋
                                  Board[newmove.from.row][newmove.from.col]=2;
                                  Board[newmove.to.row][newmove.to.col]=side;
							    //吃子,将吃子的个数存入newmove的数据成员中
                                  newmove.killingnumber=killpiece(newmove);
                                  Movestack.push(newmove);
                                  changeside();
                                  return true;
                                  }
	 else{
		 return false;
	 }
} 



int possibleMove(vector<Move>&PM)
{
	if(!PM.empty())
	{
		PM.clear();
	}

	//遍历棋盘
    for(int i=1;i<7;i++)
	{
            for(int j=1;j<6;j++)
			{
                    if(Board[i][j]==side)
					{
						//遍历四个方向
                       for(int k=0;k!=4;++k)
					   {
						   int DR=i+direction[0][k];
						   int DC=j+direction[1][k];
						   if(Board[DR][DC]==2)
						   {
							   PM.push_back(Move(Point(i,j),Point(DR,DC)));
						   }
					   }
					}
			}
	}
	return PM.size();
} 




Move bestmove1;

  

//局面评估函数的实现
//评估规则：每个棋子值15，一个棋子周围如有己方棋子则己方加4
//如是对方的棋子则对方加4，如果是空位则己方加2
//局面评估函数还需要经过后期的大量测试逐步修改才能得以完善
int evaluate1()
{
	int player=0;
	int computer=0;
    for(int i=1;i<7;i++)
	{
            for(int j=1;j<6;j++)
			{
				if(Board[i][j]==0)
				{
					player+=15;//每个子值为15； 
					for(int k=0;k!=4;k++)
					{
						if(Board[i+direction[0][k]][j+direction[1][k]]==1)
							computer+=4;
						else if(Board[i+direction[0][k]][j+direction[1][k]]==2)
							player+=2;
						else
							player+=4;
					}
				}	
				else if(Board[i][j]==1)
				{
					computer+=15; 
					for(int k=0;k!=4;k++)
					{
						if(Board[i+direction[0][k]][j+direction[1][k]]==1)
							computer+=4;
						else if(Board[i+direction[0][k]][j+direction[1][k]]==2)
							computer+=2;
						else
							player+=4;
					}
				}
			}
	}
	//返回玩家（side=0）减去电脑（side=1)的值,值越大对玩家越有利，反之对电脑越有利
	return player-computer;
}
int minmaxsearch(int depth)
{
	int best,value;
	int maxdepth=depth;
    vector<Move>PM;
	Move mv;
	if(side)
		best=10000;
	else
		best=-10000;
	if(depth==0)
		return evaluate();
	int number=possibleMove(PM);
	for(int i=0;i!=number;++i)
	{
		mv=PM[i];
		Movepiece(mv);
		value=minmaxsearch(depth-1);
		unMove();
		if(side)
		{
			if(value<best)
			{
				best=value;
				if(depth==maxdepth)
					bestmove1=mv;
			}
		}
		else
		{
			if(value>best)
			{
				best=value;
				if(depth==maxdepth)
					bestmove1=mv;
			}
		}
	}
}



					
                      
int main()
{
	unsigned long start;
	unsigned long end;
		start=GetTickCount();
		minmaxsearch(6);
		end=GetTickCount();
		unsigned long length=end-start;
		cout<<length<<endl;
	

	return 0;
}
