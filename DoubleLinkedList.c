#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;


#define EXPECT_EQ_BASE(equality, expect, actual, format) \
    do {\
        test_count++;\
        if (equality)\
            test_pass++;\
        else {\
            fprintf(stderr, "%s:%d: expect: " format " actual: " format "\n", __FILE__, __LINE__, expect, actual);\
            main_ret = 1;\
        }\
    } while(0)

#define EXPECT_EQ_INT(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%d")

typedef int ElemType;

enum Status
{
    OK = 1,
    TRUE = 2,
    FALSE = 0,
    ERROR = 0,
    OVERFLOW = 0,

};
typedef struct DuLNode
{
    ElemType data;
    struct DuLNode *prior, *next;
} DuLNode, *DuLinkList; //注释1

//带头结点的双向循环链表的基本操作：
DuLinkList InitList()
{ /* 产生空的双向循环链表L */
    DuLinkList L = (DuLinkList)malloc(sizeof(DuLNode));
    if (L)
        L->next = L->prior = L;
    else
        exit(1);
    return L;
}
//销毁双向循环链表L：
void DestroyList(DuLinkList L)
{
    DuLinkList q, p = L->next; /* p指向第一个结点 */
    while (p != L)             /* p没到表头 */
    {
        q = p->next;
        free(p);
        p = q;
    }
    free(L);
}
//重置链表为空表：
void ClearList(DuLinkList L) /* 不改变L */
{
    DuLinkList q, p = L->next; /* p指向第一个结点 */
    while (p != L)             /* p没到表头 */
    {
        q = p->next;
        free(p);
        p = q;
    }
    L->next = L->prior = L; /*头结点的两个指针域均指向自身 */
}

//验证是否为空表(只有头节点)
//注释2
enum Status ListEmpty(DuLinkList L)
{ /* 初始条件：线性表L已存在*/
    if (L->next == L && L->prior == L)
        return TRUE;
    else
        return FALSE;
}

/*计算表内元素个数*/
int ListLength(DuLinkList L) //注释3
{ /* 初始条件：L已存在。操作结果： */
    int i = 0;
    DuLinkList p = L->next;  /* p指向第一个结点 */

    while (p != L) /* p没到表头 */
    {
        i++;
        p = p->next;
    }
    return i;
}
//赋值：
enum Status GetElem(DuLinkList L, int i, ElemType *e)
{              
    /* 当第i个元素存在时，其值赋给e并返回OK，否则返回ERROR */
    int cur = 1; 
    DuLinkList p = L->next; 

    while (p != L && cur < i)
    {
        p = p->next;
        cur++;
    }
    if (p == L || cur > i) /* 第i个元素不存在 */
        return ERROR;
    *e = p->data; /* 取第i个元素 */
    return OK;
}
enum Status compare(ElemType a, ElemType b)
{
    if (a == b)
    {
        return TRUE;
    }
    return FALSE;
}

//查找元素：
int LocateElem(DuLinkList L, ElemType e, enum Status (*compare)(ElemType, ElemType))
{ /* 初始条件：L已存在，compare()是数据元素判定函数 */
    /* 操作结果：返回L中第1个与e满足关系compare()的数据元素的位序。 */
    /* 若这样的数据元素不存在，则返回值为0 */
    int cur = 0;
    DuLinkList p = L->next; /* p指向第1个元素 */
    while (p != L)
    {
        cur++;
        if (compare(p->data, e)) /* 找到这样的数据元素*/
            return cur;
        p = p->next;
    }
    return 0;
}
//查找元素前驱：
enum Status PriorElem(DuLinkList L, ElemType cur_e, ElemType *pre_e)
{
    DuLinkList p = L->next->next;

    if (ListLength(L) < 2)
    {
        return FALSE;
    }
    while (p != L) /* p没到表头 */
    {
        if (p->data == cur_e)
        {
            *pre_e = p->prior->data;
            return TRUE;
        }
        p = p->next;
    }
    return FALSE;
}
//查找元素后继：
enum Status NextElem(DuLinkList L, ElemType cur_e, ElemType *next_e)
{
    DuLinkList p = L->next;

