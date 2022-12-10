#ifndef NFASTATESTACK_H_
#define NFASTATESTACK_H_

//
// 在此处包含 C 标准库头文件
//

//
// 在此处包含其他头文件
//

//
// 在此处定义数据结构
//

#define MAX_STACK_LENGTH 1024 	// 栈的最大长度

// 栈
typedef struct _NFAStateStack
{
	NFAState* buffer[MAX_STACK_LENGTH];	// 栈的缓冲区
	int top;		// 指示栈顶的位置，而不是栈中元素的个数
}NFAStateStack;

//
// 在此处声明函数
//

void InitNFAStateStack(NFAStateStack* pS);
void PushNFAState(NFAStateStack* pS, NFAState* Elem);
NFAState* PopNFAState(NFAStateStack* pS);
int NFAStateStackEmpty(NFAStateStack* pS);

//
// 在此处声明全局变量
//

#endif /* NFASTATESTACK_H_ */

