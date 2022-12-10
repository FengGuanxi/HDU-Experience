#include "RegexpToPost.h"
#include <string.h>

/*
功能：
	将输入的正则表达式字符串转换成解析树的后续遍历序列。

参数：
	re -- 正则表达式指针。
	  
返回值：
	解析树的后续遍历序列数组指针。
*/
char* re2post(char *re)
{
	int nalt; 	// 表示解析到这个字符为止，已经有多少个分支结构 
	int natom;	// 表示解析到这个字符为止，已经有多少个原子结构 
	static char buf[8000];
	char *dst;
	
	struct postseq
	{
		int nalt;
		int natom;
	} paren[100], *p;
	
	p = paren;
	dst = buf;
	nalt = 0;
	natom = 0;
	if (strlen(re) >= sizeof buf/2)
	{
		return NULL;
	}
		
	for (; *re; re++)
	{
		switch (*re)
		{
			case '(':
				if (natom > 1)
				{
					--natom;
					*dst++ = '.';
				}
				if (p >= paren+100)
				{
					return NULL;
				}
				p->nalt = nalt;
				p->natom = natom;
				p++;
				nalt = 0;
				natom = 0;
				break;
			case '|':
				if (natom == 0)
				{
					return NULL;
				}
				while (--natom > 0)
				{
					*dst++ = '.';
				}
				nalt++;
				break;
			case ')':
				if (p == paren)
				{
					return NULL;
				}
				if (natom == 0)
				{
					return NULL;
				}
				while (--natom > 0)
				{
					*dst++ = '.';
				}
				for (; nalt > 0; nalt--)
				{
					*dst++ = '|';
				}
				--p;
				nalt = p->nalt;
				natom = p->natom;
				natom++;
				break;
			case '*':
			case '+':
			case '?':
				if (natom == 0)
				{
					return NULL;
				}
				*dst++ = *re;
				break;
			default:
				if (natom > 1)
				{
					--natom;
					*dst++ = '.';
				}
				*dst++ = *re;
				natom++;
				break;
		}
	}
	if (p != paren)
	{
		return NULL;
	}
	while (--natom > 0)
	{
		*dst++ = '.';
	}
	for (; nalt > 0; nalt--)
	{
		*dst++ = '|';
	}
	*dst = 0;
	
	return buf;
}