    while (p->next != L) /* p没到表头 */
    {
        if (p->data == cur_e)
        {
            *next_e = p->next->data;
            return TRUE;
        }
        p = p->next;
    }
    return FALSE;
}
//查找元素地址：
DuLinkList GetElemP(DuLinkList L, int i) 
{   
    /* 在双向链表L中返回第i个元素的地址。i为0，返回头结点的地址。若第i个元素不存在，*/
    /* 返回NULL */
    int j;
    DuLinkList p = L;               /* p指向头结点 */
    if (i < 0 || i > ListLength(L)) /* i值不合法 */
        return NULL;
    if (i == 0)
    {
        return L;
    }
    for (j = 1; j <= i; j++)
        p = p->next;
    return p;
}
//元素的插入：
enum Status ListInsert(DuLinkList L, int i, ElemType e)
{ /* 在带头结点的双链循环线性表L中第i个位置之前插入元素e，i的合法值为1≤i≤表长+1 */
    DuLinkList p, s;
    if (i < 1 || i > ListLength(L) + 1) /* i值不合法 */
        return ERROR;
    p = GetElemP(L, i - 1); /* 在L中确定第i个元素前驱的位置指针p */
    if (!p)                 /* p=NULL,即第i个元素的前驱不存在(设头结点为第1个元素的前驱) */
        return ERROR;
    s = (DuLinkList)malloc(sizeof(DuLNode));
    if (!s)
        return OVERFLOW;
    s->data = e;
    s->prior = p; /* 在第i-1个元素之后插入 */
    s->next = p->next;
    p->next->prior = s;
    p->next = s;
    return OK;
}
//元素的删除：
enum Status ListDelete(DuLinkList L, int i, ElemType *e)
{ /* 删除带头结点的双链循环线性表L的第i个元素，i的合法值为1≤i≤表长 */
    DuLinkList p;
    if (i < 1) /* i值不合法 */
        return ERROR;
    p = GetElemP(L, i); /* 在L中确定第i个元素的位置指针p */
    if (!p)             /* p=NULL,即第i个元素不存在 */
        return ERROR;
    *e = p->data;
    p->prior->next = p->next;
    p->next->prior = p->prior;
    free(p);
    return OK;
}
//正序查找
void ListTraverse(DuLinkList L, void (*visit)(ElemType))
{                           /* 由双链循环线性表L的头结点出发，正序对每个数据元素调用函数visit() */
    DuLinkList p = L->next; /* p指向头结点 */
    while (p != L)
    {
        visit(p->data);
        p = p->next;
    }
}
void ListTraverseBack(DuLinkList L, void (*visit)(ElemType))
//逆序查找：
{                            /* 由双链循环线性表L的头结点出发，逆序对每个数据元素调用函数visit()。另加 */
    DuLinkList p = L->prior; /* p指向尾结点 */
    while (p != L)
    {
        visit(p->data);
        p = p->prior;
    }
}

int main()
{
    DuLinkList head;
    int temp;

    head = InitList();
    ListInsert(head, 1, 10);
    EXPECT_EQ_INT(ListLength(head), 1);
    ClearList(head);
    EXPECT_EQ_INT(ListLength(head), 0);
    EXPECT_EQ_INT(ListEmpty(head), TRUE);
    ListInsert(head, 1, 1);
    ListInsert(head, 2, 2);
    ListInsert(head, 3, 3);
    NextElem(head, 2, &temp);
    EXPECT_EQ_INT(temp, 3);
    ListDelete(head, 3, &temp);
    EXPECT_EQ_INT(GetElem(head, 3, &temp), ERROR);
    printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
    return 0;
}

//1.   struct node类型和 struct node *类型的别名 https://blog.csdn.net/qq_43799957/article/details/102013257
//2.   enum 作为返回值https://www.cnblogs.com/idyllcheung/p/12932560.html
//3.   这里类似于封装，DuLinkList作为一个类，不看做一个node