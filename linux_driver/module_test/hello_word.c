#include <linux/module.h> 
#include <linux/init.h>  
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("baoli");
MODULE_DESCRIPTION("hello world module");

static int __init hello_init(void)
{
	int * p = 0;
	printk(KERN_WARNING "hello world.\n");
	*p = 1;

	return 0;
}

static void __exit hello_exit(void)
{
	printk(KERN_WARNING "hello exit!\n");
}

module_init(hello_init);
module_exit(hello_exit);
