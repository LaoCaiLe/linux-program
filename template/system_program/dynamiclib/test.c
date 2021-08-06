#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "mymath.h"
/*
1. 生成位置无关的.o 文件
gcc -c add.c -o add.o -fPIC
使用这个参数过后，生成的函数就和位置无关，挂上@plt 标识，等待动态绑定

2.
使用 gcc -shared 制作动态库
gcc -shared -o lib 库名.so add.o sub.o div.o

3.
编译可执行程序时指定所使用的动态库。-l:指定库名 -L:指定库路径
gcc test.c -o a.out -l mymath -L ./lib

4. 
运行可执行程序./a.out

*/

int main(int argc, char *argv[])
{
	int a=4,b=6;
	printf("%d + %d = %d\r\n",a,b,add(a,b));
	printf("%d - %d = %d\r\n",a,b,sub(a,b));
  	printf("%d / %d = %d\r\n",a,b,div1(a,b));

	return 0;
	
}
