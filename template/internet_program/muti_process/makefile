src = $(wildcard *.c)
inc = ../wrap
target = $(patsubst %.c,%,$(src))
obj = $(patsubst %.c,%.o,$(src))
WRAP = ../wrap/wrap.c

args = -g -Wall 
args2 = -lpthread

ALL:$(target)

%:%.c
	gcc $(args) $< $(WRAP) -I$(inc) -o $@

.PHONY: clean ALL rmtxt

clean:
	-rm -rf $(target)

rmtxt:
	-rm -rf ./*.txt

print:
	@echo target=$(target)
	@echo src=$(src)
	@echo obj=$(obj)
