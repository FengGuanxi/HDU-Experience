#include <iostream>
#include <cstdlib>
#include <fstream>
using namespace std;

typedef struct vertex //建立顶点的结构体
{
	int r;
	int p;
	char num;
}vertex;

typedef struct edge //建立边的结构体
{
	char a;
	char b;
	int flag;
	int dis;
}edge;

int cmp( const void *a ,const void *b)    //快速排序
{ 
	return (*(edge *)a). dis> (*(edge *)b).dis ? 1 : -1; 
} 

void initial(edge *e,vertex *v,int n,int m)  //初始化
{
	int i;
	printf("请分别输入顶点序号: ");
	for(i=0;i<n;i++)
	{
		getchar();
		scanf("%c",&v[i].num);
		v[i].p=v[i].r=i;
	}
	printf("请输入顶点A,顶点B以及两点之间的距离: \n");
	for(i=0;i<m;i++)
	{
		getchar();
		scanf("%c %c %d",&e[i].a,&e[i].b,&e[i].dis);
		e[i].flag=0;
	}
}

int find(vertex *v,vertex *u) //查找顶点u的根节点
{
	if(u->r==u->p)
		return u->p;
	else
	{
		return find(v,&v[u->p]);
	}
}

void merge(vertex *v,int aa,int bb)   //将序号为aa和bb的顶点并入同一集合
{
	v[aa].p=bb;
}

int main(void) 
{
	int i,j,n,m;
	int aa,bb,sum;
	vertex *v;
	edge *e;
	printf("请输入点数和边数:");
	scanf("%d %d",&n,&m);
	e=(edge *)malloc(m*sizeof(edge));
	v=(vertex *)malloc(n*sizeof(vertex));

	initial(e,v,n,m);  
    qsort(e,m,sizeof(e[0]),cmp);  
	sum=0;                   //计数器
	for(i=0;i<m;i++) 
	{
		for(j=0;j<n;j++)     //查找边ab的两个顶点的根节点,a的根节点为aa,b的根节点为bb
		{
			if(v[j].num==e[i].a)
				aa=find(v,&v[j]);
			if(v[j].num==e[i].b)
				bb=find(v,&v[j]);
		}
		if(aa!=bb)          //若aa不等于bb,则将aa,bb合并入同一个集合
		{
			merge(v,aa,bb);
			e[i].flag=1;    //标记该条边为已读
			sum++;
			if(sum==n-1)
				break;
		}
	}
	printf("最小生成树为\n");
	for(i=0;i<m;i++)
	{
		if(e[i].flag==1)
		{
			printf("%c <--> %c:%d\n",e[i].a,e[i].b,e[i].dis);
		}
	}
	ofstream f;           //文件操作，使最小生成树的各条边以及相应的权值以文本形式输出
	f.open("MiniSpanTree.txt");
	for(i=0;i<m;i++)
	{
		if(e[i].flag==1)
		{
		    f<<e[i].a<<"<-->"<<e[i].b<<":"<<e[i].dis<<endl;
		}
	}
	f.close();
	return 0;
}
