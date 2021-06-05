#include "io.h"

struct MouseData mouseData;

void waitKbcSendKey()
{
    while (true)
    {
        if ((io_in8(PORT_KEYSTA) & KEYSTA_SEND_NOTREADY) == 0)
        {
            break;
        }
    }
    return;
}

void initKeyboard()
{
    waitKbcSendKey();
    io_out8(PORT_KEYCMD, KEYCMD_WRITE_MODE);
    waitKbcSendKey();
    io_out8(PORT_KEYDAT, KBC_MODE);
    return;
}

void enableMouse(struct MouseData *mouseData)
{
    waitKbcSendKey();
    io_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
    waitKbcSendKey();
    io_out8(PORT_KEYDAT, MOUSECMD_ENABLE);

    mouseData->phase = 0;
    return;
}

int putInMouseData(struct MouseData *mouseData, unsigned char data)
{
    if (mouseData->phase == 0)
    {
        if (data == 0xfa)
        {
            mouseData->phase = 1;
        }
        return 0;
    }

    if (mouseData->phase == 1)
    {
        if ((data & 0xc8) == 0x08)
        {
            mouseData->data[0] = data;
            mouseData->phase = 2;
        }
        return 0;
    }

    if (mouseData->phase == 2)
    {
        mouseData->data[1] = data;
        mouseData->phase = 3;
        return 0;
    }

    if (mouseData->phase == 3)
    {
        mouseData->data[2] = data;
        mouseData->phase = 1;

        //解析鼠标数据
        mouseData->btn = mouseData->data[0] & 0x07;
        mouseData->moveX = mouseData->data[1];
        mouseData->moveY = mouseData->data[2];

        if ((mouseData->data[0] & 0x10) != 0)
        {
            mouseData->moveX |= 0xffffff00;
        }

        if ((mouseData->data[0] & 0x20) != 0)
        {
            mouseData->moveY |= 0xffffff00;
        }

        mouseData->moveY = -mouseData->moveY;
        return 1;
    }

    return -1;
}