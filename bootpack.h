#ifndef _BOOTPACK_H
#define _BOOTPACK_H

#include "bootpack.h"
#include "io.h"
#include "fifo.h"
#include "interupt.h"
#include "sheet.h"
#include "common.h"
#include <stdio.h>
#include <string.h>

//boot
struct BootInfo
{
    char cyls, leds, vmode, reserve;
    short screenX, screenY;
    char *vram;
};

struct BootInfo *getBootInfo();
struct MemoryManager *getMemoryManager();
void logger(char *c, unsigned int y);

//task.c
struct Task
{
    int id;
    struct Sheet *sheet;
};

#endif
