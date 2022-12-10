#include "NFAToDFA.h"
#include "RegexpToPost.h"
#include "PostToNFA.h"
#include "NFAStateStack.h"
#include "NFAFragmentStack.h"
#include "OutputResult.h"
#include <stdlib.h>

NFAFragmentStack FragmentStack; // 栈。用于储存 NFA 片段
NFAStateStack StateStack;		// 栈。用于储存 NFA 状态

const char VoidTrans = '$'; // 表示空转换

char regexp[256];

int main(int argc, char *argv[])
{
	char *post;
	DFA* dfa = (DFA*)malloc(sizeof(DFA));
	dfa->length = 0;
	
	//
	// 初始化栈
	//
	InitNFAFragmentStack(&FragmentStack);
	

	// 在input1.txt ～ input3.txt文件中包含了不同的正则表达式案例。
	// 程序开始运行时会通过命令行参数（例如 app.exe < input1.txt）将标准输入（stdin）重定向到一个input文件。
	// 所以这里调用scanf函数就可以读取到input文件中的正则表达式了。
	scanf("%255s", regexp); 	 
	
	//
	// 调用 re2post 函数将正则表达式字符串转换成解析树的后序遍历序列
	//
	post = re2post(regexp);
	
	//
	// 调用 post2dfa 函数将解析树的后序遍历序列转换为 DFA 
	//
	dfa = post2dfa(dfa, post);

	//
	// 将 DFA 打印输出
	//
	OutputResult(dfa);
				
	return 0;
}

/*
功能：
	创建一个 DFA 状态的转换。
	
参数：
	TransformChar -- 转换符号。
	NFAStateArray -- NFA 状态指针数组。
	Count -- 数组元素个数。	
	  
返回值：
	 Transform 结构体指针。
*/
Transform* CreateDFATransform(char TransformChar, NFAState** NFAStateArray, int Count)
{
	int i;
	Transform* pTransform = (Transform*)malloc(sizeof(Transform));
	
	for (i=0; i<Count; i++)
	{
		pTransform->NFAlist[i] = NFAStateArray[i];
	}
	
	pTransform->NFAStateCount = Count;	
	pTransform->TransformChar = TransformChar;
	pTransform->DFAStateIndex = -1;
	pTransform->NextTrans = NULL;
	
	return pTransform;
}

/*
功能：
	创建一个 DFA 状态。
	
参数：
	pTransform -- DFA 状态转换指针。	
	  
返回值：
	 DFAState 结构体指针。
*/
DFAState* CreateDFAState(Transform* pTransform)
{
	int i;
	DFAState* pDFAState = (DFAState*)malloc(sizeof(DFAState));
	
	for (i=0; i<pTransform->NFAStateCount; i++)
	{
		pDFAState->NFAlist[i] = pTransform->NFAlist[i];
	}

	pDFAState->NFAStateCount = pTransform->NFAStateCount;
	pDFAState->firstTran = NULL;

	return pDFAState;
}

/*
功能：
	判断一个转换中的 NFA 状态集合是否为某一个 DFA 状态中 NFA 状态集合的子集。
	
参数：
	pDFA -- DFA 指针。
	pTransform -- DFA 状态转换指针。	
	  
返回值：
	 如果存在返回 DFA 状态下标，不存在返回 -1。
*/
int NFAStateIsSubset(DFA* pDFA, Transform* pTransform)
{
	//
	// TODO: 在此添加代码
	//
	for(int i=0;i<pDFA->length;i++){
		DFAState* DFAState_tmp = pDFA->DFAlist[i];
		//DFA状态的NFA个数小于Transform的NFA个数 自然不是子集合
		if(DFAState_tmp->NFAStateCount<pTransform->NFAStateCount)
			continue;
		
		int All_find_flag=1; //假设全找到了 在下面的寻找过程中 如果有nfaID没有被找到 那么就让allfindflg=0
		for(int nfaID=0;nfaID<pTransform->NFAStateCount;nfaID++){
			int nfaID_flag=0; //表示nfaID这个nfa找到没有
			for(int j=0;j<DFAState_tmp->NFAStateCount;j++){
				if(pTransform->NFAlist[nfaID]->Name == DFAState_tmp->NFAlist[j]->Name){
					nfaID_flag=1;
					break;
				}
			}
			//如果有nfaID这个没有被找到 那不用找下面的nfaID 可以直接换一个DFALIST[i]
			if(nfaID_flag==0){
				All_find_flag=0;
				break;
			}
		}
		//如果全找到了 就不用在找了 
		if(All_find_flag){
			return i;
		}
	}
	//没找到
	return -1;
			
}

