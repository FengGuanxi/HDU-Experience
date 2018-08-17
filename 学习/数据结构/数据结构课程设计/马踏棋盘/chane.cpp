#include<iostream>
#include<malloc.h>
#include<math.h>
#define STACK_INIT_SIZE 10 // 存储空间初始分配量
#define STACKINCREMENT 2 // 存储空间分配增量
#define MAXLENGTH 8 // 设棋盘为8x8矩阵
typedef int BoardType[MAXLENGTH][MAXLENGTH]; // 棋盘数组[行][列]
BoardType b; // 棋盘数组
int HTry1[8]={-2,-1,1,2,2,1,-1,-2};
int HTry2[8]={1,2,2,1,-1,-2,-2,-1};
typedef struct
{
	int i;
	int j;
	int dir;
}PosType;//棋盘坐标位置类型
typedef struct 
{
   PosType *base;
   PosType *top; 
   int stacksize; 
}SqStack; // 顺序栈

int InitStack(SqStack &S)
 { // 构造一个空栈S
   if(!(S.base=(PosType*)malloc(STACK_INIT_SIZE*sizeof(PosType))))
     exit(OVERFLOW); 
   S.top=S.base;
   S.stacksize=STACK_INIT_SIZE;
   return 1;
 }
int StackEmpty(SqStack S)
 { // 若栈S为空栈，则返回1，否则返回0
   if(S.top==S.base)
     return 1;
   else
     return 0;
}
int Push(SqStack &S,PosType e)
 { // 插入元素e为新的栈顶元素
   if(S.top-S.base>=S.stacksize)
   {
     S.base=(PosType*)realloc(S.base,(S.stacksize+STACKINCREMENT)*sizeof(PosType));
     if(!S.base)
       exit(OVERFLOW); 
     S.top=S.base+S.stacksize;
     S.stacksize+=STACKINCREMENT;
   }
   *(S.top)++=e;
   return 1;
 }

int Pop(SqStack &S,PosType &e)
 { // 若栈不空，则删除S的栈顶元素，用e返回其值，并返回1；否则返回0
   if(S.top==S.base)
     return 0;
   e=*--S.top;
   return 1;
 }

int Pass(PosType p)
{
	if(p.i>=0&&p.i<=7&&p.j>=0&&p.j<=7&&b[p.i][p.j]==0)//p(i,j)要在b[8][8]数组范围内，并且该位置不在当前路径中
	return 1;
	else return 0;
}
int PassCount(PosType p)//可走的位置的个数
{
	int count=0,k;
	for(k=0;k<8;k++)
	{
		p.i+=HTry1[k];
		p.j+=HTry2[k];
		if(Pass(p))
			count++;
		p.i-=HTry1[k];
		p.j-=HTry2[k];
	}
		return count;
}
PosType NextPos(PosType pos)
{//找可以走的位置最少的下一个点作为最优的下一步策略
	int Max=8;
	int count=0;
	int k,x,y,dir;
	PosType p;
	p.i=pos.i;
	p.j=pos.j;
	for(k=0;k<8;k++)
	{
		p.i+=HTry1[k];
		p.j+=HTry2[k];
		if(Pass(p))
		{	
			count=PassCount(p);
			if(count<Max)
			{
				Max=count;
				x=p.i;
				y=p.j;
			   	dir=k;//dir记录当前最有策略的方向
			}
		}
		p.i-=HTry1[k];
		p.j-=HTry2[k];
		p.dir=dir;
	}
	p.i=x;
	p.j=y;
	return p;
}
void ChessBoardPath(BoardType b,PosType start)
{//求解走遍棋盘的路径，找到路径返回1，否则返回0
	SqStack S;
	PosType curpos,e,e1;//当前位置
	int m,n,num=0;
    for(m=0;m<8;m++)
		for(n=0;n<8;n++)
			b[m][n]=0;//Broad数组初始化0
	InitStack(S);
	curpos=start;
	do
	{
		if(Pass(curpos))//当前位置可走
		{
			e1.i=curpos.i;
			e1.j=curpos.j;
			e1.dir=0;
			num++;
			b[e1.i][e1.j]=num;	
			Push(S,e1);
			if(num==64) break;//走完全盘，结束
			else 
				curpos=NextPos(curpos);//探索下一个位置可能的位置个数最少的方向，并使它成为当前位置
		}		
     else
     { // 当前位置不能走
       if(!StackEmpty(S))
       {
		   int n;
           Pop(S,e); // 退栈到前一位置
		   n=PassCount(e)-1;
         while(e.dir==n&&!StackEmpty(S)) // 前一位置处于最后一个方向
         {
			 b[e.i][e.j]=0;
             Pop(S,e); // 退回一步
         }
		 n=PassCount(e)-1;
         if(e.dir<n) // 没到最后一个方向
         {
           e.dir++; // 换下一个方向探索
           Push(S,e);
	       curpos=NextPos(e); 
         }
       }
     }
   }while(!StackEmpty(S));
}
void main()
{
	PosType start;
	int i,j;
	printf("请输入马在棋盘上的初始位置(i,j)的值: ");
	scanf("%d%d",&start.i,&start.j);
	ChessBoardPath(b,start);	
	printf("棋盘路径为:\n");
	for(i=0;i<8;i++)
		for(j=0;j<8;j++)
		{
			printf("%d ",b[i][j]);
			if(j==7)
				printf("\n");
		}
}