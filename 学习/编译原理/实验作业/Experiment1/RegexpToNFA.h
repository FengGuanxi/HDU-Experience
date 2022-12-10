#ifndef REGEXPTONFA_H_
#define REGEXPTONFA_H_

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

typedef struct _NFAState
{
	char Transform;       	 // 状态间转换的标识，用 '$' 表示 'ε-转换'
	struct _NFAState *Next1; // 指向下一个状态 
	struct _NFAState *Next2; // 指向下一个状态
	int Name;			     // 状态名称
	int AcceptFlag;		     // 是否为接受状态的标志，1 表示是接受状态 0 表示不是接受状态 
}NFAState;

// Fragment 结构是一个 NFA 的片段
typedef struct _NFAFragment
{
	NFAState *StartState; 	// 开始状态
	NFAState *AcceptState; 	// 接受状态
}NFAFragment;

//
// 在此处声明函数
//

NFAState* CreateNFAState();
NFAState* post2nfa(char *postfix);
NFAFragment MakeNFAFragment(NFAState *StartState, NFAState *AcceptState);

//
// 在此声明全局变量
//

extern int nstate;
extern const char VoidTrans; 
// extern char* regexp;

#endif /* REGEXPTONFA_H_ */
