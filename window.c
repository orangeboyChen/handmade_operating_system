#include "window.h"

struct Sheet *mouseSheet;
void initMouseCursorSheet(struct Sheet *rootSheet)
{
    // mouseSheet = createSubsheetToTop(rootSheet, 0, 0, rootSheet->width, rootSheet->height);
    mouseSheet = createSubsheetToTop(rootSheet, 0, 0, MOUSE_WIDTH, MOUSE_HEIGHT);
    setFixedTop(mouseSheet);

    fillInSheet(mouseSheet, 0, 0, MOUSE_WIDTH, MOUSE_HEIGHT, COL_TRANSPARENT);
    initMouseCursor(mouseSheet, 0, 0);
    // printInSheet(mouseSheet, 0, 32, "123", COL8_FFFFFF);

    mouseSheet->attribute[0] = 0; //x
    mouseSheet->attribute[1] = 0; //y
    forceUpdateSheet(mouseSheet);
}

void updateMouseCursorSheet(short moveX, short moveY)
{
    // fillInSheet(mouseSheet, mouseSheet->attribute[0], mouseSheet->attribute[1], MOUSE_WIDTH, MOUSE_HEIGHT, COL_TRANSPARENT);
    mouseSheet->attribute[0] += moveX;
    mouseSheet->attribute[1] += moveY;

    int currentIndex = mouseSheet->fatherSheet->indexMap[mouseSheet->attribute[1] * getBootInfo()->screenX + mouseSheet->attribute[0]];
    if (currentIndex != mouseSheet->index)
    {
        mouseData.rootIndexOfPointer = currentIndex;
    }
    else
    {
        currentIndex = mouseData.rootIndexOfPointer;
    }

    if (mouseSheet->attribute[0] < 0)
    {
        mouseSheet->attribute[0] = 0;
    }
    else if (mouseSheet->attribute[0] >= getBootInfo()->screenX)
    {
        mouseSheet->attribute[0] = getBootInfo()->screenX - 1;
    }

    if (mouseSheet->attribute[1] < 0)
    {
        mouseSheet->attribute[1] = 0;
    }
    else if (mouseSheet->attribute[1] >= getBootInfo()->screenY)
    {
        mouseSheet->attribute[1] = getBootInfo()->screenY - 1;
    }
    // initMouseCursor(mouseSheet, mouseSheet->attribute[0], mouseSheet->attribute[1]);
    moveSheet(mouseSheet, mouseSheet->attribute[0], mouseSheet->attribute[1]);
    mouseData.x = mouseSheet->attribute[0];
    mouseData.y = mouseSheet->attribute[1];

    // updateAllSubsheet(mouseSheet->fatherSheet);

    // forceUpdateSheet(mouseSheet);
    // updateSheet(mouseSheet);
    // fillVramByIndexMap(mouseSheet->fatherSheet);
    // updateSheet(mouseSheet);

    // char c[32];
    // fillInSheet(mouseSheet->fatherSheet->bottomSheet, 0, 80, 150, 16, COL8_000000);
    // sprintf(c, "(%d,%d) %d", mouseSheet->attribute[0], mouseSheet->attribute[1], currentIndex);
    // printInSheet(mouseSheet->fatherSheet->bottomSheet, 0, 80, c, COL8_FFFFFF);
}

void initDesktop(struct Sheet *rootSheet)
{
    struct Sheet *desktopSheet = createSubsheetToTop(rootSheet, 0, 0, rootSheet->width, rootSheet->height);
    // struct Sheet *desktopSheet = createSubsheetToTop(rootSheet, 0, 0, rootSheet->width, 18);

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

struct Sheet *createWindow(struct Sheet *fatherSheet, short x, short y, short width, short height, char *title)
{
    struct Sheet *windowSheet = createSubsheetToTop(fatherSheet, x, y, width, height);

    // //背景图层
    // struct Sheet *backgroundSheet = createSubsheetToTop(windowSheet, 1, 19, width - 2, height - 20);
    struct Sheet *backgroundSheet = createSubsheetToTop(windowSheet, 0, 0, width, height);

    // fillInSheet(backgroundSheet, 0, 0, width - 2, height - 20, COL8_FFFFFF);
    fillInSheet(backgroundSheet, 0, 0, width, height, COL8_FFFFFF);
    setFixedBottom(backgroundSheet);

    //状态图层
    struct Sheet *statusSheet = createSubsheetToTop(windowSheet, 0, 0, width, height);

    fillInSheet(statusSheet, 1, 19, width - 2, height - 19, COL_TRANSPARENT);
    fillInSheet(statusSheet, 1, 1, width - 2, 17, COL8_FFFFFF);
    fillInSheet(statusSheet, 0, 0, width, 1, COL8_848400);
    fillInSheet(statusSheet, 0, 0, 1, height, COL8_848484);
    fillInSheet(statusSheet, width - 1, 0, 1, height, COL8_848484);
    fillInSheet(statusSheet, 0, height - 1, width, 1, COL8_848484);
    fillInSheet(statusSheet, 0, 18, width, 1, COL8_848484);

    initFourRadius(statusSheet);

    //三个按钮
    struct Sheet *buttonSheet = createSubsheetToTop(statusSheet, 10, 6, 50, 8);
    struct Sheet *backgroundOfButtonSheet = createSubsheetToTop(buttonSheet, 0, 0, 50, 8);
    fillVram(backgroundOfButtonSheet, COL8_FFFFFF);
    setFixedBottom(backgroundOfButtonSheet);

    struct Sheet *closeBtn = createSubsheetToTop(buttonSheet, 0, 0, 8, 8);
    initButtonCircle(closeBtn, 0, 0, COL8_FF0000);
    updateSheet(closeBtn);

    struct Sheet *minButton = createSubsheetToTop(buttonSheet, 12, 0, 8, 8);
    initButtonCircle(minButton, 0, 0, COL8_00FF00);
    updateSheet(minButton);

    struct Sheet *maxButton = createSubsheetToTop(buttonSheet, 24, 0, 8, 8);
    initButtonCircle(maxButton, 0, 0, COL8_0000FF);
    updateSheet(maxButton);

    updateSheet(backgroundOfButtonSheet);
    updateSheet(buttonSheet);

    setFixedTop(statusSheet);

    updateIndexMap(windowSheet);
    fillVramByIndexMap(windowSheet);
    updateSheet(windowSheet);

    //标题
    int labelX = width / 2 - getStringSize(title) * 8 / 2;
    if (labelX <= 45)
    {
        labelX = 45;
        title = "...";
    }
    struct Sheet *titleSheet = createLabelWithBackground(statusSheet, labelX, 2, getStringSize(title) * 8, 16, title, COL8_000000, COL8_FFFFFF);
    updateSheet(windowSheet);
    // printInSheet(statusSheet, width / 2 - getStringSize(title) * 8 / 2, 2, title, COL8_000000);

    // moveSheet(windowSheet, windowSheet->x, windowSheet->y);

    return windowSheet;
}