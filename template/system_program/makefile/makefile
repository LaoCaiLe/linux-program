src = $(wildcard ./src/*.c)
obj = $(patsubst ./src/%.c, ./lib/%.o, $(src))

target = test
Args = -g -Wall

inc_PATH = ./inc

$(target):$(obj)
	gcc $(obj) $(Args) -o $(target)  

$(obj):./lib/%.o:./src/%.c
	gcc -c $(Args) $< -o $@ -I$(inc_PATH)

.PTONE: clean
clean:
	-rm -rf $(obj) $(target)
