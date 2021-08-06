#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
//#include "mymath.h"
int sub(int,int);
int add(int,int);
float div1(float,float);

int main(int argc, char *argv[])
{
	int a=4,b=6;
	printf("%d + %d = %d\r\n",a,b,add(a,b));
	printf("%d - %d = %d\r\n",a,b,sub(a,b));
  printf("%d / %d = %f\r\n",a,b,div1((float)a,(float)b));

	return 0;
	
}
