#ifndef _INTERUPT_H
#define _INTERUPT_H
#include "common.h"
#include "io.h"
#include "fifo.h"
#include "timer.h"

#include <stdio.h>
void init_pic();
void init_pit();
void inthandler27(int *esp);
#define PIC0_ICW1 0x0020
#define PIC0_OCW2 0x0020
#define PIC0_IMR 0x0021
#define PIC0_ICW2 0x0021
#define PIC0_ICW3 0x0021
#define PIC0_ICW4 0x0021
#define PIC1_ICW1 0x00a0
#define PIC1_OCW2 0x00a0
#define PIC1_IMR 0x00a1
#define PIC1_ICW2 0x00a1
#define PIC1_ICW3 0x00a1
#define PIC1_ICW4 0x00a1

#define PIT_CTRL 0x0043
#define PIT_CNT0 0x0040

#endif