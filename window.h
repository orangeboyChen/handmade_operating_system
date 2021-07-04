#ifndef _WINDOW_H
#define _WINDOW_H
#include "widget.h"
#include "io.h"
#include "sheet.h"
#include <stdio.h>
#include "common.h"
#include "task.h"

#define ACTIVE_SHEET_ITEM_TOTAL 64

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

struct ActiveSheetItem
{
    struct Sheet *sheet;
    struct ActiveSheetItem *nextSheet;
    struct ActiveSheetItem *previousSheet;
    unsigned int isUsing;
};

struct WindowsManager
{
    struct Window *currentActiveWindow;
    int isDragging;
    struct Sheet *lastMouseLeftDownSheet;
    int isActiveShowing;

    struct ActiveSheetItem *firstActiveSheet;
    struct ActiveSheetItem *lastActiveSheet;
    struct ActiveSheetItem activeSheetItemStore[ACTIVE_SHEET_ITEM_TOTAL];

    struct Sheet *activeTextField;
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
void onWindowStatusBarMouseLeftDown(struct Sheet *this, unsigned int x, unsigned int y);
void onWindowCloseButtonMouseDown(struct Sheet *this);
void onWindowCloseButtonMouseUp(struct Sheet *this, unsigned int x, unsigned int y);

void initWindowsManager();
struct ActiveSheetItem *getActiveSheetItem();
struct ActiveSheetItem *removeActiveSheetItem(struct ActiveSheetItem *item);
void deleteWindowsManagerActiveSheetItems();
void onWindowKeyPress(struct Sheet *this, char c, unsigned int raw);
void onWindowKeyUp(struct Sheet *this, char c, unsigned int raw);

void releaseWindow(struct Window *window);
void releaseSingleSheet(struct Sheet *temp);
void releaseSheetRecursive(struct Sheet *sheet);
#endif