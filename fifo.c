#include "fifo.h"

struct Fifo systemFifo;
void initFifo(struct Fifo *fifo)
{
    fifo->headIndex = 0;
    fifo->tailIndex = 0;
    fifo->size = 0;
    int i;
    memset(fifo->fifoItem, 0, sizeof(struct FifoItem) * FIFO_MAX);
}

void putInFifo(struct Fifo *fifo, unsigned int type, unsigned int subtype, char data)
{
    if (fifo->size == FIFO_MAX)
    {
        // return;
        getInFifo(fifo);
    }

    struct FifoItem *currentItem = &(fifo->fifoItem[fifo->headIndex]);
    fifo->headIndex++;
    if (fifo->headIndex == FIFO_MAX)
    {
        fifo->headIndex = 0;
    }

    currentItem->data = data;
    currentItem->type = type;
    currentItem->subtype = subtype;

    fifo->size++;
}

struct FifoItem *getInFifo(struct Fifo *fifo)
{
    if (fifo->size == 0)
    {
        return NULL;
    }

    struct FifoItem *currentItem = &(fifo->fifoItem[fifo->tailIndex]);
    fifo->tailIndex++;
    if (fifo->tailIndex == FIFO_MAX)
    {
        fifo->tailIndex = 0;
    }

    fifo->size--;
    return currentItem;
}