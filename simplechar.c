#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/poll.h>
#include <linux/cdev.h>
#include <linux/atomic.h>

#define DEV_NAME "devchar_name"
#define CLASS_NAME "devclass_name"
#define BUF_LEN 100

dev_t dev;
struct class* cls;
struct cdev* char_dev_sample;
struct device* dev_create_res;

static int maj_num;
static int ctr = 0;

static int dev_open(struct inode* ind, struct file* flp);
static int dev_rel(struct inode* ind, struct file* flp);
static ssize_t dev_read(struct file*,char __user *buffer , size_t lenght, loff_t* offset);
static ssize_t dev_write(struct file*, const char __user *buffer , size_t lenght, loff_t* offset);


static struct file_operations fops = {
    .release = dev_rel,
    .open = dev_open,
    .read = dev_read,
    .write = dev_write,
};


enum {
    CDEV_NOT_USED = 0,
    CDEV_EXCLUSIVE_OPEN = 1
};

static char message[BUF_LEN];
//static atomic_t is_open = ATOMIC_OPEN(CDEV_NOT_USED);
static atomic_t is_open;

static int dev_open(struct inode* ind, struct file* flp)
{
    //static int counter = 0;
    printk("char device open !!!!!");
    if(atomic_cmpxchg(&is_open, CDEV_NOT_USED, CDEV_EXCLUSIVE_OPEN))
    {
        printk("device is busy !!!!!");
        return -EBUSY;
    }
    ctr++;
    printk("I TOLD YOU %d times gtfo", ctr);
    return 1;
}

static int dev_rel(struct inode* ind, struct file* flp)
{
    printk("char device release !!!!!!");
    atomic_set(&is_open, CDEV_NOT_USED);
    

    return 1;
}

static ssize_t dev_read(struct file*,char __user *buffer , size_t lenght, loff_t* offset)
{
    int bytes_read = 0;
    const char* msg_ptr = message;
    if(!*(msg_ptr + *offset)){
        *offset = 0;
        return 0;
    }

    msg_ptr += *offset;

    while(lenght && *msg_ptr){
        put_user(*(msg_ptr++), buffer++);
        lenght--;
        bytes_read++;
    }

    *offset += bytes_read;

    return bytes_read;
}

static ssize_t dev_write(struct file*, const char __user *, size_t, loff_t*)
{
    printk("Openration is not permitted!!!!");
    return 1;
}



static int __init dev_init(void)
{
    int err;
    maj_num = alloc_chrdev_region(&dev, 0, 1, DEV_NAME);
    if(maj_num < 0)
    {
        printk("Major number less than 0\n");
        return maj_num;
    }

    cdev_init(char_dev_sample, &fops);
    err = cdev_add(char_dev_sample, dev, 1);
    if(err < 0){
        printk("Failed to add char device !!!");
        kfree(char_dev_sample);
        unregister_chrdev_region(dev, 1);
    }

    cls = class_create(CLASS_NAME);
    if(IS_ERR(cls))
    {
        printk("Class create error\n");
        cdev_del(char_dev_sample);
        kfree(char_dev_sample);
        unregister_chrdev_region(dev, 1);
        return -1;
    }
    
    dev_create_res = device_create(cls, NULL, dev, NULL,  DEV_NAME);
    if(IS_ERR(dev_create_res))
    {
        class_destroy(cls);
        cdev_del(char_dev_sample);
        kfree(char_dev_sample);
        unregister_chrdev_region(dev, 1);
        printk("Device create error \n");
        return -1;
    }

    atomic_set(&is_open, CDEV_NOT_USED);
    return 0;
}

static void __exit dev_deinit(void)
{
    device_destroy(cls, MKDEV(maj_num, 0));
    class_destroy(cls);
    cdev_del(char_dev_sample);
    unregister_chrdev_region(dev, 1);
    return;
}

MODULE_LICENSE("GPL");

module_init(dev_init);
module_exit(dev_deinit);
