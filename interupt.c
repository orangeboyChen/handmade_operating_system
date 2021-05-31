#include "bootpack.h"
#include <stdio.h>

void init_pic()
{
    io_out8(PIC0_IMR, 0xff);
    io_out8(PIC1_IMR, 0xff);

    io_out8(PIC0_ICW1, 0x11);
    io_out8(PIC0_ICW2, 0x20);
    io_out8(PIC0_ICW3, 1 << 2);
    io_out8(PIC0_ICW4, 0x01);

    io_out8(PIC1_ICW1, 0x11);
    io_out8(PIC1_ICW2, 0x28);
    io_out8(PIC1_ICW3, 2);
    io_out8(PIC1_ICW4, 0x01);

    io_out8(PIC0_IMR, 0xfb);
    io_out8(PIC1_IMR, 0xff);

    return;
}

void inthandler21(int *esp)
{
    struct BootInfo *bootInfo = getBootInfo();
    char s4[32];
    sprintf(s4, "abc");
    // printInSheet(sheet, 0, 0, s4, COL8_FFFFFF);
    putfonts8_asc(getBootInfo()->vram, getBootInfo()->screenX, 0, 64, COL8_FFFFFF, s4);

    for (;;)
    {
        io_hlt();
    }
}
void inthandler27(int *esp) {}
void inthandler2c(int *esp) {}