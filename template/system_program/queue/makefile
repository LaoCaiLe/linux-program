src = $(wildcard *.c)
target = $(patsubst %.c,%,$(src))
obj = $(patsubst %.c,%.o,$(src))

args = -g -Wall -lpthread

ALL:$(target)

%:%.c
	gcc $(args) $< -o $@ -lrt

.PHONY: clean ALL rmtxt

clean:
	-rm -rf $(target)

rmtxt:
	-rm -rf ./*.txt

print:
	@echo target=$(target)
	@echo src=$(src)
	@echo obj=$(obj)
