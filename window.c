#include "bootpack.h"
#include <stdio.h>

unsigned int getStringSize(char *c)
{
    int i = 0;
    while (*c != '\0')
    {
        c += 1;
        i++;
    }
    return i;
}

void initMouseCursorSheet(struct Sheet *rootSheet)
{
    struct Sheet *mouseSheet = createSubsheetToTop(rootSheet, 0, 0, rootSheet->width, rootSheet->height);
    setFixedTop(mouseSheet);

    fillInSheet(mouseSheet, 0, 0, mouseSheet->width, mouseSheet->height, COL_TRANSPARENT);
    initMouseCursor(mouseSheet, 80, 80);
    printInSheet(mouseSheet, 0, 32, "123", COL8_FFFFFF);
    updateSheet(mouseSheet);
}

void initDesktop(struct Sheet *rootSheet)
{
    struct Sheet *desktopSheet = createSubsheetToTop(rootSheet, 0, 0, rootSheet->width, rootSheet->height);
    setFixedTop(desktopSheet);

    fillInSheet(desktopSheet, 0, 0, desktopSheet->width, 17, COL8_FFFFFF);
    fillInSheet(desktopSheet, 0, 17, desktopSheet->width, 1, COL8_848484);
    fillInSheet(desktopSheet, 0, 18, desktopSheet->width, desktopSheet->height - 18, COL_TRANSPARENT);

    // char s[64];
    // sprintf(s, "%d %uMB", desktopSheet->nextSheet->index, getUnusedMemoryTotal(getMemoryManager()) / (1024 * 1024));
    printInSheet(desktopSheet, 8, 0, "Explorer", COL8_000000);

    // printInSheet(desktopSheet, 8, 0, s, COL8_000000);

    // updateSheet(desktopSheet);
}

void createWindow(struct Sheet *fatherSheet, short x, short y, short width, short height, char *title)
{
    struct Sheet *windowSheet = createSubsheetToTop(fatherSheet, x, y, width, height);
    fillInSheet(windowSheet, 1, 1, width - 2, height - 2, COL8_FFFFFF);
    fillInSheet(windowSheet, 0, 0, width, 1, COL8_848400);
    fillInSheet(windowSheet, 0, 0, 1, height, COL8_848484);
    fillInSheet(windowSheet, width - 1, 0, 1, height, COL8_848484);
    fillInSheet(windowSheet, 0, height - 1, width, 1, COL8_848484);
    fillInSheet(windowSheet, 0, 18, width, 1, COL8_848484);

    fillInSheet(windowSheet, 6, 6, 8, 8, COL8_FF0000);
    fillInSheet(windowSheet, 18, 6, 8, 8, COL8_00FF00);
    fillInSheet(windowSheet, 30, 6, 8, 8, COL8_0000FF);

    printInSheet(windowSheet, width / 2 - getStringSize(title) * 8 / 2, 2, title, COL8_000000);

    char c[32];
    sprintf(c, "%u%u%u", windowSheet->index, windowSheet->nextSheet->index, windowSheet->previousSheet->index);
    printInSheet(windowSheet, 2, 20, c, COL8_000000);

    char c2[32];
    sprintf(c2, "%u/%u/%u %u/%u/%u", getFixedTop(windowSheet), getFixedTop(windowSheet->nextSheet), getFixedTop(windowSheet->previousSheet),
            windowSheet->status, windowSheet->nextSheet->status, windowSheet->previousSheet->status);
    printInSheet(windowSheet, 2, 36, c2, COL8_000000);

    // updateSheet(windowSheet);
}