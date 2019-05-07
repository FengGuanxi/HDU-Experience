#include<iostream> 
#include<cstdio>
#include<cstring> 
#include<alloc.h>
using namespace std;

typedef struct node {
	int posx,posy;
	int dir; // dir为下一步的方向：  1:向东走 ；2：向南走 ；3：向西走；4：向北走 
	struct node *pre;//以为栈是先进后出，当top删除后 我们需要知道top前面的那个节点，所以节点的指针域用来保存前驱地址更好 
}SNode;

typedef struct{
	SNode *base;
	SNode *top;
}LinkStack,*PLinkStack;

//int maze[101][101];
int hash[101][101]; //记录足迹 
int dir[4][2] = {0,1,1,0,0,-1,-1,0};
 
void InitMaze(int n,int m,int maze[100][100]){
	printf("请初始化迷宫：\n");
	int i,j;
	for(i=0;i<n;i++){
		for(j=0;j<m;j++){
			scanf("%d",&maze[i][j]);
		}
	}
	printf("入口位置：%d %d\n",1,1);
	printf("出口位置：%d %d\n",n,m);
//	printf("\n\n");
	memset(hash,0,sizeof(hash));
}

void InitStack(PLinkStack &S){
	S->base = S->top = NULL;
}

bool Pass(int x,int y,int n,int m,int maze[100][100]){
	
	if(x<0||x>n||y<0||y>m)return false;
	//cout<<"Pass: "<<x+1<<' '<<y+1<<' '<<maze[x][y]<<endl; 
	if(maze[x][y]==1||hash[x][y]!=0)return false;
	return true;
}

void Push(PLinkStack &S,SNode &e){
	SNode *p = (SNode *)malloc(sizeof(SNode));
	*p = e;//简便写法 
	/*p->posx = e.posx;
	p->posy = e.posy;
	p->dir = e.dir;*/
	if(S->base==NULL&&S->top==NULL){//当链表为空时,推进第一个节点 此时把S->base ,S->top都指向该节点 
		p->pre = NULL;
		S->base = p;
		S->top = p;
	}
	else{
		p->pre = S->top;//不为空时把新节点的pre指向top ,再修改当前的top为指向p; 
		S->top = p;
	}
}

bool StackEmpty(PLinkStack &S){
	if(S->base==NULL && S->top == NULL)
		return true;
	return false;
}

void Pop(PLinkStack &S,SNode &e){
	/*e.posx = S->top->posx;
	e.posy = S->top->posy;
	e.dir = S->top->dir;
	e.pre = S->top->pre;*/
	e = *S->top;
	if(S->base == S->top){//当base == top 时说明栈内只有一个节点 此时删除节点后把base,top 赋值为NULL 以说明它是空栈 
		free(S->top);
		S->base = S->top = NULL;
	}
	else {
		SNode *p = S->top;
		S->top = p->pre;
		free(p);
	}
}

bool MathPath(int sx,int sy,int ex,int ey,PLinkStack &S,int maze[100][100]){
	InitStack(S);
	int cntposx = sx,cntposy = sy;
	int cntstep = 1;
	do{
		if(Pass(cntposx,cntposy,ex,ey,maze)){ //该节点可以走 就入栈 ，并往东走到下一个节点 
			hash[cntposx][cntposy] = 2;//留下足迹
			SNode e;
			e.posx = cntposx;
			e.posy = cntposy;
			e.dir = 1;
			Push(S,e);            
			if(cntposx == ex && cntposy == ey)return true;
			cntposx += dir[0][0];
			cntposy += dir[0][1];
			cntstep++;
	     	//cout<<"Push: "<<S->top->posx+1<<' '<<S->top->posy+1<<' '<<1<<' '<<S->top->ord<<endl;getchar();
		}
		else { //该节点走不通，删除走向当前节点的节点，修改删除节点的方向，将修改过的节点入栈 ，然后根据新入栈的节点走向下一个节点 
			if(!StackEmpty(S)){
				SNode e;
				Pop(S,e);
		    	//if(!StackEmpty(S))cout<<"pop: "<<S->top->posx<<' '<<S->top->posy<<' '<<S->top->dir<<endl;
				while(e.dir == 4 && !StackEmpty(S)){ //如果4个方向都已走过 则删除该节点； 
					hash[e.posx][e.posy] = 3;//留下曾经走过但不能通过的足迹 
					Pop(S,e);
				    //cout<<"pop77: "<<e.posx+1<<' '<<e.posy+1<<' '<<e.dir<<endl;//	getchar();
				}
				if(e.dir < 4){
					e.dir++;//改变方向 
					Push(S,e); 
                    //cout<<"Push77: "<<e.posx+1<<' '<<e.posy+1<<' '<<e.dir<<endl; 
					cntposx = e.posx+dir[e.dir-1][0];
	    			cntposy = e.posy+dir[e.dir-1][1];;
	    		    //cout<<"cntpos77: "<<cntposx+1<<' '<<cntposy+1<<' '<<e.dir<<' '<<cntstep<<endl;getchar();
				}
			}
		}
	}while(!StackEmpty(S));
	return false;
}
			
void PrintPath(bool succ,PLinkStack S){
	if(!succ){
		printf("该迷宫找不到一条从入口到出口的路.\n\n\n");
	}
	else {
		SNode *e[1000],*p = S->top;
		int i = 0,k=0;
		while(p){
			e[i++] = p;
			p = p->pre;  
		}
		printf("该迷宫一条从入口到出口的路径描述如下:\n"); 
		for(int j=i-1;j>=0;j--){
			printf("第 %03d 步 (posx,posy,dir) ： ( %d,%d,%d )\n",++k,e[j]->posx+1,e[j]->posy+1,e[j]->dir);	
		}
	//	printf("\n\n");
	}
}

void PrintMaze(bool succ,PLinkStack &S,int n,int m,int maze[100][100]){
	int i,j;
	if(succ){
		printf("找该条道路的足迹如下:\n");
		for(i=0;i<n;i++){
			for(j=0;j<m;j++){          
				if(maze[i][j]==1) printf("# ");
				else if(hash[i][j]==0) printf("  ");
				else if(hash[i][j]==2) printf("* ");
				else if(hash[i][j]==3) printf("@ ");
			}
			printf("\n");
		}
		printf("\n\n");
	}
}
		
int main(){
	int n,m;
	bool succ;
	PLinkStack S = (PLinkStack)malloc(sizeof(LinkStack));
	int maze[100][100];
	while(1){
		printf("请输入迷宫的长和宽：\n"); 
		scanf("%d %d",&n,&m);
		//printf("\n\n");
		InitMaze(n,m,maze);
	    succ = MathPath(0,0,n-1,m-1,S,maze);
	    PrintPath(succ,S);
	    PrintMaze(succ,S,n,m,maze);
	}
}

 /*
9 8
0 0 1 0 0 0 1 0
0 0 1 0 0 0 1 0
0 0 0 0 1 1 0 1
0 1 1 1 0 0 1 0 
0 0 0 1 0 0 0 0
0 1 0 0 0 1 0 1
0 1 1 1 1 0 0 1
1 1 0 0 0 1 0 1
1 1 0 0 0 0 0 0

3 2 
0 0
0 0
0 0

3 4
0 0 0 0 
0 0 1 1
0 0 0 0

4 9
0 0 0 0 0 0 1 0 0
0 1 0 0 0 1 0 0 0
0 0 1 1 1 0 0 1 1
0 0 1 1 1 0 1 0 0

 
 */
	
	
	 
