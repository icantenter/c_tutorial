#include <stdio.h>
#include <stdlib.h>

void _swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

//end 即为array.length()
void heapify(int tree[], int start, int end)
{

    //假设child节点已经是堆(有序), heapify便可以保证根节点最大
    int parent = start;
    int max = parent;
    int child_left = parent * 2 + 1;
    int child_right = parent * 2 + 2;

    if (child_left < end)
    {
        max = (tree[child_left] > tree[max] ? child_left : max);
    }
    if (child_right < end)
    {
        max = (tree[child_right] > tree[max] ? child_right : max);
    }
    if (max == parent)
    {
        return;
    }
    _swap(tree + max, tree + parent);
    heapify(tree, max, end);
}
void build_heap(int tree[], size_t size)
{
    //叶子节点的(index - 1 ) / 2会向下取整
    //得到最后一个非叶子节点的index
    int last_node = size -1;
    int parent = (last_node - 1) / 2;
    //最后一个非叶子节点，之上的每一个节点都可以通过heapify组成新堆
    for (int i = parent; i >= 0; i--)
    {
        heapify(tree, i, size);
    }
}

void heap_sort(int tree[], size_t size)
{
    build_heap(tree, size);
    //交换根节点和最后一个节点，再pop最后一个节点
    //调用heapify后，堆会再一次有序
    for (int i = size - 1; i >= 0; i--)
    {
        _swap(tree + i, tree + 0);
        heapify(tree, 0, i);
    }
    
}

int main()
{
    int tree[] = {2, 5, 3, 1, 10, 4};
    int n = 6;
    heap_sort(tree, n);

    for (int i = 0; i < n; i++)
    {
        printf("%d\n", tree[i]);
    }
    return 0;
}