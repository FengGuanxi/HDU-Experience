#include<iostream>
typedef struct
{
	TElemType data;
	BiTNode *lchild,*rchild;
}BiTNode,*BiTree;
int InitBiTree(BiTree &T)
{ // 构造空二叉树T
   T=NULL;
   return 1;
}
void CreateBiTree(BiTree &T)
{//根据输入的前缀表达式，构造二叉链表表示的二叉树T。
	if(ch==Nil) // 空
		T=NULL;
	else
	{
		T=(BiTree)malloc(sizeof(BiTNode));
		 if(!T)
		   exit(OVERFLOW);
		 T->data=ch; // 生成根结点
		 CreateBiTree(T->lchild); // 构造左子树
		 CreateBiTree(T->rchild); // 构造右子树
		   }
	 }


}