#ifndef _FIFO_H
#define _FIFO_H

#include "fifo.h"
#include "common.h"

#include <string.h>
#include <stdio.h>

#define FIFO_MAX 512
#define FIFO_TYPE_KEYBOARD 0
#define FIFO_TYPE_MOUSE 1

//subtype
//--mouse
#define FIFO_SUBTYPE_MOUSE_LEFT = 0
#define FIFO_SUBTYPE_MOUSE_RIGHT = 1

extern struct Fifo systemFifo;
struct FifoItem
{
    unsigned int type;
    unsigned int subtype;
    unsigned char data;
};

struct Fifo
{
    struct FifoItem fifoItem[FIFO_MAX];
    unsigned int headIndex;
    unsigned int tailIndex;
    unsigned int size;
};
void initFifo(struct Fifo *fifo);
void putInFifo(struct Fifo *fifo, unsigned int type, unsigned int subtype, char data);
struct FifoItem *getInFifo(struct Fifo *fifo);

#endif