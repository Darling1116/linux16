.PHONY:all
all:test_1 test_2

test_1:test_1.c
	gcc -o $@ $^ 
test_2:test_2.c
	gcc -o $@ $^

.PHONY:clean
clean: 
	rm -f test_1 test_2