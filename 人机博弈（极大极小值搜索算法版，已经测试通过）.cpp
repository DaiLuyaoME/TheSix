
#include<iostream>
#include<stack>
#include<vector>
#include<Windows.h>
using namespace std;
//Point�࣬��¼������
struct Point{
			//���ݳ�Ա���ֱ��ʾ�к���
             int row;
             int col;
			 //Point�Ĺ��캯��
             Point(int ROW=1, int COL=1):row(ROW), col(COL){}
             };
 //Move�࣬��¼ÿһ�����ƶ����         
struct Move{
       int killingnumber;
       Point from;
       Point to;
       //���캯����killingnumberĬ��ֵΪ0      
	   Move(Point& P1, Point& P2,int a=0):from(P1), to(P2),killingnumber(a){}
	   Move(){};
             };

stack<Point>piecekilled;//����ջ���洢�����ӵ����� 
stack<Move>Movestack;//����ջ���洢���ӵ��ƶ����

         
     


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
    int K=0;//��¼���ӵĸ���
    int R=mk.to.row,C=mk.to.col;
    for(int i=0;i!=4;++i)
	{
		//ȡ�ñ���������
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
                            K+=1;//���س��ӵĸ���
				}
			}
		}
	}
	 //���س�����
    return K;
}         

//�������Ӻ�����ʵ��
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

//�߷�����������ʵ��
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
//���庯����ʵ��
bool Movepiece(Move& newmove){
     bool flagyourpiece=0,flagspace=0;
    //�ж��Ƿ��Ǽ���������  
     if(Board[newmove.from.row][newmove.from.col]==side){
                                           flagyourpiece=1;}
     else{
          cout<<" This is NOT your piece"<<endl;
          }
	 //�ж�Ŀ�ĵ��Ƿ��ǿ�λ
     if(Board[newmove.to.row][newmove.to.col]==2){
                                                       flagspace=1;
                                                       }
     else{
          cout<<"Your step is fault"<<endl;
          }
     //�ƶ�����      
     if(flagyourpiece&&flagspace){
								//����
                                  Board[newmove.from.row][newmove.from.col]=2;
                                  Board[newmove.to.row][newmove.to.col]=side;
							    //����,�����ӵĸ�������newmove�����ݳ�Ա��
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

	//��������
    for(int i=1;i<7;i++)
	{
            for(int j=1;j<6;j++)
			{
                    if(Board[i][j]==side)
					{
						//�����ĸ�����
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

  

//��������������ʵ��
//��������ÿ������ֵ15��һ��������Χ���м��������򼺷���4
//���ǶԷ���������Է���4������ǿ�λ�򼺷���2
//����������������Ҫ�������ڵĴ����������޸Ĳ��ܵ�������
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
					player+=15;//ÿ����ֵΪ15�� 
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
	//������ң�side=0����ȥ���ԣ�side=1)��ֵ,ֵԽ������Խ��������֮�Ե���Խ����
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
