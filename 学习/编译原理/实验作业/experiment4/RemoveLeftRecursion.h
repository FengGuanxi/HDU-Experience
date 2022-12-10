#ifndef REMOVELEFTRECURSIONNOREPLACE_H_
#define REMOVELEFTRECURSIONNOREPLACE_H_

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

#define MAX_STR_LENGTH 64

struct _Rule;
typedef struct _RuleSymbol
{
	struct _RuleSymbol* pNextSymbol; // 指向下一个 Symbol
	struct _RuleSymbol* pOther;		 // 指向下一个 Select
	int isToken;					 // 是否为终结符。1 表示终结符，0 表示非终结符
	char TokenName[MAX_STR_LENGTH];	 // 终结符的名称。isToken 为 1 时这个域有效
	struct _Rule* pRule;			 // 指向 Symbol 对应的 Rule。isToken 为 0 时这个域有效
}RuleSymbol;

typedef struct _Rule
{
	char RuleName[MAX_STR_LENGTH];	  // 文法的名称	
	struct _RuleSymbol* pFirstSymbol; // 指向文法的第一个 Select 的第一个 Symbol
	struct _Rule* pNextRule;		  // 指向下一条文法
}Rule;


//
// 在此处声明函数
//

Rule* InitRules();
Rule* CreateRule(const char* pRuleName);
RuleSymbol* CreateSymbol();
Rule* FindRule(Rule* pHead, const char* RuleName);

void AddSymbolToSelect(RuleSymbol* pSelect, RuleSymbol* pNewSymbol);
void AddSelectToRule(Rule* pRule, RuleSymbol* pNewSelect);
void RemoveLeftRecursion(Rule* pHead);

void PrintRule(Rule* pHead);

//
// 在此声明全局变量
//

extern const char* VoidSymbol;
extern const char* Postfix;

#endif /* REMOVELEFTRECURSIONNOREPLACE_H_ */

