src = $(wildcard *.c)
inc = ./
target = $(patsubst %.c,%,$(src))
obj = $(patsubst %.c,%.o,$(src))


args = -g -Wall 
args2 = -lpthread

ALL:$(target)

%:%.c
	gcc $(args) $< $(args2) -I$(inc) -o $@

.PHONY: clean ALL rmtxt

clean:
	-rm -rf $(target)

rmtxt:
	-rm -rf ./*.txt

print:
	@echo target=$(target)
	@echo src=$(src)
	@echo obj=$(obj)
