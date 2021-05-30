#include "bootpack.h"
#include <stdio.h>
#define MEMMAN_ADDR 0x003c0000

void initMemoryManage(struct MemoryManager *memoryManager)
{

    initMemoryManager(memoryManager, 0x00400000, 0xbfffffff);
}

void initMemoryManager(struct MemoryManager *memoryManager, unsigned int start, unsigned int end)
{

    int i;
    for (i = 0; i < 2048; i++)
    {
        memoryManager->blocks[i].isUsing = 0;
        memoryManager->blocks[i].nextBlock = NULL;
    }

    memoryManager->firstBlock = cutBlock(NULL, start, end);

    // char *s;
    // sprintf(s, "%u %u", memoryManager->firstBlock->addrFrom, memoryManager->firstBlock->addrTo);
    // fillBox(getBootInfo()->vram, getBootInfo()->screenX, COL8_000000, 0, 0, getBootInfo()->screenX, 16);
    // putfonts8_asc(getBootInfo()->vram, getBootInfo()->screenX, 0, 0, COL8_FFFFFF, s);

    // memoryManager->blocks[0].addrFrom = start;

    // memoryManager->blocks[0].blockType = 0;

    // while (1)
    // {
    //     if ((long)(end - start) > 2 << memoryManager->blocks[0].blockType)
    //     {
    //         break;
    //     }
    //     memoryManager->blocks[0].blockType += 1;
    // }
    // memoryManager->blocks[0].blockType += 1;

    // if (memoryManager->blocks[0].blockType > 32)
    // {
    //     memoryManager->blocks[0].blockType = 32;
    // }

    // memoryManager->blocks[0].addrTo = start + (1 << memoryManager->blocks[0].blockType) - 1;

    // memoryManager->blocks[0].isUsing = 1;
    // memoryManager->blocks[0].nextBlock = NULL;
    // memoryManager->firstBlock = &memoryManager->blocks[0];
}

struct MemoryBlock *cutBlock(struct MemoryBlock *nextBlock, unsigned int fromAddr, int toAddr)
{
    if (toAddr - fromAddr == 0)
    {
        return nextBlock;
    }

    struct MemoryManager *manager = getMemoryManager();
    unsigned int leftMemory = toAddr - fromAddr + 1;

    struct MemoryBlock *firstBlock = NULL;
    struct MemoryBlock *lastBlock = NULL;
    unsigned int currentFromAddr = fromAddr;

    // char *s;
    // sprintf(s, "%u", nextBlock->addrFrom);
    // fillBox(getBootInfo()->vram, getBootInfo()->screenX, COL8_000000, 0, 16, getBootInfo()->screenX, 16 * 2);
    // putfonts8_asc(getBootInfo()->vram, getBootInfo()->screenX, 0, 16, COL8_FFFFFF, s);
    // return;

    int de = 0;
    while (leftMemory > 0)
    {
        // de++;
        // if (de == 1)
        // {
        //     char *s;
        //     sprintf(s, "%u", leftMemory);
        //     fillBox(getBootInfo()->vram, getBootInfo()->screenX, COL8_000000, 0, 16, getBootInfo()->screenX, 16 * 2);
        //     putfonts8_asc(getBootInfo()->vram, getBootInfo()->screenX, 0, 16, COL8_FFFFFF, s);
        //     break;
        // }

        int blockType = getMaxBlockTypeInMemory(manager, leftMemory);

        // char *s;
        // sprintf(s, "%u", leftMemory);
        // fillBox(getBootInfo()->vram, getBootInfo()->screenX, COL8_000000, 0, 16, getBootInfo()->screenX, 16 * 2);
        // putfonts8_asc(getBootInfo()->vram, getBootInfo()->screenX, 0, 16, COL8_FFFFFF, s);

        struct MemoryBlock *block = getUnusedBlock(manager);
        block->addrFrom = currentFromAddr;
        block->addrTo = currentFromAddr + (1 << blockType) - 1;
        block->blockType = blockType;
        block->isUsing = 1;

        if (lastBlock != NULL)
        {
            lastBlock->nextBlock = block;
        }

        if (firstBlock == NULL)
        {
            firstBlock = block;

            // char *s;
            // sprintf(s, "%u %u", getMemoryManager()->firstBlock->addrFrom, getMemoryManager()->firstBlock->addrTo);
            // fillBox(getBootInfo()->vram, getBootInfo()->screenX, COL8_000000, 0, 0, getBootInfo()->screenX, 32);
            // putfonts8_asc(getBootInfo()->vram, getBootInfo()->screenX, 0, 0, COL8_FFFFFF, s);
        }

        lastBlock = block;
        currentFromAddr = currentFromAddr + (1 << blockType);
        leftMemory -= (1 << blockType);

        // de++;
        // if (de == 1)
        // {
        //     char *s;
        //     sprintf(s, "%u %u", leftMemory, blockType);
        //     fillBox(getBootInfo()->vram, getBootInfo()->screenX, COL8_000000, 0, 0, getBootInfo()->screenX, 16);
        //     putfonts8_asc(getBootInfo()->vram, getBootInfo()->screenX, 0, 0, COL8_FFFFFF, s);
        //     break;
        // }
    }
    lastBlock->nextBlock = nextBlock;

