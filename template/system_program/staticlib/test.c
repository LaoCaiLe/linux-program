#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "mymath.h"

/*
��̬������ָ��
ar rcs libmylib.a file1.o

��̬���ʹ�ã�
gcc test.c lib ����.a -o a.out
*/

int main(int argc, char *argv[])
{
	int a=4,b=6;
	printf("%d + %d = %d\r\n",a,b,add(a,b));
	printf("%d - %d = %d\r\n",a,b,sub(a,b));
  	printf("%d / %d = %d\r\n",a,b,div1(a,b));

	return 0;
	
}
