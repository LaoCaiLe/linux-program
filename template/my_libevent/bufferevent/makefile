src = $(wildcard *.c)
inc = ./
target = $(patsubst %.c,%,$(src))
obj = $(patsubst %.c,%.o,$(src))


args = -g -Wall 
args2 = -levent

ALL:$(target)

%:%.c
	gcc $(args) $< $(args2) -I$(inc) -o $@

.PHONY: clean ALL rmtxt

clean:
	-rm -rf $(target)