#ifndef NFAFRAGMENTSTACK_H_
#define NFAFRAGMENTSTACK_H_


//
// 在此处包含 C 标准库头文件
//

//
// 在此处包含其他头文件
//

#include "RegexpToNFA.h"

//
// 在此处定义数据结构
//

#define MAX_STACK_LENGTH 1024 	// 栈的最大长度

// 栈
typedef struct _NFAFragmentStack
{
	NFAFragment buffer[MAX_STACK_LENGTH];	// 栈的缓冲区
	int top;		// 指示栈顶的位置，而不是栈中元素的个数
}NFAFragmentStack;

//
// 在此处声明函数
//

void InitNFAFragmentStack(NFAFragmentStack* pS);
void PushNFAFragment(NFAFragmentStack* pS, NFAFragment Elem);
NFAFragment PopNFAFragment(NFAFragmentStack* pS);
int NFAFragmentStackEmpty(NFAFragmentStack* pS);

//
// 在此处声明全局变量
//

#endif /* NFAFRAGMENTSTACK_H_ */
