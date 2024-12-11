#include <linux/kernel.h>
//#include <linux/blkdev.h>
#include <linux/blk-mq.h>
#include "request.h"

static void request_data(void)
{
    printk("BLK DEV: request  func !!!!!!");
    return;
}

static void recieve_data(void){
    printk("BLC DEV: recieve func");
}

blk_status_t request_handler(struct blk_mq_hw_ctx* hctx, struct blk_mq_queue_data* bd)
{
    printk("Request IO message !!!!!!!!");

    struct request* rq = bd->rq;
    //struct my_block_dev* dev = q->queuedata;
    request_data();
    recieve_data();
    blk_mq_start_request(rq);
    if(rq_data_dir(rq) == READ)
    {
        recieve_data();
    }
    else if(rq_data_dir(rq) == WRITE)
    {
        request_data();
    }
    blk_mq_end_request(rq, BLK_STS_OK);

    return BLK_STS_OK;
}



MODULE_LICENSE("GPL");