/*
功能：
	判断某个 DFA 状态的转换链表中是否已经存在一个字符的转换。
	
参数：
	pDFAState -- DFAState 指针。
	TransformChar -- 转换标识符。
	  
返回值：
	 Transform 结构体指针。
*/
Transform* IsTransformExist(DFAState* pDFAState, char TransformChar)
{
	//
	// TODO: 在此添加代码
	//
	/**我自己写的 可能出错**/
	Transform* pTFCursor;
	for(pTFCursor=pDFAState->firstTran; pTFCursor!=NULL;pTFCursor = pTFCursor->NextTrans ){
		if(pTFCursor->TransformChar == TransformChar)
			return pTFCursor;
	}
	return NULL;
	
}

/*
功能：
	将一个 NFA 集合合并到一个 DFA 转换中的 NFA 集合中。
	注意，合并后的 NFA 集合中不应有重复的 NFA 状态。
	
参数：
	NFAStateArray -- NFA 状态指针数组，即待加入的 NFA 集合。 （有一个输入符号转换指向到状态的空闭包全部的NFA状态）
	Count -- 待加入的 NFA 集合中元素个数。
	pTransform -- 转换指针。(相应输入符号char TransformChar的转换链表的元素)
*/
void AddNFAStateArrayToTransform(NFAState** NFAStateArray, int Count, Transform* pTransform)
{
	//
	// TODO: 在此添加代码
	//
	//Transform NFAState* NFAlist[MAX_STATE_NUM] 需要添加新的NFA
	//考虑重复
	////printf("原来的pTransform[%c]长度%d\n",pTransform->TransformChar,pTransform->NFAStateCount);
	for(int i=0;i<Count;i++){
		int have_flag = 0;
		for(int j = 0;j<pTransform->NFAStateCount;j++){
			if(pTransform->NFAlist[j]->Name == NFAStateArray[i]->Name){
				have_flag=1;
				break;
			}
		}
		if(!have_flag){
			pTransform->NFAlist[(pTransform->NFAStateCount)++]=NFAStateArray[i];
		}
	}
	////printf("add之后的长度%d\n",pTransform->NFAStateCount);

}

/*
功能：
	使用二叉树的先序遍历算法求一个 NFA 状态的ε-闭包。
	
参数：
	State -- NFA 状态指针。从此 NFA 状态开始求ε-闭包。
	StateArray -- NFA 状态指针数组。用于返回ε-闭包。
	Count -- 元素个数。	用于返回ε-闭包中 NFA 状态的个数。
*/
void Closure(NFAState* State, NFAState** StateArray, int* Count)
{
	InitNFAStateStack(&StateStack); // 调用 InitNFAStateStack 函数初始化栈
	
	//
	// TODO: 在此添加代码
	//
	//创建visit数组表示是否遍历 
	int visit[MAX_STATE_NUM]={0};
	(*Count)=0;
	//开始状态入栈
	PushNFAState(&StateStack,State);
	visit[State->Name]=1;
	
	//开始BFS
	while(!NFAStateStackEmpty(&StateStack)){
		//栈顶NFA状态出栈
		NFAState* nowState = PopNFAState(&StateStack);
		//放入闭包数组
		StateArray[(*Count)++] = nowState;
		//判断是否有空转换
		if(nowState->Transform==VoidTrans){
			//有两条连接
			if(nowState->Next1!=NULL&&!visit[nowState->Next1->Name] && nowState->Next2!=NULL && !visit[nowState->Next2->Name]){
				//大的先入栈，小的就可以先出栈
				int flag = nowState->Next1->Name > nowState->Next2->Name ? 1 : 0;
				if(flag){
					PushNFAState(&StateStack,nowState->Next1);
					PushNFAState(&StateStack,nowState->Next2);
				}else{
					PushNFAState(&StateStack,nowState->Next2);
					PushNFAState(&StateStack,nowState->Next1);
				}
				//设置已访问标记
				visit[nowState->Next1->Name]=1;
				visit[nowState->Next2->Name]=1;
			}
			else if(nowState->Next1!=NULL &&!visit[nowState->Next1->Name]){
				PushNFAState(&StateStack,nowState->Next1);	
				visit[nowState->Next1->Name]=1;
			}
			else if(nowState->Next2!=NULL &&!visit[nowState->Next2->Name]){
				PushNFAState(&StateStack,nowState->Next2);	
				visit[nowState->Next2->Name]=1;
			}
		}

	}

}

