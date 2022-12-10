#include "NFAToDFA.h"
#include "NFAStateStack.h"


/*
功能：
	初始化栈。

参数：
	pS -- 栈的指针
*/
void InitNFAStateStack(NFAStateStack* pS)
{
	pS->top = -1;
}

/*
功能：
	将元素入栈。

参数：
	pS -- 栈的指针
	Elem -- 入栈的元素
  
返回值：
	空
*/
void PushNFAState(NFAStateStack* pS, NFAState* Elem)
{
	//
	// 栈满，入栈失败。
	//
	if (MAX_STACK_LENGTH - 1 <= pS->top)
	{
		return;
	}
		
	pS->top++;
	pS->buffer[pS->top] = Elem; // 将元素插入栈顶
	
	return;
}

/*
功能：
	将栈顶元素出栈

参数：
	pS -- 栈的指针
  
返回值：
	如果出栈成功返回出栈元素的值。
	如果出栈失败返回 -1。
*/
NFAState* PopNFAState(NFAStateStack* pS)
{
	int pos;
	NFAState* State = 0;
	
	//
	// 栈为空，出栈失败
	//	
	if (NFAStateStackEmpty(pS))
	{
		return State;
	}
	
	pos = pS->top;
	pS->top--;
	
	return pS->buffer[pos];
}

/*
功能：
	判断栈是否为空。

参数：
	pQ -- 栈的指针
  
返回值：
	如果栈空返回 1（真）
	如果栈非空返回 0（假）
*/
int NFAStateStackEmpty(NFAStateStack* pS)
{
	return -1 == pS->top ? 1 : 0;
}

