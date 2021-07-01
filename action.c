#include "action.h"

void handleOnClickOfRoot(unsigned int x, unsigned int y)
{
    handleOnClick(rootSheetManager.sheet, x - 1, y);
}

void handleOnMouseMoveOfRoot(unsigned int x, unsigned int y)
{
    handleOnMouseMove(rootSheetManager.sheet, x - 1, y);
}

void handleOnMouseLeftDownOfRoot(unsigned int x, unsigned int y)
{
    handleOnMouseLeftDown(rootSheetManager.sheet, x - 1, y);
}
void handleOnMouseLeftUpOfRoot(unsigned int x, unsigned int y)
{
    handleOnMouseLeftUp(rootSheetManager.sheet, x - 1, y);
}
void handleOnMouseRightDownOfRoot(unsigned int x, unsigned int y)
{
    handleOnMouseRightDown(rootSheetManager.sheet, x - 1, y);
}
void handleOnMouseRightUpOfRoot(unsigned int x, unsigned int y)
{
    handleOnMouseRightUp(rootSheetManager.sheet, x - 1, y);
}
void handleOnMouseMiddleDownOfRoot(unsigned int x, unsigned int y)
{
    handleOnMouseMiddleDown(rootSheetManager.sheet, x - 1, y);
}
void handleOnMouseMiddleUpOfRoot(unsigned int x, unsigned int y)
{
    handleOnMouseMiddleUp(rootSheetManager.sheet, x - 1, y);
}
void handleOnDoubleClickOfRoot(unsigned int x, unsigned int y)
{
    handleOnDoubleClick(rootSheetManager.sheet, x - 1, y);
}

void handleOnRightClickOfRoot(unsigned int x, unsigned int y)
{
    handleOnRightClick(rootSheetManager.sheet, x - 1, y);
}
void handleOnMiddleClickOfRoot(unsigned int x, unsigned int y)
{
    handleOnMiddleClick(rootSheetManager.sheet, x - 1, y);
}

// int i2 = 0;
void handleOnClick(struct Sheet *sheet, unsigned int x, unsigned int y)
{
    if (sheet->actionMap == NULL || sheet->actionMap[y * sheet->width + x] == 0 || y < 0 || x < 0)
    {
        // i2 = 0;
        return;
    }

    // char c[32];
    // sprintf(c, "%d", sheet->index);
    // setLabelText(statusLabel, c, COL8_848400);

    struct Sheet *targetSheet = sheet->sheetStore[sheet->actionMap[y * sheet->width + x]];

    if (targetSheet != NULL && targetSheet->actionManager != NULL && targetSheet->actionManager->onClick != NULL)
    {
        targetSheet->actionManager->onClick(targetSheet, x, y);
    }

    unsigned int subSheetX = x - targetSheet->x;
    unsigned int subSheetY = y - targetSheet->y;
    handleOnClick(targetSheet, subSheetX, subSheetY);
}

void handleOnMouseMove(struct Sheet *sheet, unsigned int x, unsigned int y)
{
    if (sheet->actionMap == NULL || sheet->actionMap[y * sheet->width + x] == 0 || y < 0 || x < 0)
    {
        return;
    }

    struct Sheet *targetSheet = sheet->sheetStore[sheet->actionMap[y * sheet->width + x]];
    if (targetSheet != NULL && targetSheet->actionManager != NULL && targetSheet->actionManager->onMouseIn != NULL)
    {
        targetSheet->actionManager->onMouseIn(targetSheet, x, y);
    }

    unsigned int subSheetX = x - targetSheet->x;
    unsigned int subSheetY = y - targetSheet->y;
    handleOnMouseMove(targetSheet, subSheetX, subSheetY);
}

void handleOnMouseLeftDown(struct Sheet *sheet, unsigned int x, unsigned int y)
{
    if (sheet->actionMap == NULL || sheet->actionMap[y * sheet->width + x] == 0 || y < 0 || x < 0)
    {
        return;
    }

    struct Sheet *targetSheet = sheet->sheetStore[sheet->actionMap[y * sheet->width + x]];

    if (targetSheet != NULL && targetSheet->actionManager != NULL && targetSheet->actionManager->onMouseLeftDown != NULL)
    {
        targetSheet->actionManager->onMouseLeftDown(targetSheet, x, y);
        windowsManager.lastMouseLeftDownSheet = targetSheet;
    }

    unsigned int subSheetX = x - targetSheet->x;
    unsigned int subSheetY = y - targetSheet->y;
    handleOnMouseLeftDown(targetSheet, subSheetX, subSheetY);
}

void handleOnMouseLeftUp(struct Sheet *sheet, unsigned int x, unsigned int y)
{
    if (windowsManager.lastMouseLeftDownSheet != NULL && windowsManager.lastMouseLeftDownSheet->actionManager != NULL &&
        windowsManager.lastMouseLeftDownSheet->actionManager->onMouseLeftUp != NULL)
    {
        windowsManager.lastMouseLeftDownSheet->actionManager->onMouseLeftUp(sheet, x, y);
        windowsManager.lastMouseLeftDownSheet = NULL;
    }

    windowsManager.isDragging = false;

    if (sheet->actionMap == NULL || sheet->actionMap[y * sheet->width + x] == 0 || y < 0 || x < 0)
    {
        return;
    }

    struct Sheet *targetSheet = sheet->sheetStore[sheet->actionMap[y * sheet->width + x]];
    if (targetSheet != NULL && targetSheet->actionManager != NULL && targetSheet->actionManager->onMouseLeftUp != NULL)
    {
        targetSheet->actionManager->onMouseLeftUp(targetSheet, x, y);
    }

    unsigned int subSheetX = x - targetSheet->x;
    unsigned int subSheetY = y - targetSheet->y;
    handleOnMouseLeftUp(targetSheet, subSheetX, subSheetY);
}