/*
功能：
	将解析树的后序遍历序列转换为 DFA。

参数：
	pDFA -- DFA 指针。
	postfix -- 正则表达式的解析树后序遍历序列。
	  
返回值：
	DFA 指针。
*/
NFAState* Start = NULL;
DFA* post2dfa(DFA* pDFA, char *postfix)
{
	int i, j;								// 游标
	Transform* pTFCursor;  					// 转换指针
	NFAState* NFAStateArray[MAX_STATE_NUM]; // NFA 状态指针数组。用于保存ε-闭包
	int Count = 0;							// ε-闭包中元素个数
	
	//
	// 调用 post2nfa 函数将解析树的后序遍历序列转换为 NFA 并返回开始状态
	//
	////printf("准备转nfa\n");
	Start = post2nfa(postfix);
	////printf("post2nfa成功\n");
	
	//
	// 调用 Closure 函数构造开始状态的ε-闭包
	//
	Closure(Start, NFAStateArray, &Count);
	////printf("Closure成功");

	// 调用 CreateDFATransform 函数创建一个转换(忽略转换字符)，
	// 然后，调用 CreateDFAState 函数，利用刚刚创建的转换新建一个 DFA 状态
	Transform* pTransform = CreateDFATransform('\0', NFAStateArray, Count);
	DFAState* pDFAState = CreateDFAState(pTransform);

	// 将 DFA 状态加入到 DFA 状态线性表中
	pDFA->DFAlist[pDFA->length++] = pDFAState;

	// 遍历线性表中所有 DFA 状态
	for(i=0; i<pDFA->length; i++)
	{
		////printf("第%d个DFA\n",i);
		// 遍历第 i 个 DFA 状态中的所有 NFA 状态
		for(j=0; j<pDFA->DFAlist[i]->NFAStateCount; j++)
		{
			////printf("第%d个DFA的%d\n",i,pDFA->DFAlist[i]->NFAlist[j]->Name);
			NFAState* NFAStateTemp = pDFA->DFAlist[i]->NFAlist[j];

			// 如果 NFA 状态是接受状态或者转换是空转换，就跳过此 NFA 状态 如果为转换为空转换 那么什么输入符号都没用	
			if(NFAStateTemp->Transform == VoidTrans || NFAStateTemp->AcceptFlag == 1)
				continue;

			// 调用 Closure 函数构造 NFA 状态的ε-闭包
			Closure(NFAStateTemp->Next1, NFAStateArray, &Count);

			// 调用 IsTransfromExist 函数判断当前 DFA 状态的转换链表中是否已经存在该 NFA 状态的转换
			pTransform = IsTransformExist(pDFA->DFAlist[i], NFAStateTemp->Transform);
			////printf("%d\n",pTransform==NULL);
			if(pTransform == NULL)
			{
				// 不存在，调用 CreateDFATransform 函数创建一个转换，并将这个转换插入到转换链表的开始位置
				
				//
				// TODO: 在此添加代码
				//
				//创建一个转换
				////printf("不存在这个转换，新建转换并放在开头\n");
				Transform* pTransform_tmp = CreateDFATransform(NFAStateTemp->Transform , NFAStateArray,Count);
				//插入到转换链表到开始位置
				pTransform_tmp->NextTrans = pDFA->DFAlist[i]->firstTran;
				pDFA->DFAlist[i]->firstTran = pTransform_tmp;

			}
			else
			{
				//存在，调用 AddNFAStateArrayToTransform 函数把ε-闭包合并到已存在的转换中
				
				//
				// TODO: 在此添加代码
				//
				//在转换链表里面好到转换字符与之相同的Transform
				////printf("已经存在这个转换，添加进去\n");
				for(Transform* pTFCursor_tmp =pDFA->DFAlist[i]->firstTran ; pTFCursor_tmp!=NULL;pTFCursor_tmp=pTFCursor_tmp->NextTrans){
					if(pTFCursor_tmp->TransformChar == NFAStateTemp->Transform){
						AddNFAStateArrayToTransform(NFAStateArray,Count,pTFCursor_tmp);
					}
				}	
			}
		}

		// 遍历 DFA 状态的转换链表，根据每个转换创建对应的 DFA 状态
		for(pTFCursor = pDFA->DFAlist[i]->firstTran; pTFCursor != NULL; pTFCursor = pTFCursor->NextTrans)
		{
			// 调用 NFAStateIsSubset 函数判断转换中的 NFA 状态集合是否为某一个 DFA 状态中 NFA 状态集合的子集
			int Index = NFAStateIsSubset(pDFA, pTFCursor);
			////printf("是不是Dataset里面的子集%d\n",Index);
			if(Index == -1)
			{
				// 不是子集，调用 CreateDFAState 函数创建一个新的 DFA 状态并加入 DFA 线性表中
				// 将转换的 DFAStateIndex 赋值为新加入的 DFA 状态的下标
				
				//
				// TODO: 在此添加代码
				//
				int pre_len = pDFA->length;
				pTFCursor->DFAStateIndex=pre_len;
				DFAState* pDFAState_new = CreateDFAState(pTFCursor);
				pDFA->DFAlist[pre_len] = pDFAState_new;
				pDFA->length++;
				
			}
			else
			{
				// 是子集，将转换的 DFAStateIndex 赋值为 Index
				
				//
				// TODO: 在此添加代码
				//
				pTFCursor->DFAStateIndex=Index;
				
			}
		}
		//printf("第%d次循环结束\n",i);
		if(i>12)
			break;
	}

	return pDFA;
}
