
examples build:
<1 app
	reference hello_world local.Mk
	using "make hello_world" to compile a simple axample

<2 shared libs:
        reference libtest_shared local.Mk
        using "make libtest_shared" to compile a shared lib

   apps using shared libs:
        reference test_shared local.Mk
        1. using "make libtest_shared" to compile a shared lib FIRST!
        2. using "make test_shared" to compile a test app using share lib

<3 static libs:
        reference libtest_static local.Mk
        using "make libtest_static" to compile a static lib

   apps using static libs:
        reference test_static local.Mk
        1. using "make libtest_static" to compile a static lib FIRST!
        2. using "make test_static" to compile a test app using static lib

build your own apps:
	reference samples in ./apps/hello_world
		1. write local.Mk
		2. add your app name into apps/app.Mk  ->  "export APP_MODULES += you_app_name"
		3. using "make your_app_name"
	
	uild app with libs
		reference "pps/test/" both shared and static libs dependent
	
build your own libraries:
	reference samples in ./apps/libs
