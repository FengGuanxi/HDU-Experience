#include "Follow.h"
#include <string.h>
#include <stdlib.h>

const char *VoidSymbol = "#"; // "ε"
const char *DollarSymbol = "$";

char rule_table[20][256];
char ruleNameArr[20][64];

int main(int argc, char *argv[])
{
	//
	// 调用 InitRules 函数初始化文法
	//

	Rule *pHead = InitRules();

	//
	// 初始化 First 集合、Follow 集合
	//
	SetList FirstSetList, FollowSetList;
	FirstSetList.nSetCount = 0;
	FollowSetList.nSetCount = 0;

	//
	// 调用 Follow 函数求文法的 First 集合、Follow 集合
	//
	Follow(pHead, &FollowSetList, &FirstSetList);

	//
	// 输出文法
	//
	PrintRule(pHead);

	//
	// 输出Follow集合
	//
	PrintFollowSet(&FollowSetList);

	return 0;
}

/*
功能：
	添加一个 Set 到 SetList。

参数：
	pSetList -- SetList 指针。
	pName -- 集合名称字符串指针。
*/
void AddOneSet(SetList *pSetList, const char *pName)
{

	//
	// TODO: 在此添加代码
	//
	Set *pSet = GetSet(pSetList, pName);
	if (pSet == NULL)
	{
		Set newSet;
		strcpy(newSet.Name, pName);
		pSetList->Sets[pSetList->nSetCount++] = newSet;
	}
}

/*
功能：
	根据名称在 SetList 中查找。

参数：
	pSetList -- SetList 指针。
	pName -- 集合名称字符串指针。

返回值：
	如果找到返回 Set 指针，否则返回 NULL。
*/
Set *GetSet(SetList *pSetList, const char *pName)
{

	//
	// TODO: 在此添加代码
	//
	for (int i = 0; i < pSetList->nSetCount; i++)
	{
		if (strcmp(pSetList->Sets[i].Name, pName) == 0)
		{
			return &pSetList->Sets[i];
		}
	}
	return NULL;
}

/*
功能：
	添加一个终结符到 Set。

参数：
	pSet -- Set 指针。
	pTerminal -- 终结符名称指针。

返回值：
	添加成功返回 1，否则返回 0。
*/
int AddTerminalToSet(Set *pSet, const char *pTerminal)
{

	//
	// TODO: 在此添加代码
	//
	for (int i = 0; i < pSet->nTerminalCount; i++)
	{
		if (strcmp(pSet->Terminal[i], pTerminal) == 0)
			return 0;
	}
	strcpy(pSet->Terminal[pSet->nTerminalCount++], pTerminal);
	return 1;
}

/*
功能：
	将源 Set 添加到目标 Set 中，忽略ε。

参数：
	pDesSet -- 目标 Set 指针。
	pSrcSet -- 源 Set 指针。

返回值：
	添加成功返回 1，否则返回 0。
*/
int AddSetToSet(Set *pDesSet, const Set *pSrcSet)
{

	//
	// TODO: 在此添加代码
	//
	int flag = 0;
	for (int i = 0; i < pSrcSet->nTerminalCount; i++)
	{
		if (strcmp(pSrcSet->Terminal[i], VoidSymbol) == 0)
			continue;
		int isAdd = AddTerminalToSet(pDesSet, pSrcSet->Terminal[i]);
		flag += isAdd;
	}
	return flag > 0 ? 1 : 0;
}

/*
功能：
	判断 Set 中是否含有ε。

参数：
	pSet -- Set 指针。

返回值：
	存在返回 1。
	不存在返回 0。
*/
int SetHasVoid(const Set *pSet)
{

	//
	// TODO: 在此添加代码
	//
	for (int i = 0; i < pSet->nTerminalCount; i++)
	{
		if (strcmp(pSet->Terminal[i], VoidSymbol) == 0)
			return 1;
	}
	return 0;
}

