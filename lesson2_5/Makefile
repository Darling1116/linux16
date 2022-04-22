.PHONY:all
all:main 

main:main.c
	gcc -o $@ $^  -I./mylib/include -L./mylib/lib -lmymath -static

.PHONY:clean
clean: 
	rm -f main 