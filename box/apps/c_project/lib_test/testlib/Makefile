TARGET_LIBS := libtest.so libtest.a

.PHONY:clean
.PHONY:share_lib
.PHONY:static_lib

all:$(TARGET_LIBS)
	@echo build $(TARGET_LIBS) done

share_lib:libtest.so
	@echo build share_lib done

static_lib:libtest.a
	@echo build static_lib done

libtest.a:test_static.o
	ar rcs $@ $^

libtest.so:test_shared.o
	gcc -o $@ -shared $^

test_static.o:test_static.c
	gcc -o $@ -fPIC -c $^
test_shared.o:test_shared.c
	gcc -o $@ -fPIC -c $^
clean:
	rm $(TARGET_LIBS) *.o  



