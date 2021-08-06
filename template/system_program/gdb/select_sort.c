
#include <stdio.h>

void sort(int *arr)
{
    int i, j;  //循环变量
    int MinIndex;  //保存最小的值的下标
    int buf;
    //存放数组a中元素的个数
    
    for (i=0; i<11; ++i)  //n个数比较n-1轮
    {
        MinIndex = i;
        for (j=i+1; j<12; ++j)  //每轮比较n-1-i次, 找本轮最小数的下标
        {
            if (arr[MinIndex] > arr[j])
            {
                MinIndex = j;  //保存小的数的下标
            }
        }
        if (MinIndex != i)  /*找到最小数之后如果它的下标不是i则说明它不在最左边, 则互换位置*/
        {
            buf = arr[MinIndex];
            arr[MinIndex] = arr[i];
            arr[i] = buf;
        }
    }
}

void vector_print(int *arr)
{
    int i;  
    int n = sizeof(arr) / sizeof(arr[0]);  //存放数组a中元素的个数
    for (i=0; i<12; ++i)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main(void)
{
    
    int a[] = {5, 4, 3, 7, 4, 2, 5, 4, 9, 1, 8, 6};
    printf("----before sort----:\n");
    vector_print(a);

    sort(a);
    printf("----after sort-----:\n");
    vector_print(a);
   
    return 0;
}
