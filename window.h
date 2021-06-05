#ifndef _WINDOW_H
#define _WINDOW_H

#include "io.h"
#include "sheet.h"
#include <stdio.h>
#include "common.h"
struct Window
{
    struct Sheet *windowSheet;
};
void initDesktop(struct Sheet *rootSheet);
struct Sheet *createWindow(struct Sheet *fatherSheet, short x, short y, short width, short height, char *title);
void initMouseCursorSheet(struct Sheet *rootSheet);
void updateMouseCursorSheet(short moveX, short moveY);

#endif