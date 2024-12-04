#include <linux/bio.h>

static void transfer()
{
    printk("BLK DEV: recei ve func !!!!!!");
    //blk_mq_complete_reuqest(rq, );
    return;
}

static blk_status_t recieve(struct blk_mq_hw_ctx* hctx, 
                    const struct blk_mq_queue_data* bd)
{
    printk("Request IO message !!!!!!!!");

    struct requet* rq = bd->rq;
    struct my_block_dev* dev = q->queuedata;
    blk_mq_start_request(rq);
    blk_mq_end_request(rq, BLK_STS_OK);
    return BLK_STATUS_OK;
}



MODULE_LICENSE("GPL");