#include <linux/fs.h>
#include <linux/blkdev.h>
#include <linux/genhd.h>

#define BL_DEVICE_MAJOR 240
#define BL_DEVICE_NAME "sample_blk_dev"
#define MY_BLOCK_MINORS 1

static struct block_dev_info {
    spinlock_t lock;
    struct request_queue *queue;
    struct blk_mg_tag_set tag_set;
    struct dendisk *gd;
} blk_info;

//NEED TO BE COMPLETED LATER !!!!!!!!!!!!!!!
// struct block_device_operations blc_ops = {

// };

static blk_info *info;


void create_block_device(blk_info* dev_struct)
{
    int status;

    //Create TAG SET for queue reqeusts order
    err = blk_mq_alloc_tag_set(info->tag_set);


    //Create queue
    dev->queue = blk_mq_init_queue(dev->tag_set);
    if(IS_ERR(dev->queue))
    {
        printk("BLK DEV: Queue creating error");
        return -1
    }

    blk_queue_logical_block_size(dev->queue, KERNEL_SECTOR_SIZE);




    
    //Allocate disk 
    info->gd = alloc_disk(MY_BLOCK_MINORS);





    //Register block device
    status = register_blkdev(BL_DEVICE_MAJOR, BL_DEVICE_NAME);
    if(status < 0 )
    {
        printk("Block dev: device register error !!!");
        return -EBUSY;
    }

    
    //Add block device disk to the system
    add_disk(info->gd);

    register_blkdev(BL_DEVICE_MAJOR, BL_DEVICE_NAME);
}


static void delete_block_device(blk_info* dev_struct)
{
    if(info->gd)
    {
        del_gendisk(info->gd);
    }


    blk_cleanup_queue(dev->queue);
    blk_free_tag_set(dev->tag_set);

    unregister_blkdev(BL_DEVICE_MAJOR, BL_DEVICE_NAME);
    return;
}

static int __init block_init(void)
{
    create_block_device(info);
}

static int __exit block_deinit()
{
    delete_block_device(info);
}

module_init(block_init);
module_exit(block_deinit);