/*
功能：
	求文法的 First 集合。

参数：
	pHead -- 文法的头指针。
	pFirstSetList -- First 集合指针。
*/
void First(const Rule *pHead, SetList *pFirstSetList)
{
	const Rule *pRule;	 // Rule 指针
	int isChange;		 // 集合是否被修改的标志
	RuleSymbol *pSymbol; // Symbol 游标

	//
	// TODO: 在此添加代码
	//
	// 使用文法链表初始化 First 集合
	for (pRule = pHead; pRule != NULL; pRule = pRule->pNextRule)
	{
		AddOneSet(pFirstSetList, pRule->RuleName);
	}

	do
	{
		isChange = 0; // 设置修改标志

		for (pRule = pHead; pRule != NULL; pRule = pRule->pNextRule)
		{
			// 根据文法名称在 pFirstSetList 中查找 Set
			Set *pDesSet = GetSet(pFirstSetList, pRule->RuleName);

			int hasVoid = 1; // First 集合中是否含有ε的标志
			for (pSymbol = pRule->pFirstSymbol; pSymbol != NULL && hasVoid; pSymbol = pSymbol->pNextSymbol)
			{
				if (pSymbol->isToken) // 终结符
				{
					// 调用 AddTerminalToSet 函数将终结符添加到 pDesSet，并设置修改标志
					if (AddTerminalToSet(pDesSet, pSymbol->SymbolName))
						isChange = 1;

					hasVoid = 0; // 设置 First 集合中是否含有ε的标志
				}
				else // 非终结符
				{
					// 根据非终结符名称在 pFirstSetList 中查找 Set
					Set *pSrcSet = GetSet(pFirstSetList, pSymbol->SymbolName);

					// 调用 AddSetToSet 函数，将源 Set 中的所有终结符添加到目标 Set 中，并设置修改标志
					if (AddSetToSet(pDesSet, pSrcSet))
						isChange = 1;

					// 调用 SetHasVoid 函数，判断源 Set 中是否含有ε
					hasVoid = SetHasVoid(pSrcSet);
				}
			}
			if (hasVoid)
			{
				// 调用 AddTerminalToSet 函数将ε加入到目标集合中
				AddTerminalToSet(pDesSet, VoidSymbol);
			}
		}
	} while (isChange);
}

/*
功能：
	求文法的 Follow 集合。

参数：
	pHead -- 文法的头指针。
	pFollowSetList -- Follow 集合指针。
	pFirstSetList -- First 集合指针。
*/
void Follow(const Rule *pHead, SetList *pFollowSetList, SetList *pFirstSetList)
{
	const Rule *pRule;	 // Rule 指针
	int isChange;		 // 集合是否被修改的标志
	RuleSymbol *pSymbol; // Symbol 游标

	// 调用 First 函数求文法的 First 集合
	First(pHead, pFirstSetList);

	// 使用文法链表初始化 Follow 集合
	for (pRule = pHead; pRule != NULL; pRule = pRule->pNextRule)
	{
		AddOneSet(pFollowSetList, pRule->RuleName);
	}

	// 在开始符号的 Follow 集合中加入终结符 $
	AddTerminalToSet(&pFollowSetList->Sets[0], DollarSymbol);

	do
	{
		isChange = 0; // 设置集合修改标志

		for (pRule = pHead; pRule != NULL; pRule = pRule->pNextRule)
		{
			for (pSymbol = pRule->pFirstSymbol; pSymbol != NULL; pSymbol = pSymbol->pNextSymbol)
			{
				// 忽略终结符
				if (pSymbol->isToken)
					continue;

				// 初始化一个 First 集合 TempFirstSet，作为临时变量
				Set TempFirstSet;
				TempFirstSet.nTerminalCount = 0;

				if (NULL != pSymbol->pNextSymbol)
				{
					if (pSymbol->pNextSymbol->isToken) // 终结符
					{
						// 调用 AddTerminalToSet 函数将该终结符加入到 TempFirstSet
						AddTerminalToSet(&TempFirstSet, pSymbol->pNextSymbol->SymbolName);
					}
					else // 非终结符
					{
						// 调用 GetSet 函数在 pFirstSetList 中查找 Set
						Set *pFirstSetXn = GetSet(pFirstSetList, pSymbol->pNextSymbol->SymbolName);

						// 将找到的 Set 加入 TempFirstSet 中
						AddSetToSet(&TempFirstSet, pFirstSetXn);
					}
				}
				else
				{
					// 调用 AddTerminalToSet 函数将 VoidSymbol 加入到 TempFirstSet 中
					AddTerminalToSet(&TempFirstSet, VoidSymbol);
				}

				// 调用 GetSet 函数在 pFollowSetList 中查找 Set
				Set *pFollowSetXi = GetSet(pFollowSetList, pSymbol->SymbolName);

				// 将集合 TempFirstSet 加入 pFollowSetXi
				if (AddSetToSet(pFollowSetXi, &TempFirstSet))
					isChange = 1; // 设置集合修改标志

				if (SetHasVoid(&TempFirstSet)) // 判断集合 TempFirstSet 是否含有ε
				{
					// 调用 GetSet 函数得到当前文法名称对应的 Follow 集合
					Set *pFollowSetA = GetSet(pFollowSetList, pRule->RuleName);

					// 将集合 pFollowSetA 加入 pFollowSetXi
					if (AddSetToSet(pFollowSetXi, pFollowSetA))
						isChange = 1; // 设置集合修改标志
				}
			}
		}

	} while (isChange);
}

typedef struct _SYMBOL
{
	int isToken;
	char SymbolName[MAX_STR_LENGTH];
} SYMBOL;

