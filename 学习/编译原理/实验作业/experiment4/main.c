#include "RemoveLeftRecursion.h"
#include <string.h>
#include <stdlib.h>

const char* VoidSymbol = "$"; // "ε"
const char* Postfix = "'";

char rule_table[20][256];
char ruleNameArr[20][64];

int main(int argc, char* argv[])
{
	//
	// 调用 InitRules 函数初始化文法
	//	

	Rule* pHead = InitRules();

	//
	// 输出消除左递归之前的文法
	//
	printf("Before Remove Left Recursion:\n");
	PrintRule(pHead);

	//
	// 调用 RemoveLeftRecursion 函数对文法消除左递归
	//
	RemoveLeftRecursion(pHead);
	
	//
	// 输出消除左递归之后的文法
	//
	printf("\nAfter Remove Left Recursion:\n");
	PrintRule(pHead);
	
	return 0;
}

/*
功能：
	将一个 Symbol 添加到 Select 的末尾。

参数：
	pSelect -- Select 指针。
	pNewSymbol -- Symbol 指针。
*/
void AddSymbolToSelect(RuleSymbol* pSelect, RuleSymbol* pNewSymbol)
{
	
	//
	// TODO: 在此添加代码
	//
	RuleSymbol* pSymbol = pSelect;
	while(pSymbol->pNextSymbol!=NULL){
		pSymbol=pSymbol->pNextSymbol;
	}
	pSymbol->pNextSymbol=pNewSymbol;
}

/*
功能：
	将一个 Select 加入到文法末尾。当 Select 为 NULL 时就将一个ε终结符加入到文法末尾。

参数：
	pRule -- 文法指针。
	pNewSelect -- Select 指针。
*/
void AddSelectToRule(Rule* pRule, RuleSymbol* pNewSelect)
{

	//
	// TODO: 在此添加代码
	//
	if(pNewSelect==NULL){
		pNewSelect = CreateSymbol();
		pNewSelect -> isToken = 1;
		pNewSelect->TokenName[0] = '$';
	}
	if(pRule->pFirstSymbol==NULL){
		pRule->pFirstSymbol=pNewSelect;
	}
	else{
		RuleSymbol* pSelect = pRule->pFirstSymbol;
		while(pSelect->pOther!=NULL){
			pSelect=pSelect->pOther;
		}
		pSelect->pOther=pNewSelect;
	}
}

/*
功能：
	消除左递归。

参数：
	pHead -- 文法链表的头指针。
*/
void RemoveLeftRecursion(Rule* pHead)
{
	RuleSymbol* pSelect; // Select 游标
	Rule* pNewRule;  	 // Rule 指针
	
	pNewRule = CreateRule(pHead->RuleName); // 创建新 Rule
	strcat(pNewRule->RuleName, Postfix); //如果原始为A 现在就为A'

	pSelect = pHead->pFirstSymbol; // 初始化 Select 游标
	RuleSymbol **pSelectPrePtr = &pHead->pFirstSymbol;
	while(pSelect != NULL) // 循环处理所有的 Select
	{
		if(0 == pSelect->isToken && pSelect->pRule == pHead)// Select 存在左递归
		{
			// 移除包含左递归的 Select，将其转换为右递归后添加到新 Rule 的末尾，并移动游标
			
			//
			// TODO: 在此添加代码
			//
			//新建一个指向newRule的Symbol
			RuleSymbol* pSymbol2newRule = CreateSymbol();
			pSymbol2newRule->isToken=0;
			pSymbol2newRule->pRule=pNewRule;
			//
			//
			if(pSelect==pHead->pFirstSymbol){
				pHead->pFirstSymbol=pSelect->pOther;
			}else{
				RuleSymbol* pHead_new_select = pHead->pFirstSymbol;
				while(pHead_new_select->pOther!=pSelect){
					pHead_new_select=pHead_new_select->pOther;
				}
				pHead_new_select->pOther=pSelect->pOther;
			}
			RuleSymbol* pafterSymbol = pSelect->pNextSymbol;
			AddSymbolToSelect(pafterSymbol,pSymbol2newRule);
			AddSelectToRule(pNewRule,pafterSymbol);	
			//
			pSelect=pSelect->pOther;
		}
		else // Select 不存在左递归
		{
			// 在没有左递归的 Select 末尾添加指向新 Rule 的非终结符，并移动游标  
			
			//
			// TODO: 在此添加代码
			//
			pSelectPrePtr = &pSelect;
			RuleSymbol* pSymbol2newRule = CreateSymbol();
			pSymbol2newRule->isToken=0;
			pSymbol2newRule->pRule=pNewRule;
			AddSymbolToSelect(pSelect,pSymbol2newRule);
			pSelect = pSelect->pOther;
		}
	}

	// 在新 Rule 的最后加入ε(用 '$' 代替)
	// 将新 Rule 插入文法链表

	//
	// TODO: 在此添加代码
	//
	AddSelectToRule(pNewRule,NULL);
	pHead->pNextRule=pNewRule;
	
	return;
}

/*
功能：
	使用给定的数据初始化文法链表

返回值：
	Rule 指针
*/
typedef struct _SYMBOL
{
	int isToken;
	char Name[MAX_STR_LENGTH];
}SYMBOL;

typedef struct _RULE_ENTRY
{
	char RuleName[MAX_STR_LENGTH];
	SYMBOL Selects[64][64];
}RULE_ENTRY;

