src = $(wildcard *.c)
target = $(patsubst %.c,%,$(src))
obj = $(patsubst %.c,%.o,$(src))

args = -g -Wall

ALL:$(target)

%:%.c
	gcc $(args) $< -o $@

.PHONY: clean ALL rmtxt rmswp

clean:
	-rm -rf $(target)

rmtxt:
	-rm -rf ./*.txt

rmswp:
	-rm -rf ./*.swp
print:
	@echo target=$(target)
	@echo src=$(src)
	@echo obj=$(obj)
