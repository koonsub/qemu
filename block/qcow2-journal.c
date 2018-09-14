
#include "block/block_int.h"
#include "qemu-common.h"
#include "qcow2.h"
#include "trace.h"


void qcow2_journal_write(BlockDriverState *bs, Qcow2Cache *c,void *table )
{
    BDRVQcowState *s = bs->opaque;
    int i;

    for (i = 0; i < s->journal->tx_size; i++) {
        if (s->journal->block[i] == table)   // log absorbtion
            break;
    }

    s->journal->block[i] = table;

    if(i == s->journal->tx_size) {
        s->journal->tx_size++;
    } 


   qcow2_cache_entry_mark_dirty( c,table);
}

void qcow2_journal_begin_op(BlockDriverState *bs)
{
    BDRVQcowState *s = bs->opaque;

    s->journal->begin = 1;

}