    return firstBlock;
}

unsigned int allocaMemory(struct MemoryManager *memoryManager, unsigned int size)
{
    int preferBlockType = getPreferBlockType(memoryManager, size);

    struct MemoryBlock *preferBlock = NULL;

    struct MemoryBlock *previousBlockOfPreferBlock = NULL;
    struct MemoryBlock *previousBlock = NULL;
    struct MemoryBlock *currentBlock = memoryManager->firstBlock;

    // char *s3;
    // sprintf(s3, "abc");
    // putfonts8_asc(getBootInfo()->vram, getBootInfo()->screenX, 0, 0, COL8_FFFFFF, s3);
    // return;

    while (1)
    {
        if (currentBlock->isUsing == 1 && currentBlock->blockType < preferBlock->blockType && currentBlock->blockType >= preferBlockType)
        {
            preferBlock = currentBlock;
            previousBlockOfPreferBlock = previousBlock;
        }

        if (currentBlock->nextBlock == NULL)
        {
            break;
        }
        previousBlock = currentBlock;
        currentBlock = currentBlock->nextBlock;
    }

    if (preferBlock->isUsing != 1)
    {
        return NULL;
    }

    // char *s;
    // sprintf(s, "%u %u", preferBlock->addrFrom, preferBlock->nextBlock->addrTo);
    // fillBox(getBootInfo()->vram, getBootInfo()->screenX, COL8_000000, 0, 16, getBootInfo()->screenX, 32);
    // putfonts8_asc(getBootInfo()->vram, getBootInfo()->screenX, 0, 16, COL8_FFFFFF, s);

    unsigned int newAddrFrom = preferBlock->addrTo - size + 1;

    if (previousBlockOfPreferBlock != NULL)
    {
        previousBlockOfPreferBlock->nextBlock = cutBlock(preferBlock, preferBlock->addrFrom, newAddrFrom - 1);
    }
    else
    {
        memoryManager->firstBlock = cutBlock(preferBlock, preferBlock->addrFrom, newAddrFrom - 1);
    }

    preferBlock->addrFrom = newAddrFrom;
    preferBlock->isUsing = 2;
    preferBlock->requestSize = size;

    combineMemoryBlock(getMemoryManager());
    // char *s;
    // sprintf(s, "%u %u %u %u", preferBlock->addrFrom, preferBlock->addrTo, memoryManager->firstBlock->addrFrom, memoryManager->firstBlock->addrTo);
    // fillBox(getBootInfo()->vram, getBootInfo()->screenX, COL8_000000, 0, 16, getBootInfo()->screenX, 32);
    // putfonts8_asc(getBootInfo()->vram, getBootInfo()->screenX, 0, 16, COL8_FFFFFF, s);

    return preferBlock->addrFrom;

    // if (preferBlock->blockType == preferBlockType)
    // {
    //     preferBlock->isUsing = 2;
    //     return preferBlock->addrFrom;
    // }
    // else
    // {
    //     while (1)
    //     {
    //         if (preferBlock->blockType <= preferBlockType)
    //         {
    //             break;
    //         }

    //         struct MemoryBlock *rightFreeBlock = getUnusedBlock(memoryManager);
    //         rightFreeBlock->addrFrom = ((preferBlock->addrFrom + preferBlock->addrTo) / 2 + 1);
    //         rightFreeBlock->addrTo = preferBlock->addrTo;
    //         rightFreeBlock->blockType = preferBlock->blockType - 1;
    //         rightFreeBlock->nextBlock = preferBlock->nextBlock;

    //         preferBlock->nextBlock = rightFreeBlock;
    //         preferBlock->addrTo = ((preferBlock->addrFrom + preferBlock->addrTo) / 2);
    //         preferBlock->blockType--;
    //     }

    //     preferBlock->isUsing = 2;
    //     preferBlock->requestSize = size;

    //     combineMemoryBlock(memoryManager);
    //     return preferBlock->addrFrom;
    // }
}

