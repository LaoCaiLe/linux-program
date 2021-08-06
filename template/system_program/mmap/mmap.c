#include <stdio.h>
#include <fcntl.h>
#include <error.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>

// void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
// int munmap(void *addr, size_t length); 

int main()
{
	int fd;
	char *p=NULL;
	char *str="hello mmap";
	fd = open("testmmap.txt",O_RDWR,0644);
	if(fd==-1)
	{
		perror("open error");;
		exit(1);
	}

	//拓展文件空间 需要写权限
	/*
	lseek(fd,20,SEEK_END);
	write(fd,'\0',1);
	*/
	ftruncate(fd,20);
	int len = lseek(fd,0,SEEK_END);

	p = mmap(NULL,len,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	//mmap() returns a pointer to the mapped area.
	if(p==MAP_FAILED)
	{
		perror("mmap error");
		exit(1);
	}

	//文件描述符 fd，在 mmap 创建映射区完成即可关闭。后续访问文件，用 地址访问。
	close(fd);
	
	strcpy(p,str);
	printf("test mmap: %s\n",p);

	int ret = munmap(p,len);
	if(ret == -1)
	{
		perror("munmap error");
		exit(1);
	}
	
	return 0;

}

/*
 *
 * ~使用注意事项：
 * 1. 用于创建映射区的文件大小为 0，实际指定非 0 大小创建映射区，出 “总线错误”。
 * 2. 用于创建映射区的文件大小为 0，实际制定 0 大小创建映射区， 出 “无效参数”。
 * 3. 用于创建映射区的文件读写属性为，只读。映射区属性为 读、写。 出 “无效参数”。
 * 4. 创建映射区，需要 read 权限。当访问权限指定为 “共享”MAP_SHARED 时， mmap 的读写权限，应该 <=文件的 open 权限。 只写不行。
 * 5. 文件描述符 fd，在 mmap 创建映射区完成即可关闭。后续访问文件，用 地址访问。
 * 6. offset 必须是 4096 的整数倍。（MMU 映射的最小单位 4k ）
 * 7. 对申请的映射区内存，不能越界访问。
 * 8. munmap 用于释放的 地址，必须是 mmap 申请返回的地址。
 * 9. 映射区访问权限为 “私有”MAP_PRIVATE, 对内存所做的所有修改，只在内存有效，不会反应到物理磁盘上。
 * 10. 映射区访问权限为 “私有”MAP_PRIVATE, 只需要 open 文件时，有读权限，用于创建映射区即可。
 * */
