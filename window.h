#ifndef _WINDOW_H
#define _WINDOW_H
#include "widget.h"
#include "io.h"
#include "sheet.h"
#include <stdio.h>
#include "common.h"
#include "task.h"

struct Window
{
    struct Sheet *closeButtonSheet;
    struct Sheet *statusBarSheet;
    struct Sheet *backgroundSheet;
    struct SHeet *buttonSheet;
    struct Sheet *backgroundOfButtonSheet;
    struct Sheet *titleSheet;
    struct Task *task;
    struct Sheet *sheet;
    char *title;
};

struct WindowsManager
{
    struct Window *currentActiveWindow;
    int isDragging;
    struct Sheet *lastMouseLeftDownSheet;
};

extern struct WindowsManager windowsManager;

void initDesktop(struct Sheet *rootSheet);
struct Window *createWindow(struct Sheet *fatherSheet, short x, short y, short width, short height, char *title);
void initMouseCursorSheet(struct Sheet *rootSheet);
void updateMouseCursorSheet(short moveX, short moveY);
void activeWindow(struct Window *window);
void disactiveWindow(struct Window *window);
void fillWindowBackground(struct Window *window, unsigned int color);
void onWindowStatusBarClick(struct Sheet *this, unsigned int x, unsigned int y);
void activeSheetWindow(struct Sheet *sheet);
void disactiveSheetWindow(struct Sheet *sheet);
#endif