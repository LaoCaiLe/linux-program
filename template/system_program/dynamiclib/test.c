#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "mymath.h"
/*
1. ����λ���޹ص�.o �ļ�
gcc -c add.c -o add.o -fPIC
ʹ����������������ɵĺ����ͺ�λ���޹أ�����@plt ��ʶ���ȴ���̬��

2.
ʹ�� gcc -shared ������̬��
gcc -shared -o lib ����.so add.o sub.o div.o

3.
�����ִ�г���ʱָ����ʹ�õĶ�̬�⡣-l:ָ������ -L:ָ����·��
gcc test.c -o a.out -l mymath -L ./lib

4. 
���п�ִ�г���./a.out

*/

int main(int argc, char *argv[])
{
	int a=4,b=6;
	printf("%d + %d = %d\r\n",a,b,add(a,b));
	printf("%d - %d = %d\r\n",a,b,sub(a,b));
  	printf("%d / %d = %d\r\n",a,b,div1(a,b));

	return 0;
	
}
