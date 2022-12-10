#include "OutputResult.h"

/*
功能：
	输出 DFA 邻接表。

参数：
	dfa -- DFA 指针。
*/
void OutputResult(DFA* dfa)
{
	int i =0, j = 0;
	for (j = 0; j < dfa->length;j++)
	{	
		printf("%d ", j);		
		int count = dfa->DFAlist[j]->NFAStateCount;
		int flag = 0;
		for (i = 0; i < count; i++)
		{
			if (dfa->DFAlist[j]->NFAlist[i]->AcceptFlag == 1)
			{
				flag = 1;
				break;
			}
		}
		for (i = 0; i < count; i++)
		{
			if (count == 1)
			{
				printf("%d->", dfa->DFAlist[j]->NFAlist[i]->Name);
			}
			else
			{
				if (i == 0)
				{
					if (flag == 1)
					{
						printf("[%d ", dfa->DFAlist[j]->NFAlist[i]->Name);
					}
					else
					{
						printf("%d ", dfa->DFAlist[j]->NFAlist[i]->Name);
					}
					
				}
				else if (i == count - 1)
				{
					if (flag == 1)
					{
						printf("%d]->", dfa->DFAlist[j]->NFAlist[i]->Name);
					}
					else
					{
						printf("%d->", dfa->DFAlist[j]->NFAlist[i]->Name);
					}
					
				}
				else
				{
					printf("%d ", dfa->DFAlist[j]->NFAlist[i]->Name);
				}	
			}	
		}

			
		Transform* curTran = NULL;
		curTran = dfa->DFAlist[j]->firstTran;
		while (curTran != NULL)
		{
			for (i = 0; i < curTran->NFAStateCount; i++)
			{
				if (curTran->NFAStateCount == 1)
				{
					printf("%d->", curTran->NFAlist[i]->Name);
				}
				else
				{
					if (i == 0)
					{
						printf("%c(%d,", curTran->TransformChar, curTran->NFAlist[i]->Name);
					}
					else if (i == curTran->NFAStateCount - 1)
					{
						printf("%d)%d->", curTran->NFAlist[i]->Name, curTran->DFAStateIndex);
					}
					else
					{
						printf("%d,", curTran->NFAlist[i]->Name);
					}	
				}	
			}
			curTran = curTran->NextTrans;
		}
		printf("NULL\n");			
	}	
}
