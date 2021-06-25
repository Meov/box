1. 动态库编译
2. 编译可执行文件时查找动态库需要指定动态库的位置，添加环境变量方法如下：
	export LIBRARY_PATH=LIBDIR1:LIBDIR2:$LIBRARY_PATH

3. 编译完可执行文件，运行时需要指定动态库的位置，不然会报错找不着动态库。
	export LD_LIBRARY_PATH=LIBDIR1:LIBDIR2:$LD_LIBRARY_PATH
	其中： LIBDIR1和LIBDIR2是动态库存放的位置.
	也将其添加进 /usr/lib之类的系统位置，也是系统在启动之后指定的动态库存放位置！