void handleOnMouseRightDown(struct Sheet *sheet, unsigned int x, unsigned int y)
{
    if (sheet->actionMap == NULL || sheet->actionMap[y * sheet->width + x] == 0 || y < 0 || x < 0)
    {
        return;
    }

    struct Sheet *targetSheet = sheet->sheetStore[sheet->actionMap[y * sheet->width + x]];
    if (targetSheet != NULL && targetSheet->actionManager != NULL && targetSheet->actionManager->onMouseRightDown != NULL)
    {
        targetSheet->actionManager->onMouseRightDown(targetSheet, x, y);
    }

    unsigned int subSheetX = x - targetSheet->x;
    unsigned int subSheetY = y - targetSheet->y;
    handleOnMouseRightDown(targetSheet, subSheetX, subSheetY);
}

void handleOnMouseRightUp(struct Sheet *sheet, unsigned int x, unsigned int y)
{
    if (sheet->actionMap == NULL || sheet->actionMap[y * sheet->width + x] == 0 || y < 0 || x < 0)
    {
        return;
    }

    struct Sheet *targetSheet = sheet->sheetStore[sheet->actionMap[y * sheet->width + x]];
    if (targetSheet != NULL && targetSheet->actionManager != NULL && targetSheet->actionManager->onMouseRightUp != NULL)
    {
        targetSheet->actionManager->onMouseRightUp(targetSheet, x, y);
    }

    unsigned int subSheetX = x - targetSheet->x;
    unsigned int subSheetY = y - targetSheet->y;
    handleOnMouseRightUp(targetSheet, subSheetX, subSheetY);
}

void handleOnMouseMiddleDown(struct Sheet *sheet, unsigned int x, unsigned int y)
{
    if (sheet->actionMap == NULL || sheet->actionMap[y * sheet->width + x] == 0 || y < 0 || x < 0)
    {
        return;
    }

    struct Sheet *targetSheet = sheet->sheetStore[sheet->actionMap[y * sheet->width + x]];
    if (targetSheet != NULL && targetSheet->actionManager != NULL && targetSheet->actionManager->onMouseMiddleDown != NULL)
    {
        targetSheet->actionManager->onMouseMiddleDown(targetSheet, x, y);
    }

    unsigned int subSheetX = x - targetSheet->x;
    unsigned int subSheetY = y - targetSheet->y;
    handleOnMouseMiddleDown(targetSheet, subSheetX, subSheetY);
}

void handleOnMouseMiddleUp(struct Sheet *sheet, unsigned int x, unsigned int y)
{
    if (sheet->actionMap == NULL || sheet->actionMap[y * sheet->width + x] == 0 || y < 0 || x < 0)
    {
        return;
    }

    struct Sheet *targetSheet = sheet->sheetStore[sheet->actionMap[y * sheet->width + x]];
    if (targetSheet != NULL && targetSheet->actionManager != NULL && targetSheet->actionManager->onMouseMiddleUp != NULL)
    {
        targetSheet->actionManager->onMouseMiddleUp(targetSheet, x, y);
    }

    unsigned int subSheetX = x - targetSheet->x;
    unsigned int subSheetY = y - targetSheet->y;
    handleOnMouseMiddleUp(targetSheet, subSheetX, subSheetY);
}

void handleOnDoubleClick(struct Sheet *sheet, unsigned int x, unsigned int y)
{
    if (sheet->actionMap == NULL || sheet->actionMap[y * sheet->width + x] == 0 || y < 0 || x < 0)
    {
        return;
    }

    struct Sheet *targetSheet = sheet->sheetStore[sheet->actionMap[y * sheet->width + x]];
    if (targetSheet != NULL && targetSheet->actionManager != NULL && targetSheet->actionManager->onDoubleClick != NULL)
    {
        targetSheet->actionManager->onDoubleClick(targetSheet, x, y);
    }

    unsigned int subSheetX = x - targetSheet->x;
    unsigned int subSheetY = y - targetSheet->y;
    handleOnDoubleClick(targetSheet, subSheetX, subSheetY);
}

void handleOnRightClick(struct Sheet *sheet, unsigned int x, unsigned int y)
{
    if (sheet->actionMap == NULL || sheet->actionMap[y * sheet->width + x] == 0 || y < 0 || x < 0)
    {
        return;
    }

    struct Sheet *targetSheet = sheet->sheetStore[sheet->actionMap[y * sheet->width + x]];
    if (targetSheet != NULL && targetSheet->actionManager != NULL && targetSheet->actionManager->onRightClick != NULL)
    {
        targetSheet->actionManager->onRightClick(targetSheet, x, y);
    }

    unsigned int subSheetX = x - targetSheet->x;
    unsigned int subSheetY = y - targetSheet->y;
    handleOnRightClick(targetSheet, subSheetX, subSheetY);
}

void handleOnMiddleClick(struct Sheet *sheet, unsigned int x, unsigned int y)
{
    if (sheet->actionMap == NULL || sheet->actionMap[y * sheet->width + x] == 0 || y < 0 || x < 0)
    {
        return;
    }

    struct Sheet *targetSheet = sheet->sheetStore[sheet->actionMap[y * sheet->width + x]];
    if (targetSheet != NULL && targetSheet->actionManager != NULL && targetSheet->actionManager->onMiddleClick != NULL)
    {
        targetSheet->actionManager->onMiddleClick(targetSheet, x, y);
    }

    unsigned int subSheetX = x - targetSheet->x;
    unsigned int subSheetY = y - targetSheet->y;
    handleOnMiddleClick(targetSheet, subSheetX, subSheetY);
}
