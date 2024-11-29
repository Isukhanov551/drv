#include <linux/fs.h>
#include <linux/blkdev.h>
#include <linux/blk-mq.h>

#define BL_DEVICE_MAJOR 240
#define BL_DEVICE_NAME "sample_blk_dev"
#define MY_BLOCK_MINORS 1


spinlock_t lock;
struct request_queue *queue;
struct blk_mq_tag_set tag_set;
struct gendisk *gd;


//NEED TO BE COMPLETED LATER !!!!!!!!!!!!!!!
// struct block_device_operations blc_ops = {

// };

//static blk_info *info;


int create_block_device(void)
{
    int err;

    //Create TAG SET for queue reqeusts order
    err = blk_mq_alloc_tag_set(&tag_set);


    //Create queue
    queue = blk_mq_init_queue(&tag_set);
    if(IS_ERR(queue))
    {
        printk("BLK DEV: Queue creating error");
        return 1;
    }

    blk_queue_logical_block_size(queue, KERNEL_IMAGE_SIZE);




    
    //Allocate disk 
    gd = blk_alloc_disk(MY_BLOCK_MINORS);





    //Register block device
    err = register_blkdev(BL_DEVICE_MAJOR, BL_DEVICE_NAME);
    if(err < 0 )
    {
        printk("Block dev: device register error !!!");
        return EBUSY;
    }

    
    //Add block device disk to the system
    err = add_disk(gd);
    if(err < 0)
    {
        printk("BLK DEV: Failed to allocate disk !!!");
        return -1;
    }

    register_blkdev(BL_DEVICE_MAJOR, BL_DEVICE_NAME);
    return 0;
}


static void delete_block_device(void)
{
    if(gd)
    {
        del_gendisk(gd);
    }


    blk_mq_free_tag_set(&tag_set);
    blk_put_queue(queue);

    unregister_blkdev(BL_DEVICE_MAJOR, BL_DEVICE_NAME);
    return;
}

static int __init block_init(void)
{
    create_block_device();
    return 0;
}

static void __exit block_deinit(void)
{
    delete_block_device();
}

module_init(block_init);
module_exit(block_deinit);


MODULE_LICENSE("GPL");