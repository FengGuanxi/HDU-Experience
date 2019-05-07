#include<iostream>
#include<cstdio>
#include<cstring>
using namespace std;

#define MAX 1023717496

typedef struct ArcCell{
	int adj;//两点间的距离 
}ArcCell,AdjMatrix[100][100];

typedef struct {
	char scename[30];//风景名称 
	int scenum;//风景代号 
	char sceintro[200];//风景简介 
}Scenery;

typedef struct {
	Scenery vexs[100];
	AdjMatrix arcs;
	int vexnum,arcnum;
}MGraph;//图的数组表示法 

int Path[100];//记录最短路径 
int D[100];//记录节点到起始节点的距离 
bool final[100];//标记该节点是否在路径上 
                                           
int GetSce(char scename[],MGraph G){//由风景名得到风景在数组中的下标 
	int i;
	for(i=1;i<=G.vexnum;i++)
		if(strcmp(scename,G.vexs[i].scename)==0)return i;
	return -1;
}

void CreatMGraph(MGraph &G){
	int i,j;
	int dis,n1,n2;
	char name1[30],name2[30];
	printf("请输入景点个数: ");
	scanf("%d",&G.vexnum);
	printf("请输入地图上路的条数: ");
	scanf("%d",&G.arcnum);
	printf("\n请输入每个景点的信息（风景名称，风景代号，风景简介). \n");
	for(i=1;i<=G.vexnum;i++){
		printf("第 %d 个景点（风景名称，风景代号，风景简介: ",i);
		scanf("%s",G.vexs[i].scename);
		scanf("%d",&G.vexs[i].scenum);
		scanf("%s",G.vexs[i].sceintro);
	}
	printf("\n请输入景点之间的路且路的长度: \n");
	for(i=1;i<=G.vexnum;i++){
		for(j=1;j<=G.vexnum;j++){
			G.arcs[i][j].adj = MAX;
		}
	}
	
	for(i=1;i<=G.arcnum;i++){
		printf("请输入起始某条路的起始和终止景点的名称，及该条路的长度: ");
		scanf("%s",name1);
		scanf("%s",name2);
		n1 = GetSce(name1,G);
		n2 = GetSce(name2,G);
		scanf("%d",&dis);
		G.arcs[n1][n2].adj = dis;
		G.arcs[n2][n1].adj = dis;
	}
	printf("\n");
}


void OutPutPath(MGraph G,bool find,int sn,int en){
	int i;
	int path[100];
	if(!find){
		printf("不能找到从景点 %s 到景点 %s 的路.\n\n",G.vexs[sn].scename,G.vexs[en].scename);
		return;
	}
	else {
        
		printf("\n景点 %s 到景点 %s 的最短路径的长度为: %d\n\n",G.vexs[sn].scename,G.vexs[en].scename,D[en]);
		printf("景点 %s 到景点 %s 最短路径中的风景依次为: \n",G.vexs[sn].scename,G.vexs[en].scename);
	    //for(i=1;i<=G.vexnum;i++)cout<<Path[i]<<' ';cout<<endl;
		path[0] = en;
		int k = 1,x = Path[en];
		while(x != sn){
			path[k++] = x;
			x = Path[x];
		}
        path[k] = sn;
		for(i=k;i>=0;i--){
            printf("该最短路径上第 %d 个景点为(名称，代号，简介)：%s %d %s \n",k-i+1,G.vexs[path[i]].scename,G.vexs[path[i]].scenum,G.vexs[path[i]].sceintro);
		}
		printf("\n");
	}
}


void ShortestPath_DIJ(MGraph G,int sn,int en){
	int i;
	int v,min;
	for(i=1;i<=G.vexnum;i++){
		final[i] = false;//用于记录是否在已确定的点的集合中 
		D[i] = G.arcs[sn][i].adj;//每个点与起点的距离 
		if(D[i]!= MAX)Path[i] = sn;//初始化Path[] 与起点相连的则标记为起点 否则标记为0 
		else Path[i] = 0;
	}
	D[sn] = 0;
	final[sn] = true;
	Path[sn] = sn;
	bool find = false;
	while(1){
		min = MAX;
		v = -1;
		for(i=1;i<=G.vexnum;i++){//找到当前位置与起点最近的点 
			if(final[i]==false&&D[i]<min){
				v = i;
				min = D[i];
			}
		}
		if(v==-1){
			find = false;
			break;
		}
	//	cout<<"v: "<<v<<" MIN: "<<min<<endl;
		final[v] = true;
		for(i=1;i<=G.vexnum;i++){//修改每个点到起点的最近距离 ，若修改了 则说明通过v点修改点离起点更近了 此时记录Path[i] = v; 
			if(final[i]==false&&(min+G.arcs[v][i].adj<D[i])){
				D[i] = min+G.arcs[v][i].adj;
				Path[i] = v;
			}
		} 
	//	for(i=1;i<=G.vexnum;i++)cout<<D[i]<<' ';cout<<endl;
		if(v==en){
			find = true;
			break;
		}
	}
	OutPutPath(G,find,sn,en);
}




int main(){
    while(1){
    	MGraph G;
    	char name1[30],name2[30],name[30],num;
    	int sn,en,cho;
    	CreatMGraph(G);
    	while(1){
			printf("要查询景点，请输入 1，要查询两个景点间的距离，请输入 2.\n");
			scanf("%d",&cho);
			if(cho==2){
        		printf("请输入起始景点名称:");
				scanf("%s",name1);
				printf("请输入终止景点名称:");
        		scanf("%s",name2);
        		sn = GetSce(name1,G);
        		en = GetSce(name2,G);
        		if(sn==en)
					 printf("起始景点即为终止景点.\n\n");
				else 
        			 ShortestPath_DIJ(G,sn,en);
			}
			else{
				printf("请输入要查找的景点的名称: ");
				scanf("%s",name);
				num = GetSce(name, G);
				
				printf("景点%s的风景代号为%d:\n",name,G.vexs[num].scenum);
				cout<<"景点"<<name<<"的风景简介为"<<G.vexs[num].sceintro<<endl;
			}
        	printf("停止查询，请输入 1 .否则，输入 0 .\n\n");
        	int q;
			scanf("%d",&q);
			if(q)break;
         }
     }
	return 0;
}

/*
6 
8
v1 1 11
v2 2 22
v3 3 33
v4 4 44
v5 5 55
v6 6 66
v1 v3 10
v1 v5 30
v1 v6 100
v2 v3 5
v3 v4 50
v4 v6 10
v4 v5 20
v5 v6 60
*/






	



	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
