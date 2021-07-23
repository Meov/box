1. 动态库编译
2. 编译可执行文件时查找动态库需要指定动态库的位置，添加环境变量方法如下：
	export LIBRARY_PATH=LIBDIR1:LIBDIR2:$LIBRARY_PATH
	对于本例：
	export LIBRARY_PATH=~/linux_learning_notes/c_useful_demo/lib_test/testlib:$LIBRARY_PATH

	编译应用时，应使用 -l 使用的动态库的名称. 如： -ltest
	设定好库的路径时，使用gcc编译：
		gcc -o main main.c -ltest
	或者在编译的时候，制定库的路径：
		gcc -o main main.c -L./testlib -ltest


3. 编译完可执行文件，运行时需要指定动态库链接的位置，不然会报错找不着动态库。
	export LD_LIBRARY_PATH=LIBDIR1:LIBDIR2:$LD_LIBRARY_PATH
	其中： LIBDIR1和LIBDIR2是动态库存放的位置.
	也将其添加进 /usr/lib之类的系统位置，也是系统在启动之后指定的动态库存放位置！

	对于本例：
	export LD_LIBRARY_PATH=~/linux_learning_notes/c_useful_demo/lib_test/testlib:$LD_LIBRARY_PATH
