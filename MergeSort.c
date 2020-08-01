#include <stdlib.h>
#include <stdio.h>

void Merge(int sourceArr[], int tempArr[], int startIndex, int midIndex, int endIndex)
{
    //i开始时指向第一个，j 开始指向i终点后一个
    int i = startIndex, j = midIndex + 1, k = startIndex;

    while (i != midIndex + 1 && j != endIndex + 1)
    {
        //把两组中小的放进去，并移动pointer或index
        if (sourceArr[i] > sourceArr[j])
            tempArr[k++] = sourceArr[j++];
        else
            tempArr[k++] = sourceArr[i++];
    }
    //将i, j中没有到达分组末尾的那组剩余数全部存入temp
    //下面两个while循环，只会有一个起作用
    while (i != midIndex + 1)
        tempArr[k++] = sourceArr[i++];
    while (j != endIndex + 1)
        tempArr[k++] = sourceArr[j++];
    //将排好序的数组复制回来
    for (i = startIndex; i <= endIndex; i++)
        sourceArr[i] = tempArr[i];
}

//内部使用递归
void MergeSort(int sourceArr[], int tempArr[], int startIndex, int endIndex)
{
    int midIndex;
    if (startIndex == endIndex) //递归出口
    {
        return ;
    }
    midIndex = startIndex + (endIndex - startIndex) / 2; //等价于((endIndex + startIndex) / 2, 但可以避免溢出int
    MergeSort(sourceArr, tempArr, startIndex, midIndex);
    MergeSort(sourceArr, tempArr, midIndex + 1, endIndex);
    Merge(sourceArr, tempArr, startIndex, midIndex, endIndex);
}

int main(int argc, char *argv[])
{
    int a[8] = {50, 10, 20, 30, 70, 40, 80, 60};
    int i, b[8];
    MergeSort(a, b, 0, 7);
    for (i = 0; i < 8; i++)
        printf("%d ", a[i]);
    printf("\n");
    return 0;
}