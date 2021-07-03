#ifndef _IO_H
#define _IO_H

#include "common.h"
#include "action.h"

#define MOUSE_WIDTH 8
#define MOUSE_HEIGHT 16

#define CAPS_LOCK_KEY 0x3a
#define NUM_LOCK_KEY 0x45
#define SCROLL_LOCK_KEY 0x46
#define LEFT_SHIFT_KEY 0x2a
#define RIGHT_SHIFT_KEY 0x36

extern struct MouseData mouseData;
extern struct KeyboardData keyboardData;
struct MouseData
{
    unsigned char data[3];
    unsigned char phase;
    int moveX, moveY, btn;
    int preX, preY, preBtn;
    long lastClickTimestamp;
    int x, y;
    short rootIndexOfPointer;
};

struct KeyboardData
{
    unsigned int pressData[0x80];
    unsigned int capslock;
};

static char keyTable0[0x80] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '^', 0, 0,
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '@', '[', 0, 0, 'A', 'S',
    'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', ':', 0, 0, ']', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1',
    '2', '3', '0', '.', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0x5c, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x5c, 0, 0};
static char keyTable1[0x80] = {
    0, 0, '!', 0x22, '#', '$', '%', '&', 0x27, '(', ')', '~', '=', '~', 0, 0,
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '`', '{', 0, 0, 'A', 'S',
    'D', 'F', 'G', 'H', 'J', 'K', 'L', '+', '*', 0, 0, '}', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1',
    '2', '3', '0', '.', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, '_', 0, 0, 0, 0, 0, 0, 0, 0, 0, '|', 0, 0};

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

void analyseKeyborardData(unsigned int key);

#endif