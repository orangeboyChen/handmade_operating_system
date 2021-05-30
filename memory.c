#include "bootpack.h"
#include <stdio.h>
#include <string.h>

void initMemoryManage(struct MemoryManager *memoryManager)
{

    // initMemoryManager(memoryManager, 0x00400000, 0xbfffffff);
    initMemoryManager(memoryManager, 0x00400000, 0x00400000 + 16 * 1024 * 1024);
}

void initMemoryManager(struct MemoryManager *memoryManager, unsigned int start, unsigned int end)
{

    int i;
    for (i = 0; i < 2048; i++)
    {
        memoryManager->blocks[i].isUsing = 0;
        memoryManager->blocks[i].nextBlock = NULL;
    }

    struct MemoryBlock *lastCutBlock = NULL;
    memoryManager->firstBlock = cutBlock(lastCutBlock, start, end);
    lastCutBlock->nextBlock = NULL;

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

struct MemoryBlock *cutBlock(struct MemoryBlock **lastReturnBlock, unsigned int fromAddr, int toAddr)
{
    if (toAddr - fromAddr + 1 <= 0)
    {
        return NULL;
    }

    struct MemoryManager *manager = getMemoryManager();
    unsigned int leftMemory = toAddr - fromAddr + 1;

    if (leftMemory == 1)
    {
        struct MemoryBlock *block = getUnusedBlock(manager);
        block->addrFrom = fromAddr;
        block->addrTo = toAddr;
        block->blockType = 0;
        *lastReturnBlock = block;
        return block;
    }

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

        de++;
        if (toAddr - fromAddr + 1 == 1 && de == 1)
        {
            char s[32];
            sprintf(s, "%u %u", leftMemory, blockType);
            fillBox(getBootInfo()->vram, getBootInfo()->screenX, COL8_000000, 0, 0, getBootInfo()->screenX, 16 * 1);
            putfonts8_asc(getBootInfo()->vram, getBootInfo()->screenX, 0, 0, COL8_FFFFFF, s);
        }

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

    *lastReturnBlock = lastBlock;

    // char s[32];
    // sprintf(s, "%u/%u %u", (*lastReturnBlock)->addrFrom, (*lastReturnBlock)->addrTo, (*lastReturnBlock)->requestSize);
    // fillBox(getBootInfo()->vram, getBootInfo()->screenX, COL8_000000, 0, 0, getBootInfo()->screenX, 16);
    // putfonts8_asc(getBootInfo()->vram, getBootInfo()->screenX, 0, 0, COL8_FFFFFF, s);

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
    struct MemoryBlock *lastCutBlock = NULL;

    if (size == (1 << preferBlock->blockType))
    {
        if (previousBlockOfPreferBlock != NULL)
        {
            previousBlockOfPreferBlock->nextBlock = preferBlock;
        }
        else
        {
            memoryManager->firstBlock = preferBlock;
        }
    }
    else
    {
        if (previousBlockOfPreferBlock != NULL)
        {
            previousBlockOfPreferBlock->nextBlock = cutBlock(&lastCutBlock, preferBlock->addrFrom, newAddrFrom - 1);
        }
        else
        {
            memoryManager->firstBlock = cutBlock(&lastCutBlock, preferBlock->addrFrom, newAddrFrom - 1);
        }
        lastCutBlock->nextBlock = preferBlock;
        preferBlock->addrFrom = newAddrFrom;
    }
    preferBlock->blockType = preferBlockType;
    preferBlock->isUsing = 2;
    preferBlock->requestSize = size;

    // char s[48];
    // sprintf(s, "%u %u", previousBlockOfPreferBlock->nextBlock->nextBlock->nextBlock->nextBlock->nextBlock->nextBlock->nextBlock->addrFrom, previousBlockOfPreferBlock->nextBlock->nextBlock->nextBlock->nextBlock->nextBlock->nextBlock->nextBlock->addrTo);
    // sprintf(s, "%u %u %u %u", preferBlock->addrFrom, preferBlock->addrTo, preferBlock->requestSize, preferBlock->isUsing);
    // sprintf(s, "%u %u %u", lastCutBlock->addrFrom, lastCutBlock->addrTo, lastCutBlock);

    // fillBox(getBootInfo()->vram, getBootInfo()->screenX, COL8_000000, 0, 16, getBootInfo()->screenX, 32);
    // putfonts8_asc(getBootInfo()->vram, getBootInfo()->screenX, 0, 16, COL8_FFFFFF, s);

    // combineMemoryBlock(getMemoryManager());

    memset(preferBlock->addrFrom, 0, size);
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
    struct MemoryBlock *previousBlock = NULL;
    struct MemoryBlock *currentBlock = memoryManager->firstBlock;

    if (currentBlock->nextBlock == NULL)
    {
        return;
    }

    struct MemoryBlock *pointerBlock = currentBlock->nextBlock;

    if (pointerBlock == NULL)
    {
        return;
    }

    int de = 0;
    while (currentBlock != NULL && pointerBlock != NULL)
    {
        if (currentBlock->isUsing == 1 && pointerBlock->isUsing == 1)
        {
            currentBlock->isUsing = 0;
            pointerBlock->isUsing = 0;

            while (pointerBlock->nextBlock != NULL && pointerBlock->nextBlock->isUsing == 1)
            {
                pointerBlock = pointerBlock->nextBlock;
                pointerBlock->isUsing = 0;
            }

            de++;
            struct MemoryBlock *lastCutBlock = NULL;
            if (previousBlock == NULL)
            {
                char s2[32];
                sprintf(s2, "%u %u %u", previousBlock->addrFrom, previousBlock->addrTo);
                fillBox(getBootInfo()->vram, getBootInfo()->screenX, COL8_000000, 0, 48 + 16 * de, getBootInfo()->screenX, 48 + 16 * de + 16);
                putfonts8_asc(getBootInfo()->vram, getBootInfo()->screenX, 0, 48 + 16 * de, COL8_FFFFFF, s2);
                memoryManager->firstBlock = cutBlock(&lastCutBlock, currentBlock->addrFrom, pointerBlock->addrTo);
            }
            else
            {
                previousBlock->nextBlock = cutBlock(&lastCutBlock, currentBlock->addrFrom, pointerBlock->addrTo);
            }

            if (1)
            {
                char s[32];
                // sprintf(s, "%u %u", currentBlock->addrFrom, pointerBlock->addrTo);
                // sprintf(s, "%u %u", memoryManager->firstBlock->nextBlock->nextBlock->nextBlock->nextBlock->nextBlock->nextBlock->nextBlock->nextBlock->nextBlock->nextBlock->nextBlock->nextBlock->nextBlock->nextBlock->addrFrom,
                //         memoryManager->firstBlock->nextBlock->nextBlock->nextBlock->nextBlock->nextBlock->nextBlock->nextBlock->nextBlock->nextBlock->nextBlock->nextBlock->nextBlock->nextBlock->nextBlock->addrTo);
                // fillBox(getBootInfo()->vram, getBootInfo()->screenX, COL8_000000, 0, 100, getBootInfo()->screenX, 116);
                // putfonts8_asc(getBootInfo()->vram, getBootInfo()->screenX, 0, 100, COL8_FFFFFF, s);
            }

            lastCutBlock->nextBlock = pointerBlock->nextBlock;

            if (lastCutBlock->nextBlock == NULL || lastCutBlock->nextBlock->nextBlock == NULL)
            {
                break;
            }

            previousBlock = lastCutBlock;
            currentBlock = lastCutBlock->nextBlock;
            pointerBlock = currentBlock->nextBlock;
        }
        else
        {
            currentBlock = pointerBlock->nextBlock;
            pointerBlock = currentBlock->nextBlock;
        }
    }
}

int releaseBlock(struct MemoryManager *memoryManager, unsigned int addr)
{
    struct MemoryBlock *previousBlock = NULL;
    struct MemoryBlock *currentBlock = memoryManager->firstBlock;
    while (currentBlock != NULL)
    {
        if (currentBlock->addrFrom == addr)
        {
            struct MemoryBlock *lastCutBlock = NULL;

            if (previousBlock == NULL)
            {
                memoryManager->firstBlock = cutBlock(&lastCutBlock, currentBlock->addrFrom, currentBlock->addrTo);
            }
            else
            {
                previousBlock->nextBlock = cutBlock(&lastCutBlock, currentBlock->addrFrom, currentBlock->addrTo);
            }
            lastCutBlock->nextBlock = currentBlock->nextBlock;

            currentBlock->isUsing = 0;

            // combineMemoryBlock(memoryManager);
            return 0;
        }
        previousBlock = currentBlock;
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

    int de = 0;
    while (currentBlock != NULL)
    {

        if (currentBlock->isUsing == 1)
        {
            total += currentBlock->addrTo - currentBlock->addrFrom + 1;
        }

        // de++;
        // if (de == 3)
        // {
        //     char s[32];
        //     sprintf(s, "%u %u %u %u", currentBlock->addrFrom, currentBlock->addrTo - currentBlock->addrFrom + 1, currentBlock->isUsing, total);
        //     fillBox(getBootInfo()->vram, getBootInfo()->screenX, COL8_000000, 0, 100, getBootInfo()->screenX, 116);
        //     putfonts8_asc(getBootInfo()->vram, getBootInfo()->screenX, 0, 100, COL8_FFFFFF, s);
        //     break;
        // }

        currentBlock = currentBlock->nextBlock;
    }

    // currentBlock = memoryManager->firstBlock;
    // int a = 0;
    // while (1)
    // {
    //     if (currentBlock->nextBlock == NULL)
    //     // if (0)
    //     {
    //         char s[32];
    //         sprintf(s, "%u~%u|%u%u", currentBlock->addrFrom, currentBlock->addrTo, currentBlock->blockType, currentBlock->isUsing);
    //         fillBox(getBootInfo()->vram, getBootInfo()->screenX, COL8_000000, 0, 64 + a * 16, getBootInfo()->screenX, 64 + a * 16 + 16);
    //         putfonts8_asc(getBootInfo()->vram, getBootInfo()->screenX, 0, 64 + a * 16, COL8_FFFFFF, s);
    //         break;
    //     }
    //     else
    //     {
    //         char s[32];
    //         sprintf(s, "%u~%u|%u%u %u~%u|%u%u", currentBlock->addrFrom, currentBlock->addrTo, currentBlock->blockType, currentBlock->isUsing, currentBlock->nextBlock->addrFrom, currentBlock->nextBlock->addrTo, currentBlock->nextBlock->blockType, currentBlock->nextBlock->isUsing);
    //         fillBox(getBootInfo()->vram, getBootInfo()->screenX, COL8_000000, 0, 64 + a * 16, getBootInfo()->screenX, 64 + a * 16 + 16);
    //         putfonts8_asc(getBootInfo()->vram, getBootInfo()->screenX, 0, 64 + a * 16, COL8_FFFFFF, s);

    //         if (currentBlock->nextBlock->nextBlock == NULL)
    //         {
    //             break;
    //         }
    //         currentBlock = currentBlock->nextBlock->nextBlock;
    //     }
    //     a++;
    // }

    return total;
}