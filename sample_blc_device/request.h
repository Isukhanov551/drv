#ifndef __REQUEST_H__
#define __REQUEST_H__

#include <linux/blkdev.h>

void request(struct request_queue *q);

#endif