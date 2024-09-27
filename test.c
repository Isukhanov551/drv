#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/stat.h>

MODULE_LICENSE("BSD");
MODULE_AUTHOR("MOLODOY FACE");
MODULE_DESCRIPTION("DRIVER FOR URONIT ZAPAD. ZA POBEDY");

static short param1 = 0;
static int param2  = 2;
static long int param3 = 3;
//static char* my_str = "sstr";
//static int arr_argc = 0;

module_param(param1, short, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(param1, "Custom short insteger");
module_param(param2, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(param2, "Custon int integer");
// module_param(my_str, charp, 0000);
// MODULE_PARM_DESC(param2, "Custon char string");
module_param(param3, long,  S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(param3, "Custon long int");
// module_param(arr_argc, int, 0);
// MODULE_PARM_DESC(param2, "Array arguments");



static int __init hello_init(void)
{
    printk("HEllo drv porj !!!");
    printk("My short is  %hd", param1);
    printk("My long ins is  %ld", param3);
    printk("My int is %d", param2);
    return 0;
}
static void __exit hello_exit(void)
{
    printk("EXIT FUNC !!!!!");
}

module_init(hello_init);
module_exit(hello_exit);
