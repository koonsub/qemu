
#include "block/block_int.h"
#include "qemu-common.h"
#include "qcow2.h"
#include "trace.h"


int qcow2_journal_write(BlockDriverState *bs, int64_t offset ,const void *buf, int count )
{
    BDRVQcowState *s = bs->opaque;
    Journal j = s->journal;

    j->block[ j -> tx_block_count ].data = buf;
    j->block[j -> tx_block_count].offset = offset;
  
    j->tx_block_count++;

    if (j -> tx_block_count == 5) {
        
    }
    
   //qcow2_cache_entry_mark_dirty( c,table);
}

static void qcow2_journal_recover(void)
{
    install_trans();
  
}

static void qcow2_journal_install_trans(BlockDriverState *bs)
{
  BDRVQcowState *s = bs->opaque;

  int tail;

  for(tail = ) 
}
static void qcow2_journal_check_pointing(BlockDriverState *bs)
{

}

void qcow2_journal_begin_op(BlockDriverState *bs)
{
    BDRVQcowState *s = bs->opaque;
    Journal j = s->journal;
    
    if ( j -> tx_size == 3 ) {
        qcow2_journal_recover();
    } else {
        j -> tx_id = (j->tx_id + 1) % 3;

    }
    // s->journal->begin = 1;

}

void qcow2_journal_end_op(BlockDriverState *bs)
{
    BDRVQcowState *s =bs->opaque;
    Journal j = s -> journal;

    

}
static void
qcow2_journal_commit(BlockDriverState *bs) 
{
    BDRVQcowState *s = bs->opaque;
    Journal j = s->journal;
    int i, ret;

    if( j->tx_block_count > TRANSACTIONBLOCKSIZE){
      return;
    }

    for (i = 0; i < j->tx_block_count; i++) {

        // 캐시의 각 테이블을 저널 영역에 쓴다.
        // TODO: bdrv_pwrite는 bs의 file에 직접 쓰는 것이기 때문에, bs의 file이 아닌, 저널에 쓰게
        // bdrv_pwrite의 함수를 좀 바꾸거나, 새로 함수를 만들어서, 저널영역에 쓰게 해야함.
        ret = bdrv_pwrite(bs,  j->journal_offset + s->cluster_size * (j->tx_id * 5 +1+i) , j->block[i].data, s->cluster_size);

        if (ret < 0) {
           return;
        }
    }

    uint32_t new_head_id =( j ->jsb.start_tx_id + j->jsb.tx_count++)  % 3;
    JournalTransactionHeader jth;

    jth.tx_id = new_head_id;
    jth.tx_block_count = j->tx_block_count;
    jth.tx_journal_offset = j->journal_offset + (5*new_head_id + 1) *s->cluster_size; 
    for( i = 0; i < j->tx_block_count; i++ ) {
        jth.tx_disk_offset[i] = j->block[i].offset;
    }

    ret = bdrv_pwrite(bs,  j->journal_offset , &(j->jsb), j->jsb.jsb_size);
    
    if (ret < 0) {
           return;
    }

    ret = bdrv_pwrite(bs,  j->journal_offset + j->jsb.jsb_size + new_head_id*sizeof(JournalTransactionHeader) ,
         &jth, sizeof(JournalTransactionHeader));
    
    if (ret < 0) {
           return;
    }

    

}



// Copy modified blocks from cache to journal.
static void
qcow2_journal_flush_to_disk(BlockDriverState *bs)
{
    BDRVQcowState *s = bs->opaque;
    Journal j = s->journal;
    int i;

    if( j->tx_size > TRANSACTIONBLOCKSIZE){
      return;
    }

    for (i = 0; i < j->tx_size; i++) {

        // 캐시의 테이블을 가져온다.

        // 캐시의 각 테이블을 저널 영역에 쓴다.
        // TODO: bdrv_pwrite는 bs의 file에 직접 쓰는 것이기 때문에, bs의 file이 아닌, 저널에 쓰게
        // bdrv_pwrite의 함수를 좀 바꾸거나, 새로 함수를 만들어서, 저널영역에 쓰게 해야함.
        bdrv_pwrite(bs,  , s->journal->block[i].data, s->cluster_size);

        
    //     // 쓸 block들에 대하여 읽어온다.
    //     struct buf *to = bread(log.dev, log.start+tail+1); // log block

    //     // 캐시 block들을 읽어온다.
    //     struct buf *from = bread(log.dev, log.lh.block[tail]); // cache block

    //     // 쓸 block들에 대하여, 캐시 block의 데이터들을 저널 영역에 넣어준다.
    //     memmove(to->data, from->data, BSIZE);
    //     bwrite(to);  // write the log
    //     brelse(from);
    //     brelse(to);
    // }
}
