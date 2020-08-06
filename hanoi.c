#include <stdio.h>
#include <stdlib.h>
void hanoi(int n, char A, char B, char C)；
int main()
{
    hanoi(2, 'A', 'B', 'C');
    return 0;
}

//先考虑递归出口和最小问题的解决
//再实现大问题的拆分(如果可以拆分)
void hanoi(int n, char A, char B, char C)
{
    if (n == 1)
    {
        printf("%c -> %c\n", A, C);
    }
    else
    {
        hanoi(n - 1, A, C, B);
        printf("%c -> %c\n", A, C);
        hanoi(n - 1, B, A, C);
    }
}
