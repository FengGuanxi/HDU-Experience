#include <iostream>
#include <cstdlib>
#include<io.h> 
#include <fstream.h>
#define MAX_VERTEX_NUM 30 // 最大顶点个数
struct VertexType
{
	char vex;
	int parent;
};
struct ArcCellType
{
	int adj;// 权值
	int vi,vj;//端点
};
struct MGraph
{
	 VertexType vexs[MAX_VERTEX_NUM]; // 顶点向量
	 ArcCellType arcs[MAX_VERTEX_NUM*(MAX_VERTEX_NUM-1)/2+1];//存储边的数组
     int vexnum,arcnum; // 图的当前顶点数和弧数
};
struct MFSet
{
	VertexType nodes[MAX_VERTEX_NUM];
	int r,n;//根的位置和结点数
};
void InitMfset(MFSet &S,MGraph G)
{//初始化等价类
	int i;
	S.n=G.vexnum;
	for(i=0;i<S.n;i++)
	S.nodes[i].parent=-1;
}
int LocateVex(MGraph G,char u)
{//若G中存在顶点u,则返回该顶点在图中位置;否则返回-1
   int i;
   for(i=0;i<G.vexnum;++i)
     if(u==G.vexs[i].vex)
       return i;
   return -1;
}
void CreateAN(MGraph &G)
{//构造无向网G
	int i,j,k,w;
	char va,vb;  
	FILE *graphlist;
	graphlist=fopen("mincstree.txt","r");
	fscanf(graphlist,"%d",&G.vexnum);
    fscanf(graphlist,"%d",&G.arcnum);
	for(i=0;i<G.vexnum;++i)//构造顶点向量
		fscanf(graphlist,"%s",&G.vexs[i].vex);
    for(k=1;k<=G.arcnum;++k)
	{
		fscanf(graphlist,"%s %s %d",&va,&vb,&w);	
		i=LocateVex(G,va);
		j=LocateVex(G,vb);
		G.arcs[k].adj=w;
		G.arcs[k].vi=i;
		G.arcs[k].vj=j;
	}
}
int find_mfset(MFSet &S,int i)
{//查找i属于所在集合并返回
	int j,k,t;
	if(i<0||i>S.n-1) return -1;//不属于S的任一子集
	for(j=i;S.nodes[j].parent>=0;j=S.nodes[j].parent);
	for(k=i;k!=j;k=t)
	{
		t=S.nodes[k].parent;
		S.nodes[k].parent=j;
	}
	return j;
}
int is_mfset(MFSet &S,int vi,int vj)
{//判断i j是否属于一个集合，是返回1，否则返回0
	if(find_mfset(S,vi)==find_mfset(S,vj))
		return 1;
	else return 0;
}
int min_mfset(MFSet &S,int i,int j)
{//和并i j所在的两个集合
	if(i<0||i>S.n-1||j<0||j>S.n-1) return 0;
	if(S.nodes[i].parent>S.nodes[j].parent)
	{
		S.nodes[j].parent+=S.nodes[i].parent;
		S.nodes[i].parent=j;
	}
	else
	{
		S.nodes[i].parent+=S.nodes[j].parent;
		S.nodes[j].parent=i;
	}
	return 1;
}
int Partition(ArcCellType arcs[],int low,int high)
{//交换L中elem[low..high]的记录，枢轴记录到位，并返回其所在位置，此时在它之前的记录均不大于它
	int pivotkey;
	arcs[0].adj=arcs[low].adj;//用子表的第一个记录作枢轴记录
    arcs[0].vi=arcs[low].vi;
    arcs[0].vj=arcs[low].vj;
    pivotkey=arcs[low].adj;//用权值做关键字
    while(low<high)
	{
		while(low<high&&arcs[high].adj>=pivotkey)
			--high;
		arcs[low].adj=arcs[high].adj;//将比枢轴记录小的记录移到低端
		arcs[low].vi=arcs[high].vi;
		arcs[low].vj=arcs[high].vj;
		while(low<high&&arcs[low].adj<=pivotkey)
			++low;
		arcs[high].adj=arcs[low].adj;//将比枢轴记录大的记录移到高端
		arcs[high].vi=arcs[low].vi;
		arcs[high].vj=arcs[low].vj;
	}
	arcs[low].adj=arcs[0].adj;
	arcs[low].vi=arcs[0].vi;
	arcs[low].vj=arcs[0].vj;
	return low; // 返回枢轴位置
}
void QSort(ArcCellType arcs[],int low,int high)
{//对L中的L.elem[low..high]作快速排序
	int pivotloc;
    if(low<high)
	{//长度大于1
		pivotloc=Partition(arcs,low,high);
		QSort(arcs,low,pivotloc-1);
		QSort(arcs,pivotloc+1,high);
	}
}
void minspantree(MGraph &G,MFSet &S)
{//克鲁斯算法求最小生成树
	int ecnum,i,v,w;
	FILE *output;
	output=fopen("mincstreeOutput.txt","w"); //文件操作，使最小生成树的各条边以及相应的权值以文本形式输出
	QSort(G.arcs,1,G.arcnum);
	i=1;
	ecnum=G.vexnum; //连通分量个数
	while(ecnum>1)
	{
		v=find_mfset(S,G.arcs[i].vi);
		w=find_mfset(S,G.arcs[i].vj);
		if(!is_mfset(S,G.arcs[i].vi,G.arcs[i].vj)) 
		{//两个端点不在一个等价类
			min_mfset(S,v,w); //合并连通分量
		    printf("%c%c %d\n",G.vexs[G.arcs[i].vi].vex,G.vexs[G.arcs[i].vj].vex,G.arcs[i].adj);
			fprintf(output,"%c%c %d\n",G.vexs[G.arcs[i].vi].vex,G.vexs[G.arcs[i].vj].vex,G.arcs[i].adj);
		    ecnum--;
		}
		i++;
	}
}
void main()
{
	MGraph G;
	MFSet S;
	CreateAN(G);
	InitMfset(S,G);
	minspantree(G,S);
}