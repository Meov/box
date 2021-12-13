# 1. 介绍
box_project：
 2021年10月份，对本工程进行了结构的调整以及更新，一个仿照Android/build-root编译框架，搭建管理linux 应用层APP以及内核部分的编译管理工具。
 目前的编译设计思路分两部分，一个是跟随各子项目的Makefile，可单独编译子项目, 一个是使用local.Mk统一管理的大项目，可一次性编译所有的项目，也可单独编译某一个小项目。

# 2. 使用方法

**编译**：
 cd box/box
 使用 "make app" 编译所有的应用程序 ：） 编译这部分没有编译链的限制。
 使用 "make bsp" 编译板级文件 （如果你要构建一个嵌入式工程，需要指定编译链的位置）

编译完成之后，生成物在 ./out 目录下
应用程序： ./out/app_out/apps_target/app
内核、设备树以及文件系统： out/bsp_out/bsp_target/

**q1**: 如何指定编译工具链?
编辑Makefile 找到：
```Makefile
"BUILD_TOOL_CHAINS := <填上你自己的编译链路径>"
```
如我的：
```Makefile
BUILD_TOOL_CHAINS :=$(TOP_DIR)/build_tools/build_chains/gcc-linaro-7.3.1-2018.05-x86_64_aarch64-linux-gnu
```
我的编译链就放在 "box/box/build_tools/build_chains/gcc-linaro-7.3.1-2018.05-x86_64_aarch64-linux-gnu" 路径下

**q2**: 如何添加自己的源码文件？
cpp 仿照 "box/box/apps/cpp_test"中的进行添加；
c语言文件，仿照"box/box/apps/c_useful_demo"中的demo进行添加；
kernel\rootfs\uboot 仿照 box/box/bsp下的进行添加；

对于box/box/apps下新添加的文件，注意将新增的模块加进app.Mk
这是目前已有的：

#c项目
export APP_MODULES += hello_world
#c++项目
export APP_MODULES += rsa_verify

在该文件添加之后，方可在box/box中使用: make "your app module name" 进行编译。


对于box/box/bsp下新添加的文件，注意将新增的模块加进bsp.Mk
这是目前已有的：
```Shell
#kernel config and path
#use TARGET_KERNEL_DIR to decide the target kernel to build
#ifeq ($(PLATFORM),ARM64)
export TARGET_KERNEL_DIR := $(TOP_DIR)/bsp/kernel
export TARGET_KERNEL_CONFIG := tegra_defconfig
#uboot config and path
export TARGET_UBOOT_DIR := $(TOP_DIR)/bsp/uboot
export TARGET_UBOOT_CONFIG := p2371-2180_defconfig

export BSP_MODULES += kernel
export BSP_MODULES += uboot
#endif
```
在该文件添加之后，方可在box/box中使用: make kernel / make uboot /make rootfs 分别进行编译。
