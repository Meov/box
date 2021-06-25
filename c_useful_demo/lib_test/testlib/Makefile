.PHONY:clean
libtest.so:test.o
	gcc -o $@ -shared $^    #动态共享库
test.o:test.c
	gcc -o $@ -fPIC -c $^   #和位置没有关系
clean:
	rm libtest.so test.o  

