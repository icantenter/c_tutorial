#include <stdio.h>
#include <stdlib.h>
#define STACK_INIT_SIZE 1000
#define STACKINCREMENT 10

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

#define EXPECT_EQ_BASE(equality, expect, actual, format)                                                           \
    do                                                                                                             \
    {                                                                                                              \
        test_count++;                                                                                              \
        if (equality)                                                                                              \
            test_pass++;                                                                                           \
        else                                                                                                       \
        {                                                                                                          \
            fprintf(stderr, "%s:%d: expect: " format " actual: " format "\n", __FILE__, __LINE__, expect, actual); \
            main_ret = 1;                                                                                          \
        }                                                                                                          \
    } while (0)

#define EXPECT_EQ_INT(expect, actual) EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%d")

enum Status
{
    OK = 1,
    TRUE = 2,
    FALSE = 0,
    ERROR = 0,
    OVERFLOW = 0,

};

typedef int SElemType;
struct Stack
{
    SElemType *base;
    SElemType *top;
    size_t size;
};
typedef struct Stack Stack;



enum Status StackTraverse(Stack *MyStack, enum Status (*visit)(SElemType))
{
    return OK;
}

int StackLength(Stack* MyStack)
{
    return MyStack->top - MyStack->base;
}

Stack *InitStack(size_t cap)
{ //栈初始化
    Stack *MyStack = (Stack *)malloc(sizeof(Stack));
    MyStack->base = (SElemType *)malloc(sizeof(SElemType) * cap);
    MyStack->top = MyStack->base;
    MyStack->size = cap;
   return MyStack;
}

enum Status StackEmpty(Stack *MyStack)
{
    if (MyStack->top == MyStack->base)
    {
        return TRUE;
    }
    return FALSE;
}

enum Status push(Stack *MyStack, const SElemType e)
{
    if (MyStack->top - MyStack->base == MyStack->size)
    {
        MyStack->base = (SElemType *)realloc(MyStack->base
         , (MyStack->size + STACKINCREMENT) * sizeof(SElemType));
        if (!MyStack->base)
        {
            exit(OVERFLOW);
        }
        MyStack->size += STACKINCREMENT;
    }
    *MyStack->top++ = e;
    return OK;
}

enum Status top(Stack *MyStack, SElemType *e)
{
    if (MyStack->base == MyStack->top)
        return ERROR;
    *e = *(MyStack->top - 1);
    return OK;
}

enum Status pop(Stack *MyStack, SElemType *e)
{
    if (MyStack->base == MyStack->top)
        return ERROR;
    MyStack->top--;
    *e = *MyStack->top;
    return OK;
}

enum Status ClearStack(Stack *MyStack)
{
    MyStack->top = MyStack->base;
    return OK;
}

enum Status DestroyStack(Stack *MyStack)
{
    free(MyStack->base);
    free(MyStack);
    return OK;
}

int main()
{
    int temp;
    Stack *MyStack = InitStack(10);
    EXPECT_EQ_INT(StackLength(MyStack), 0);
    push(MyStack, 1);
    top(MyStack, &temp);
    EXPECT_EQ_INT(temp, 1);
    ClearStack(MyStack);
    EXPECT_EQ_INT(StackLength(MyStack), 0);
    printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
    return 0;
}