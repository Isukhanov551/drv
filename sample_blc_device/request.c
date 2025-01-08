#include <linux/kernel.h>
//#include <linux/blkdev.h>
#include <linux/blk-mq.h>
#include "request.h"


char* blc_buffer = NULL;

static void block_read(sector_t sector, char* buffer, unsigned long bytes)
{
    printk("BLK DEV: request  func !!!!!!");

    return;
}

static void block_write(sector_t sector, char* buffer, unsigned long bytes){
    printk("BLC DEV: recieve func");
    return;
}

blk_status_t request_handler(struct blk_mq_hw_ctx* hctx, struct blk_mq_queue_data* bd)
{
    printk("Request IO message !!!!!!!!");

    struct request* rq = bd->rq;
    //struct my_block_dev* dev = rq->queuedata;
    //struct bio_vec bvec;
    sector_t sc = blk_rq_pos(rq);
    unsigned long nbts = blk_rq_cur_bytes(rq);

    //block_read();
    //block_write();
    
    blk_mq_start_request(rq);
    if(rq_data_dir(rq) == READ)
    {
        block_read(sc, blc_buffer, nbts);
    }
    else if(rq_data_dir(rq) == WRITE)
    {
        block_write(sc, blc_buffer, nbts);
    }
    blk_mq_end_request(rq, BLK_STS_OK);

    return BLK_STS_OK;
}



MODULE_LICENSE("GPL");