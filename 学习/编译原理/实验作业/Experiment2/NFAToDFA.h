#ifndef NFATODFA_H_
#define NFATODFA_H_

//
// 在此处包含 C 标准库头文件
//

#include <stdio.h>

//
// 在此处包含其他头文件
//

//
// 在此处定义数据结构
//

#define MAX_STATE_NUM 64	// 状态的最大数量。

typedef struct _NFAState
{
	char Transform;       	 // 状态间装换的标识，用 '$' 表示 'ε-转换'。
	struct _NFAState *Next1; // 指向下一个状态。
	struct _NFAState *Next2; // 指向下一个状态。
	int Name;			     // 状态名称。
	int AcceptFlag;		     // 是否为接受状态的标志，1 表示是接受状态 0 表示非接受状态。 
}NFAState;

// Fragment 结构是一个 NFA 的片段
typedef struct _NFAFragment
{
	NFAState *StartState; 	// 开始状态。
	NFAState *AcceptState; 	// 接受状态。
}NFAFragment;

// 转换
typedef struct _Transform
{
	char TransformChar;				 // 状态之间的转换标识符。
	int DFAStateIndex;				 // DFA 状态在数组中的下标。
	NFAState* NFAlist[MAX_STATE_NUM];// NFA 状态集合。
	int NFAStateCount; 				 // 状态集合计数。
	struct _Transform* NextTrans;	 // 指向下一个转换。
}Transform;

// DFA 状态
typedef struct _DFAState
{
	NFAState* NFAlist[MAX_STATE_NUM];// NFA 状态集合。
	int NFAStateCount;				 // 状态集合计数。
	Transform* firstTran;			 // 指向第一个转换。
}DFAState;

// DFA
typedef struct DFA
{
	DFAState* DFAlist[MAX_STATE_NUM]; // DFA 状态集合。
	int length;						  // 状态集合计数。
}DFA;

//
// 在此处声明函数
//

void Closure(NFAState* State, NFAState** StateArray, int* Count);
DFAState* CreateDFAState(Transform* pTransform);
Transform* CreateDFATransform(char TransformChar, NFAState** NFAStateArray, int Count);
int NFAStateIsSubset(DFA* pDFA, Transform* pTransform);
Transform* IsTransformExist(DFAState* pDFAState, char TransformChar);
void AddNFAStateArrayToTransform(NFAState** NFAStateArray, int Count, Transform* pTransform);
DFA* post2dfa(DFA* pDFA, char *postfix);

//
// 在此声明全局变量
//

extern const char VoidTrans;
extern NFAState* Start;

#endif /* NFATODFA_H_ */


