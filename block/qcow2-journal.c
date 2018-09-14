#include "block/block_int.h"
#include "qemu-common.h"
#include "qcow2.h"
#include "trace.h"


void qcow2_journal_write(BlockDriverState *bs, void *table )
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


    // ((Qcow2CacheTable *)table)->dirty = true;

    // c->entries[i].dirty = true;

    // s->journal->block[s->journal_transaction.size++] = c->entries[i];
    // s->end_tx = ..

}
