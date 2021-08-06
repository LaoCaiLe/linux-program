object = test.o test2.o test1.o
target = test
CC 	   = gcc

#src = $(wildcard ./*.c) #当前目录下的所有.c文件
#obj = $(src:%.c=%.o)	#将.c 转化为.o

$(target):$(object)
	$(CC) -o $(target) $(object) 

%.o:%.c
	$(CC) -c -Wall -o $@ $<

.PHONE :clean #伪指令

clean:
	rm -rf $(object)
	rm -rf $(target)

print:
	@echo 傻杯 	

test01:
	cd /home/laocaile;pwd
