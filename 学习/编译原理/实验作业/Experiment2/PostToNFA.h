#ifndef POSTTONFA_H_
#define POSTTONFA_H_

//
// 在此处包含 C 标准库头文件
//

//
// 在此处包含其他头文件
//

#include "NFAToDFA.h"

//
// 在此处定义数据结构
//

//
// 在此处声明函数
//

NFAState* CreateNFAState();
NFAFragment MakeNFAFragment(NFAState *StartState, NFAState *AcceptState);
NFAState* post2nfa(char *postfix);

//
//  在此声明全局变量
//

#endif /* POSTTONFA_H_ */
