#ifndef _WINDOW_H
#define _WINDOW_H
#include "widget.h"
#include "io.h"
#include "sheet.h"
#include <stdio.h>
#include "common.h"

struct Window
{
    struct Sheet *windowSheet;
    char *title;
};
void initDesktop(struct Sheet *rootSheet);
struct Sheet *createWindow(struct Sheet *fatherSheet, short x, short y, short width, short height, char *title);
void initMouseCursorSheet(struct Sheet *rootSheet);
void updateMouseCursorSheet(short moveX, short moveY);

#endif