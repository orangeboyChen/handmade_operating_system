#ifndef _IO_H
#define _IO_H

#include "common.h"

#define MOUSE_WIDTH 8
#define MOUSE_HEIGHT 16
extern struct MouseData mouseData;
struct MouseData
{
    unsigned char data[3];
    unsigned char phase;
    int moveX, moveY, btn;
    int x, y;
    short rootIndexOfPointer;
};
#define PORT_KEYDAT 0x0060
#define PORT_KEYSTA 0x0064
#define PORT_KEYCMD 0x0064
#define KEYCMD_SENDTO_MOUSE 0xd4
#define MOUSECMD_ENABLE 0xf4
#define KEYSTA_SEND_NOTREADY 0x02
#define KEYCMD_WRITE_MODE 0x60
#define KBC_MODE 0x47
void enableMouse();
void initKeyboard();
void waitKbcSendKey();
int putInMouseData(struct MouseData *mouseData, unsigned char data);

#endif