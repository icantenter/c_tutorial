/*=========================
 * 队列的链式存储结构（链队）
 ==========================*/

#include <stdio.h>
#include <stdlib.h>     // 提供malloc、realloc、free、exit原型
#include "Status.h"     //**▲01 绪论**//
#include "unit_test.h"

/* 链队元素类型定义 */
typedef int QElemType;

// 队列元素结构
typedef struct QNode {
    QElemType data;
    struct QNode* next;
} QNode, * QueuePtr;

// 队列结构
typedef struct {
    QueuePtr front;     // 队头指针
    QueuePtr rear;      // 队尾指针
} LinkQueue;            // 队列的链式存储表示


/*
 * 初始化
 *
 * 构造一个空的链队。
 * 初始化成功则返回OK，否则返回ERROR。
 *
 *【注】
 * 这里的队列带有头结点
 */
Status InitQueue(LinkQueue* Q);

/*
 * 销毁(结构)
 *
 * 释放链队所占内存。
 */
Status DestroyQueue(LinkQueue* Q);

/*
 * 置空(内容)
 *
 * 这里需要释放链队中非头结点处的空间。
 */
Status ClearQueue(LinkQueue* Q);

/*
 * 判空
 *
 * 判断链队中是否包含有效数据。
 *
 * 返回值：
 * TRUE : 链队为空
 * FALSE: 链队不为空
 */
Status QueueEmpty(LinkQueue Q);

/*
 * 计数
 *
 * 返回链队包含的有效元素的数量。
 */
int QueueLength(LinkQueue Q);

/*
 * 取值
 *
 * 获取队头元素，将其存储到e中。
 * 如果可以找到，返回OK，否则，返回ERROR。
 */
Status GetHead(LinkQueue Q, QElemType* e);

/*
 * 入队
 *
 * 将元素e添加到队列尾部。
 */
Status EnQueue(LinkQueue* Q, QElemType e);

/*
 * 出队
 *
 * 移除队列头部的元素，将其存储到e中。
 */
Status DeQueue(LinkQueue* Q, QElemType* e);

/*
 * 遍历
 *
 * 用visit函数访问队列Q
 */
Status QueueTraverse(LinkQueue Q, void(Visit)(QElemType));

                    //**▲03 栈和队列**//

/*
 * 初始化
 *
 * 构造一个空的链队。
 * 初始化成功则返回OK，否则返回ERROR。
 *
 *【注】
 * 这里的队列带有头结点
 */
Status InitQueue(LinkQueue* Q) {
    if(Q == NULL) {
        return ERROR;
    }
    
    (*Q).front = (*Q).rear = (QueuePtr) malloc(sizeof(QNode));//注释1
    if(!(*Q).front) {
        exit(OVERFLOW);
    }
    
    (*Q).front->next = NULL;
    
    return OK;
}

/*
 * 销毁(结构)
 *
 * 释放链队所占内存。
 */
Status DestroyQueue(LinkQueue* Q) {
    if(Q == NULL) {
        return ERROR;
    }
    
    while((*Q).front) {
        (*Q).rear = (*Q).front->next;
        free((*Q).front);
        (*Q).front = (*Q).rear;
    }
    
    return OK;
}

/*
 * 置空(内容)
 *
 * 这里需要释放链队中非头结点处的空间。
 */
Status ClearQueue(LinkQueue* Q) {
    if(Q == NULL) {
        return ERROR;
    }
    
    (*Q).rear = (*Q).front->next;
    
    while((*Q).rear) {
        (*Q).front->next = (*Q).rear->next;
        free((*Q).rear);
        (*Q).rear = (*Q).front->next;
    }
    
    (*Q).rear = (*Q).front;
    
    return OK;
}

/*
 * 判空
 *
 * 判断链队中是否包含有效数据。
 *
 * 返回值：
 * TRUE : 链队为空
 * FALSE: 链队不为空
 */
Status QueueEmpty(LinkQueue Q) {
    if(Q.front == Q.rear) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/*
 * 计数
 *
 * 返回链队包含的有效元素的数量。
 */
int QueueLength(LinkQueue Q) {
    int count = 0;
    QueuePtr p = Q.front;
    
    while(p != Q.rear) {
        count++;
        p = p->next;
    }
    
    return count;
}

/*
 * 取值
 *
 * 获取队头元素，将其存储到e中。
 * 如果可以找到，返回OK，否则，返回ERROR。
 */
Status GetHead(LinkQueue Q, QElemType* e) {
    QueuePtr p;
    
    if(Q.front == NULL || Q.front == Q.rear) {
        return ERROR;
    }
    
    p = Q.front->next;
    *e = p->data;
    
    return OK;
}

/*
 * 入队
 *
 * 将元素e添加到队列尾部。
 */
Status EnQueue(LinkQueue* Q, QElemType e) {
    QueuePtr p;
    
    if(Q == NULL || (*Q).front == NULL) {
        return ERROR;
    }
    
    p = (QueuePtr) malloc(sizeof(QNode));
    if(!p) {
        exit(OVERFLOW);
    }
    
    p->data = e;
    p->next = NULL;
    
    (*Q).rear->next = p;
    (*Q).rear = p;
    
    return OK;
}

/*
 * 出队
 *
 * 移除队列头部的元素，将其存储到e中。
 */
Status DeQueue(LinkQueue* Q, QElemType* e) {
    QueuePtr p;
    
    if(Q == NULL || (*Q).front == NULL || (*Q).front == (*Q).rear) {
        return ERROR;
    }
    
    p = (*Q).front->next;
    *e = p->data;
    
    (*Q).front->next = p->next;
    if((*Q).rear == p) {
        (*Q).rear = (*Q).front;
    }
    
    free(p);
    
    return OK;
}

/*
 * 遍历
 *
 * 用visit函数访问队列Q
 */
Status QueueTraverse(LinkQueue Q, void (Visit)(QElemType)) {
    QueuePtr p;
    
    if(Q.front == NULL) {
        return ERROR;
    }
    
    p = Q.front->next;
    
    while(p != NULL) {
        Visit(p->data);
        p = p->next;
    }
    
    
    return OK;
}

//1. 这里类似于封装，LinkQueue作为一个类，申请了一个node的空间用于访问队列元素，但不存放值