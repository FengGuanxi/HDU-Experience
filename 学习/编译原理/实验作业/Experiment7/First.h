#ifndef FIRST_H_
#define FIRST_H_

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
	int isToken;					 // 是否为终结符。1 表示终结符，0 表示非终结符
	char SymbolName[MAX_STR_LENGTH]; // 终结符和非终结符的名称
}RuleSymbol;

typedef struct _Rule
{
	char RuleName[MAX_STR_LENGTH];	 // 文法的名称
	struct _RuleSymbol* pFirstSymbol;// 指向文法的第一个 Symbol
	struct _Rule* pNextRule;		 // 指向下一条文法
}Rule;

typedef struct _Set
{
	char Name[MAX_STR_LENGTH];		  // 集合的名称
	char Terminal[32][MAX_STR_LENGTH];// 终结符数组
	int nTerminalCount;				  // 数组元素个数
}Set;

typedef struct _SetList
{
	Set Sets[32];	// 集合数组
	int nSetCount;	// 数组元素个数
}SetList;


//
// 在此处声明函数
//

void First(const Rule* pHead, SetList* pFirstSetList);
Set* GetSet(SetList* pSetList, const char* pName);
void AddOneSet(SetList* pSetList, const char* pName);
int AddTerminalToSet(Set* pSet, const char* pTerminal);
int AddSetToSet(Set* pDesSet, const Set* pSrcSet);
int SetHasVoid(const Set* pSet);

Rule* InitRules();
Rule* CreateRule(const char* pRuleName);
RuleSymbol* CreateSymbol();

void PrintRule(const Rule* pHead);

void PrintFirstSet(SetList* pFirstSetList);

//
// 在此声明全局变量
//

extern const char* VoidSymbol;

#endif /* FIRST_H_ */
