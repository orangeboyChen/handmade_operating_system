#include "interupt.h"
#include "utils.h"
#include "bootpack.h"
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

void init_pit()
{
    io_out8(PIT_CTRL, 0x34);
    io_out8(PIT_CNT0, 0x9c);
    io_out8(PIT_CNT0, 0x2e);
    return;
}

void inthandler21(int *esp)
{
    unsigned char data;
    io_out8(PIC0_OCW2, 0x61);
    data = io_in8(PORT_KEYDAT);
    putInFifo(&systemFifo, FIFO_TYPE_KEYBOARD, NULL, data);
    return;
}
void inthandler27(int *esp) {}
void inthandler2c(int *esp)
{
    unsigned char data;
    io_out8(PIC1_OCW2, 0x64);
    io_out8(PIC0_OCW2, 0x62);
    data = io_in8(PORT_KEYDAT);
    putInFifo(&systemFifo, FIFO_TYPE_MOUSE, NULL, data);
    return;
}

void inthandler20(int *esp)
{
    io_out8(PIC0_OCW2, 0x60);
    onSystemTimerTick();
    return;
}