/*
功能：
	初始化文法链表。
	
返回值：
	文法的头指针。
*/
Rule* InitRules()
{
	/* A -> Aa | bA | c | Ad */

	int nRuleCount = 0;
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
			ruleNameArr[i][j]= rule_table[i][j];
		}	  
		
		nRuleCount++;
	}
			
	Rule *pHead, *pRule;
	RuleSymbol **pSymbolPtr1, **pSymbolPtr2;
		
	int i, j, k;

	Rule** pRulePtr = &pHead;
	for (i=0; i<nRuleCount; i++)
	{
		*pRulePtr = CreateRule(ruleNameArr[i]);
		pRulePtr = &(*pRulePtr)->pNextRule;
	}

	pRule = pHead;
	for (i=0; i<nRuleCount; i++)
	{
		pSymbolPtr1 = &pRule->pFirstSymbol;
		
		int start = 0;
		for (int j = 0; rule_table[i][j] != '\0'; j++)
		{
			if (rule_table[i][j] == ' '
			 && rule_table[i][j + 1] == '-'
			&& rule_table[i][j + 2] == '>' 
			&& rule_table[i][j + 3] == ' ')
			{
				start = j + 4;
				break;
			}
		}
			
		for (k = start; rule_table[i][k] != '\0'; k++)
		{
			if (rule_table[i][k] == '|')
			{
				pSymbolPtr1 = &(*pSymbolPtr1)->pOther;
				pSymbolPtr2 = pSymbolPtr1;
				continue;
			}
			if (rule_table[i][k] == ' ')
			{
				continue;
			}
			if (k == start)
			{
				pSymbolPtr2 = pSymbolPtr1;
			}

			*pSymbolPtr2 = CreateSymbol();
			
			char tokenName[MAX_STR_LENGTH] = {};
			tokenName[0] = rule_table[i][k];
			tokenName[1] = '\0';
			(*pSymbolPtr2)->isToken = 1;
			for (int m = 0; m < nRuleCount; m++)
			{
				if (strcmp(tokenName, ruleNameArr[m]) == 0)
				{
					(*pSymbolPtr2)->isToken = 0;
					(*pSymbolPtr2)->pRule = FindRule(pHead, tokenName);
					if (NULL == (*pSymbolPtr2)->pRule)
					{
						printf("Init rules error, miss rule \"%s\"\n", tokenName);
						exit(1);
					}
				}
			}
			if ((*pSymbolPtr2)->isToken == 1)
			{
				strcpy((*pSymbolPtr2)->TokenName, tokenName);
			}
			
			pSymbolPtr2 = &(*pSymbolPtr2)->pNextSymbol;
			
		}
			
		pRule = pRule->pNextRule;
	}

	return pHead;
}


/*
功能：
	创建一个新的 Rule。

参数：
	pRuleName -- 文法的名字。
	
返回值：
	Rule 指针	
*/
Rule* CreateRule(const char* pRuleName)
{
	Rule* pRule = (Rule*)malloc(sizeof(Rule));

	strcpy(pRule->RuleName, pRuleName);
	pRule->pFirstSymbol = NULL;
	pRule->pNextRule = NULL;

	return pRule;
}

/*
功能：
	创建一个新的 Symbol。
	
返回值：
	RuleSymbol 指针	
*/
RuleSymbol* CreateSymbol()
{
	RuleSymbol* pSymbol = (RuleSymbol*)malloc(sizeof(RuleSymbol));

	pSymbol->pNextSymbol = NULL;
	pSymbol->pOther = NULL;
	pSymbol->isToken = -1;
	pSymbol->TokenName[0] = '\0';
	pSymbol->pRule = NULL;

	return pSymbol;
}

/*
功能：
	根据 RuleName 在文法链表中查找名字相同的文法。

参数：
	pHead -- 文法链表的头指针。
	RuleName -- 文法的名字。
	
返回值：
	如果存在名字相同的文法返回 Rule 指针，否则返回 NULL	
*/
Rule* FindRule(Rule* pHead, const char* RuleName)
{
	Rule* pRule;
	for (pRule = pHead; pRule != NULL; pRule = pRule->pNextRule)
	{
		if (0 == strcmp(pRule->RuleName, RuleName))
		{
			break;
		}
	}
	
	return pRule;
}	

/*
功能：
	输出文法。

参数：
	pHead -- 文法链表的头指针。
*/
void PrintRule(Rule* pHead)
{
	
	//
	// TODO: 在此添加代码
	//
	Rule* pRule;
	//遍历每条产生式
	for(pRule=pHead ; pRule!=NULL; pRule=pRule->pNextRule){
		printf("%s->",pRule->RuleName);
		RuleSymbol* pSelect;
		//遍历每个select
		for(pSelect=pRule->pFirstSymbol;pSelect!=NULL;pSelect=pSelect->pOther){
			RuleSymbol* pSymobl;
			for(pSymobl=pSelect;pSymobl!=NULL;pSymobl=pSymobl->pNextSymbol){
				if(pSymobl->isToken==1){
					printf("%s",pSymobl->TokenName);
				}else{
					printf("%s",pSymobl->pRule->RuleName);
				}
			}
			if(pSelect->pOther!=NULL)
				printf("|");
		}
		printf("\n");
	}
}