struct MemoryBlock *getUnusedBlock(struct MemoryManager *memoryManager)
{
    int i;

    for (i = 0; i < 2048; i++)
    {
        if (memoryManager->blocks[i].isUsing == 0)
        {
            memoryManager->blocks[i].isUsing = 1;
            return &memoryManager->blocks[i];
        }
    }
    return NULL;
}

void combineMemoryBlock(struct MemoryManager *memoryManager)
{
    struct MemoryBlock *currentBlock = memoryManager->firstBlock;
    struct MemoryBlock *nextBlock = memoryManager->firstBlock->nextBlock;

    while (1)
    {
        if (nextBlock == NULL)
        {

            break;
        }

        if (currentBlock->isUsing == 2)
        {
            currentBlock = nextBlock;
            nextBlock = nextBlock->nextBlock;
            continue;
        }

        if (nextBlock->isUsing == 2)
        {
            currentBlock = nextBlock->nextBlock;
            if (currentBlock == NULL)
            {
                break;
            }

            nextBlock = currentBlock->nextBlock;
            continue;
        }

        if (currentBlock->blockType == nextBlock->blockType)
        {
            currentBlock->addrTo = nextBlock->addrTo;
            currentBlock->blockType++;
            currentBlock->nextBlock = nextBlock->nextBlock;

            struct MemoryBlock *temp = nextBlock;

            nextBlock = nextBlock->nextBlock;
            temp->nextBlock = NULL;
            temp->isUsing = 0;
        }
        else
        {
            currentBlock = nextBlock;
            nextBlock = nextBlock->nextBlock;
        }

        if (nextBlock == NULL)
        {

            break;
        }
    }
}

int releaseBlock(struct MemoryManager *memoryManager, unsigned int addr)
{
    struct MemoryBlock *currentBlock = memoryManager->firstBlock;
    while (currentBlock != NULL)
    {
        if (currentBlock->addrFrom == addr)
        {
            currentBlock->isUsing = 1;
            combineMemoryBlock(memoryManager);
            return 0;
        }
        currentBlock = currentBlock->nextBlock;
    }

    return 1;
}

unsigned int getMaxBlockTypeInMemory(struct MemoryManager *memoryManager, unsigned int size)
{
    int blockType = 31;
    while (size < 1 << blockType && blockType > 0)
    {
        blockType--;
    }

    return blockType;
}

unsigned int getPreferBlockType(struct MemoryManager *memoryManager, unsigned int size)
{

    int blockType = 0;
    while (size > 1 << blockType && blockType < 31)
    {
        blockType++;
    }

    return blockType;
}

unsigned int getUnusedMemoryTotal(struct MemoryManager *memoryManager)
{
    unsigned int total = 0;
    struct MemoryBlock *currentBlock = memoryManager->firstBlock;

    // char *s;
    // sprintf(s, "%u %u", currentBlock->blockType, currentBlock->addrTo);
    // fillBox(getBootInfo()->vram, getBootInfo()->screenX, COL8_000000, 0, 100, getBootInfo()->screenX, 116);
    // putfonts8_asc(getBootInfo()->vram, getBootInfo()->screenX, 0, 100, COL8_FFFFFF, s);

    while (currentBlock != NULL)
    {
        if (currentBlock->isUsing == 1)
        {
            total += currentBlock->addrTo - currentBlock->addrFrom + 1;
        }

        currentBlock = currentBlock->nextBlock;
    }
    return total;
}