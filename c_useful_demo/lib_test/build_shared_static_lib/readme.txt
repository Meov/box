NOTE:

gitignore 会忽略 .o .a 以及.so文件，因此在此处将其打包

使用： 
	tar -xvf libs_test_case.tar.gz 
即可使用


this is just a test case for building a target needing both shard and static libs


if says"libbtest_shared.so: cannot open shared object file..."
*use:
	export LD_LIBRARY_PATH=LIBDIR1:LIBDIR2:$LD_LIBRARY_PATH

to add libbtest_shared.so to your "linking searching path"


build the test cases "main.o libtest_shared.so libtest_static.a"
	Referce  the  readme of "sdk_build_sample" in this git to how to build libs
	Also you can use "make test_static" in ../sdk_build_sample path. its same as this case :)

