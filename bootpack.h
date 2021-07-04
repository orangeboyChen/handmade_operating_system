#ifndef _BOOTPACK_H
#define _BOOTPACK_H

#include "bootpack.h"
#include "io.h"
#include "fifo.h"
#include "interupt.h"
#include "sheet.h"
#include "common.h"
#include "widget.h"
#include <stdio.h>
#include <string.h>
void showSomething();
struct BootInfo
{
    char cyls, leds, vmode, reserve;
    short screenX, screenY;
    char *vram;
};

struct BootInfo *getBootInfo();
struct MemoryManager *getMemoryManager();

void textCursorTick();

extern struct Sheet *statusLabel;

#endif
