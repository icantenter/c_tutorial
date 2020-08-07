#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void move_prefix_table(int prefix[], size_t size);
void prefix_table(char pattern[], int prefix[], size_t size);
void kmp_search(char text[], char pattern[]);
int main()
{
    char pattern[] = "ABABCABAA";
    char text[] = "AABCABABCABAABABCABAA";
    kmp_search(text, pattern);
    
    return 0;
}


// 0  1 2 3 4 (index)
//      j<- j
// A  B A B # (prefix_table)
// -1 0 0 1 2
//在#处失配，那么text串和pattern字符串的前边4位就是相同的。又因为pattern字符串的前4位，
//它的前2位前缀和后2位后缀是相同的，所以我们推知text字符串j之前的2位和模式字符串开头的2位是相同的,
//那这部分就不用再比较了。
void kmp_search(char text[], char pattern[])
{
    size_t n = strlen(pattern);
    size_t m = strlen(text);
    int *prefix = (int *)malloc(sizeof(int) * n);
    prefix_table(pattern, prefix, n);
    move_prefix_table(prefix, n);

    //len(text) = m
    //len(pattern) = n

    int i = 0, j = 0;
    while (i < m)
    {
        //完全匹配
        if (j == n - 1 && pattern[j] == text[i])
        {
            printf("found at %d\n", i - j);
            j = prefix[j];
        }
        //当前匹配
        if (text[i] == pattern[j])
        {
            i++;
            j++;
        }
        //不匹配
        else
        {
            //根据prefix_table前滚
            j = prefix[j];
            if (j == -1)
            {
                i++;
                j = 0;
            }
        }
    }
}

//prefix_table向右一格，-1补位
void move_prefix_table(int prefix[], size_t size)
{
    for (int i = size - 1; i > 0; i--)
    {
        prefix[i] = prefix[i - 1];
    }
    prefix[0] = -1;
}

//"前缀"指除了最后一个字符以外，一个字符串的全部头部组合；"后缀"指除了第一个字符以外，一个字符串的全部尾部组合
//生成前缀表
void prefix_table(char pattern[], int prefix[], size_t size)
{
    prefix[0] = 0;
    int len = 0;
    int i = 1;
    while (i < size)
    {   
        //prefix_table存放的是最长公共前后缀。
        //所以也用到了匹配。
        if (pattern[i] == pattern[len])
        {
            len++;
            prefix[i] = len;
            i++;
        }
        else
        {
            //和最长前缀匹配不上。就利用已经求出的prefix_table进行匹配。
            //直到匹配成功，或到prefix_table尽头
            if (len > 0)
            {
                //prefix_table整体还没后移,所以是len - 1
                len = prefix[len - 1];
            }
            else
            {
                prefix[i] = len;
                i++;
            }
        }
    }
}