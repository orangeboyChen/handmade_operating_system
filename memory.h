#ifndef _MEMORY_H
#define _MEMORY_H

#include <stdio.h>
#include <string.h>
#include "nas.h"
#include "bootpack.h"
#include "common.h"

#define MEMMAN_ADDR 0x003c0000
#define MEMORY_BLOCK 8192
struct MemoryBlock
{
    unsigned int addrFrom;
    unsigned int addrTo;
    struct MemoryBlock *nextBlock;
    unsigned int blockType;

    unsigned int requestSize;
    unsigned int taskId;
    unsigned int isUsing;
};

struct MemoryManager
{
    struct MemoryBlock *firstBlock;
    struct MemoryBlock blocks[MEMORY_BLOCK];
};

void initMemoryManage(struct MemoryManager *memoryManager);
void initMemoryManager(struct MemoryManager *memoryManager, unsigned int start, unsigned int end);
unsigned int allocaMemory(struct MemoryManager *memoryManager, unsigned int size);
struct MemoryBlock *getUnusedBlock(struct MemoryManager *memoryManager);
void combineMemoryBlock(struct MemoryManager *memoryManager);
unsigned int getPreferBlockType(struct MemoryManager *memoryManager, unsigned int size);
unsigned int getUnusedMemoryTotal(struct MemoryManager *memoryManager);
int releaseBlock(struct MemoryManager *memoryManager, unsigned int addr);
struct MemoryBlock *cutBlock(struct MemoryBlock **lastReturnBlock, unsigned int fromAddr, int toAddr);
unsigned int getMaxBlockTypeInMemory(struct MemoryManager *memoryManager, unsigned int size);
unsigned int getUsableMemory(unsigned int start, unsigned int end);

#endif