typedef struct _RULE_ENTRY
{
	char RuleName[MAX_STR_LENGTH];
	SYMBOL Symbols[64];
} RULE_ENTRY;

Rule *InitRules()
{
	int nRuleCount = 0;
	// 需要读取多行文本
	for (int i = 0; i < 20; i++)
	{
		gets(rule_table[i]);
		int length = strlen(rule_table[i]);
		if (length == 0)
		{
			break;
		}

		for (int j = 0; j < length; j++)
		{
			if (rule_table[i][j] == ' ')
			{
				ruleNameArr[i][j] = '\0';
				break;
			}
			ruleNameArr[i][j] = rule_table[i][j];
		}
		nRuleCount++;
	}

	Rule *pHead, *pRule;
	RuleSymbol **pSymbolPtr, *pNewSymbol;

	Rule **pRulePtr = &pHead;
	for (int i = 0; i < nRuleCount; i++)
	{
		*pRulePtr = CreateRule(ruleNameArr[i]);
		pRulePtr = &(*pRulePtr)->pNextRule;
	}

	pRule = pHead;
	for (int i = 0; i < nRuleCount; i++)
	{
		pSymbolPtr = &pRule->pFirstSymbol;

		int start = 0;
		for (int j = 0; rule_table[i][j] != '\0'; j++)
		{
			if (rule_table[i][j] == ' ' && rule_table[i][j + 1] == '-' && rule_table[i][j + 2] == '>' && rule_table[i][j + 3] == ' ')
			{
				start = j + 4;
				break;
			}
		}

		for (int k = start; rule_table[i][k] != '\0'; k++)
		{
			if (rule_table[i][k] == ' ')
			{
				continue;
			}

			pNewSymbol = CreateSymbol();
			char tokenName[MAX_STR_LENGTH] = {};

			for (int m = 0;; m++)
			{
				if (rule_table[i][k] == ' ' || rule_table[i][k] == '\0' || rule_table[i][k] == '\n')
				{
					tokenName[m] = '\0';
					break;
				}
				tokenName[m] = rule_table[i][k++];
			}

			strcpy(pNewSymbol->SymbolName, tokenName);

			pNewSymbol->isToken = 1;
			for (int n = 0; n < nRuleCount; n++)
			{
				if (strcmp(pNewSymbol->SymbolName, ruleNameArr[n]) == 0)
				{
					pNewSymbol->isToken = 0;
					break;
				}
			}

			*pSymbolPtr = pNewSymbol;

			pSymbolPtr = &pNewSymbol->pNextSymbol;
		}

		pRule = pRule->pNextRule;
	}

	return pHead;
}

/*
功能：
	创建一个新的文法。

参数：
	pRuleName -- 文法的名字。

返回值：
	文法的指针。
*/
Rule *CreateRule(const char *pRuleName)
{
	Rule *pRule = (Rule *)malloc(sizeof(Rule));

	strcpy(pRule->RuleName, pRuleName);
	pRule->pFirstSymbol = NULL;
	pRule->pNextRule = NULL;

	return pRule;
}

/*
功能：
	创建一个新的符号。

返回值：
	符号的指针。
*/
RuleSymbol *CreateSymbol()
{
	RuleSymbol *pSymbol = (RuleSymbol *)malloc(sizeof(RuleSymbol));

	pSymbol->pNextSymbol = NULL;
	pSymbol->isToken = -1;
	pSymbol->SymbolName[0] = '\0';

	return pSymbol;
}

/*
功能：
	输出文法。

参数：
	pHead -- 文法的头指针。
*/
void PrintRule(const Rule *pHead)
{
	const Rule *pRule;
	for (pRule = pHead; pRule != NULL; pRule = pRule->pNextRule)
	{
		printf("%s ->", pRule->RuleName);

		RuleSymbol *pRuleSymbol;
		for (pRuleSymbol = pRule->pFirstSymbol; pRuleSymbol != NULL;
			 pRuleSymbol = pRuleSymbol->pNextSymbol)
		{
			printf(" %s", pRuleSymbol->SymbolName);
		}
		printf("\n");
	}
}

/*
功能：
	输出 First 集合。

参数：
	pFirstSetList -- First 集合指针。
*/
void PrintFollowSet(SetList *pFollowSetList)
{
	printf("\nThe Follow Set:\n");
	for (int i = 0; i < pFollowSetList->nSetCount; i++)
	{
		printf("Follow(%s) = { ", pFollowSetList->Sets[i].Name);
		for (int j = 0; j < pFollowSetList->Sets[i].nTerminalCount; j++)
		{
			if (j == pFollowSetList->Sets[i].nTerminalCount - 1)
			{
				printf("%s ", pFollowSetList->Sets[i].Terminal[j]);
			}
			else
			{
				printf("%s, ", pFollowSetList->Sets[i].Terminal[j]);
			}
		}
		printf("}\n");
	}
}
