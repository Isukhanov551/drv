#include <linux/cdev.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/poll.h>

static int device_open(struct inode*, struct file *);
static  int device_release(struct inode*, struct file *);
static ssize_t device_read(struct file* , char* __user, size_t, loff_t*);
static ssize_t device_write(struct file*, const char __user *, size_t, loff_t *);


#define SUCCESS 0
#define DEVICE_NAME "some_char_device"
#define BUFF_LEN 80

static int major;

enum{   
    CDEV_NOT_USED = 0,
    CDEV_EXCL_OPEN = 1,
};


static atomic_t already_open = ATOMIC_INIT(CDEV_NOT_USED);

static char msg[BUFF_LEN];

static struct class *cls;


static struct file_operations chardev_ops  = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release,
};

static int __init chardev_init(void)
{
    major = register_chrdev(0, DEVICE_NAME, &chardev_ops);
    if(major < 0)
    {
        pr_alert("Device register failed with %d\n", major);
        return major;
    }

    pr_info("I was assgned major number with %d\n", major);
    cls = class_create(DEVICE_NAME);
    device_create(cls, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);

    pr_info("Device created on /dev/%s", DEVICE_NAME);

    return SUCCESS;
}

static void __exit chardev_exit(void)
{
    device_destroy(cls, MKDEV(major, 0));
    class_destroy(cls);

    unregister_chrdev(major, DEVICE_NAME);
    return;
}

// Main methods


static int device_open(struct inode *inode, struct file *file)
{
    static int counter = 0;

    if(atomic_cmpxchg(&already_open, CDEV_NOT_USED, CDEV_EXCL_OPEN))
    {
        return -EBUSY;
    }
    sprintf(msg, "I already told you &d times Hello world\n", counter++);
}

static int device_release(struct inode *inode, struct file *file)
{
    atomic_set(&already_open, CDEV_NOT_USED);

    module_put(THIS_MODULE);
    return SUCCESS;
}

static ssize_t device_read(struct file* flip, char __user * buffer, 
                            size_t lenght, loff_t* offset)
{
    int bytes_read = 0;
    const char* msg_ptr = msg;
    if(! *(msg_ptr + *offset))
    {
        *offset = 0;
        return 0;
    }
    msg_ptr += *offset;

    while(lenght && *msg_ptr)
    {
        put_user(*(msg_ptr++), buffer++);
        lenght--;
        bytes_read++;
    }
    
    *offset += bytes_read;
        
    return bytes_read;

}

static ssize_t device_write(struct file* flip, char __user * buffer, 
                            size_t lenght, loff_t* offset)
{
    pr_alert("Sorry, this operation is not supported. \n");
    return -EINVAL;

}


module_init(chardev_init);
module_exit(chardev_exit);

MODULE_LICENSE("BSD2");