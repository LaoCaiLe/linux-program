src = $(wildcard *.c)
target = $(patsubst %.c,%,$(src))
obj = $(patsubst %.c,%.o,$(src))

args = -g -Wall

ALL:$(target)

%:%.c
	gcc $(args) $< -o $@

.PHONY: clean ALL

clean:
	rm -rf $(target)

print:
	@echo target=$(target)
	@echo src=$(src)
	@echo obj=$(obj)
