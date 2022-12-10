#include "PostToNFA.h"
#include "NFAFragmentStack.h"
#include <stdlib.h>

NFAFragmentStack FragmentStack;

/*
功能：
	初始化一个 NFA 状态。

返回值：
	状态结构体指针。
*/
int nstate = 1;
NFAState* CreateNFAState()
{
	NFAState* s = (NFAState*)malloc(sizeof(NFAState));

	s->Name = nstate++;
	s->Transform = '\0';
	s->Next1 = NULL;
	s->Next2 = NULL;
	s->AcceptFlag = 0;
	
	return s;
}

/*
功能：
	初始化 NFAFragment 结构体。
	
参数：
	StartState -- 开始状态。
	AcceptState -- 接受状态。	
	  
返回值：
	 NFAFragment 结构体指针。
*/
NFAFragment MakeNFAFragment(NFAState *StartState, NFAState *AcceptState)
{
	NFAFragment n = {StartState, AcceptState};
	return n;
}

/*
功能：
	将解析树的后序序列转换为 NFA。

参数：
	postfix -- 解析树的后序序列指针。
	  
返回值：
	NFA 的开始状态指针。
*/
NFAState* post2nfa(char *postfix)
{
	char *p;							   		// 游标
	NFAFragment fragment1, fragment2, fm;     	// NFA 片段结构体变量
	NFAFragment fragment = {0, 0};			   	// 初始化用于返回的 NFA 片段
	NFAState *NewStartState, *NewAcceptState; 	// 开始状态和接受状态指针
	
	for(p=postfix; *p != '\0'; p++)
	{
		switch(*p)
		{
			default:	// 构造单字符 NFA 片段
				// 调用 CreateNFAState 函数生成两个新的状态
				NewStartState = CreateNFAState();
				NewAcceptState = CreateNFAState();
				// 开始状态通过单字符转换到接受状态
				NewStartState->Transform = *p;
				NewStartState->Next1 = NewAcceptState;
				NewAcceptState->AcceptFlag = 1;
				// 调用 MakeNFAFragment 函数生成一个新的 NFA 片段, 并入栈
				fm = MakeNFAFragment(NewStartState, NewAcceptState);
				PushNFAFragment(&FragmentStack, fm);
				break;
			case '.':	// 构造连接 NFA 片段
				// 栈顶的两个片段出栈，构造新的 NFA 片段
				fragment2 = PopNFAFragment(&FragmentStack);
				fragment1 = PopNFAFragment(&FragmentStack);
				// 片段 1 通过ε转换与片段 2 连接
				fragment1.AcceptState->AcceptFlag = 0;
				fragment1.AcceptState->Next1 = fragment2.StartState;
				fragment1.AcceptState->Transform = VoidTrans;
				// 调用 MakeNFAFragment 函数生成一个新的 NFA 片段, 并入栈
				fm = MakeNFAFragment(fragment1.StartState, fragment2.AcceptState);
				PushNFAFragment(&FragmentStack, fm);
				break;
			case '|':	// 构造选择 NFA 片段
				
				//
				// TODO: 在此添加代码
				//
				// 栈顶的两个片段出栈，构造新的 NFA 片段
				fragment2 = PopNFAFragment(&FragmentStack);
				fragment1 = PopNFAFragment(&FragmentStack);
				//
				fragment2.AcceptState->AcceptFlag =0;
				fragment1.AcceptState->AcceptFlag =0;
				//
				NewStartState = CreateNFAState();
				NewAcceptState = CreateNFAState();
				//
				NewStartState->Next1 = fragment1.StartState;
				NewStartState->Next2 = fragment2.StartState;
				NewStartState->Transform = VoidTrans;
				//
				fragment1.AcceptState->Next1 = NewAcceptState;
				fragment1.AcceptState->Transform = VoidTrans;
				fragment2.AcceptState->Next1 = NewAcceptState;
				fragment2.AcceptState->Transform = VoidTrans;
				//
				NewAcceptState->AcceptFlag = 1;
				//
				fm = MakeNFAFragment(NewStartState, NewAcceptState);
				PushNFAFragment(&FragmentStack, fm);
								
				break;
			case '*':	// 构造星号 NFA 片段
				
				//
				// TODO: 在此添加代码
				//
				// 栈顶的两个片段出栈，构造新的 NFA 片段
				fragment1 = PopNFAFragment(&FragmentStack);
				//
				fragment1.AcceptState->AcceptFlag = 0;
				//
				NewStartState = CreateNFAState();
				NewAcceptState = CreateNFAState();
				//
				NewStartState->Next1 = fragment1.StartState;
				NewStartState->Next2 = NewAcceptState;
				NewStartState->Transform = VoidTrans;
				//
				fragment1.AcceptState->Next1 = NewAcceptState;
				fragment1.AcceptState->Next2 = fragment1.StartState;
				fragment1.AcceptState->Transform = VoidTrans;
				//
				NewAcceptState->AcceptFlag = 1;
				//
				fm = MakeNFAFragment(NewStartState, NewAcceptState);
				PushNFAFragment(&FragmentStack, fm);	
								
				break;
			case '?':	// 构造问号 NFA 片段
				
				//
				// TODO: 在此添加代码
				//
				fragment1 = PopNFAFragment(&FragmentStack);
				fragment1.AcceptState->AcceptFlag = 0;
				//
				NewStartState = CreateNFAState();
				NewAcceptState = CreateNFAState();
				//
				NewStartState->Next1 = fragment1.StartState;
				NewStartState->Next2 = NewAcceptState;
				NewStartState->Transform = VoidTrans;
				//
				fragment1.AcceptState->Next1 = NewAcceptState;
				fragment1.AcceptState->Transform = VoidTrans;
				NewAcceptState->AcceptFlag = 1;
				//
				fm = MakeNFAFragment(NewStartState, NewAcceptState);
				PushNFAFragment(&FragmentStack, fm);

								
				break;
			case '+':	// 构造加号 NFA 片段
				
				//
				// TODO: 在此添加代码
				//
				
				fragment = PopNFAFragment(&FragmentStack);
				fragment.AcceptState->AcceptFlag = 0;
				NewAcceptState = CreateNFAState();
				NewAcceptState->AcceptFlag = 1;
				//
				fragment.AcceptState->Next1 = NewAcceptState;
				fragment.AcceptState->Transform = VoidTrans;
				NewAcceptState->Next1 = fragment.StartState;
				NewAcceptState->Transform = VoidTrans;
				//
				fm = MakeNFAFragment(fragment.StartState, NewAcceptState);
				PushNFAFragment(&FragmentStack, fm);
				break;

		}
	}
	
	//
	// 将 NFA 最终结果出栈保存到 fragment 中
	//
	fragment = PopNFAFragment(&FragmentStack);

	return fragment.StartState;
}

