#include "RegexpToNFA.h"
#include "RegexpToPost.h"
#include "NFAFragmentStack.h"
#include "OutputResult.h"
#include <stdlib.h>

NFAFragmentStack FragmentStack;	// 栈。用于储存 NFA 片段

char regexp[256];

int main(int argc, char *argv[])
{
	char *post;
	NFAState *start;
	// 在input1.txt ～ input8.txt文件中包含了不同的正则表达式案例。
	// 程序开始运行时会通过命令行参数（例如 app.exe < input1.txt）将标准输入（stdin）重定向到一个input文件。
	// 所以这里调用scanf函数就可以读取到input文件中的正则表达式了。
	scanf("%255s", regexp);   

			
	//
	// 初始化栈
	//
	InitNFAFragmentStack(&FragmentStack);
	
	//
	// 调用 re2post 函数将正则表达式字符串转换成解析树的后续遍历序列
	//
	post = re2post(regexp);
	
	//
	// 调用 post2nfa 函数将解析树的后续遍历序列转换为 NFA 并返回开始状态
	//
	start = post2nfa(post);
	
	//
	// 将 NFA 打印输出
	//
	OutputResult(start);
				
	return 0;
}

/*
功能：
	初始化一个状态。
	  
返回值：
	状态指针。
*/
int nstate = 1;  // 状态名计数器
NFAState* CreateNFAState()
{
	NFAState *s = (NFAState*)malloc(sizeof(NFAState));

	s->Name = nstate++;
	s->Transform = '\0';
	s->Next1 = NULL;
	s->Next2 = NULL;
	s->AcceptFlag = 0;
	
	return s;
}

/*
功能：
	将开始状态和接受状态组成一个 Fragment。
	
参数：
	StartState -- 开始状态。
	AcceptState -- 接受状态。	
	  
返回值：
	 Fragment 结构体指针。
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
const char VoidTrans = '$'; // 表示空转换
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

