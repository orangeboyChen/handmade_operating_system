#include "action.h"

void handleOnClickOfRoot(unsigned int x, unsigned int y)
{
    deleteWindowsManagerActiveSheetItems();
    struct ActiveSheetItem *item = getActiveSheetItem();
    item->sheet = rootSheetManager.sheet;
    windowsManager.firstActiveSheet = item;
    windowsManager.lastActiveSheet = item;
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

    struct ActiveSheetItem *item = getActiveSheetItem();
    item->sheet = targetSheet;
    item->previousSheet = windowsManager.lastActiveSheet;
    windowsManager.lastActiveSheet->nextSheet = item;
    windowsManager.lastActiveSheet = item;

    if (targetSheet != NULL && targetSheet->systemActionManager != NULL && targetSheet->systemActionManager->onClick != NULL)
    {
        targetSheet->systemActionManager->onClick(targetSheet, x, y);
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
    if (targetSheet != NULL && targetSheet->systemActionManager != NULL && targetSheet->systemActionManager->onMouseIn != NULL)
    {
        targetSheet->systemActionManager->onMouseIn(targetSheet, x, y);
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

    // if (sheet != rootSheetManager.sheet)
    // {
    //     char c[32];
    //     sprintf(c, "%d", sheet->index);
    //     setLabelText(statusLabel, c, COL8_848400);

    //     // if (sheet->index == 6)
    //     // {
    //     //     fillInSheet(sheet, 0, 0, sheet->width, sheet->height, COL8_000000);
    //     // }
    // }

    struct Sheet *targetSheet = sheet->sheetStore[sheet->actionMap[y * sheet->width + x]];

    if (targetSheet != NULL && targetSheet->systemActionManager != NULL && targetSheet->systemActionManager->onMouseLeftDown != NULL)
    {
        targetSheet->systemActionManager->onMouseLeftDown(targetSheet, x, y);
        windowsManager.lastMouseLeftDownSheet = targetSheet;
    }

    unsigned int subSheetX = x - targetSheet->x;
    unsigned int subSheetY = y - targetSheet->y;
    handleOnMouseLeftDown(targetSheet, subSheetX, subSheetY);
}

void handleOnMouseLeftUp(struct Sheet *sheet, unsigned int x, unsigned int y)
{
    if (windowsManager.lastMouseLeftDownSheet != NULL && windowsManager.lastMouseLeftDownSheet->systemActionManager != NULL &&
        windowsManager.lastMouseLeftDownSheet->systemActionManager->onMouseLeftUp != NULL)
    {
        windowsManager.lastMouseLeftDownSheet->systemActionManager->onMouseLeftUp(sheet, x, y);
        windowsManager.lastMouseLeftDownSheet = NULL;
    }

    windowsManager.isDragging = false;

    if (sheet->actionMap == NULL || sheet->actionMap[y * sheet->width + x] == 0 || y < 0 || x < 0)
    {
        return;
    }

    struct Sheet *targetSheet = sheet->sheetStore[sheet->actionMap[y * sheet->width + x]];
    if (targetSheet != NULL && targetSheet->systemActionManager != NULL && targetSheet->systemActionManager->onMouseLeftUp != NULL)
    {
        targetSheet->systemActionManager->onMouseLeftUp(targetSheet, x, y);
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
    if (targetSheet != NULL && targetSheet->systemActionManager != NULL && targetSheet->systemActionManager->onMouseRightDown != NULL)
    {
        targetSheet->systemActionManager->onMouseRightDown(targetSheet, x, y);
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
    if (targetSheet != NULL && targetSheet->systemActionManager != NULL && targetSheet->systemActionManager->onMouseRightUp != NULL)
    {
        targetSheet->systemActionManager->onMouseRightUp(targetSheet, x, y);
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
    if (targetSheet != NULL && targetSheet->systemActionManager != NULL && targetSheet->systemActionManager->onMouseMiddleDown != NULL)
    {
        targetSheet->systemActionManager->onMouseMiddleDown(targetSheet, x, y);
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
    if (targetSheet != NULL && targetSheet->systemActionManager != NULL && targetSheet->systemActionManager->onMouseMiddleUp != NULL)
    {
        targetSheet->systemActionManager->onMouseMiddleUp(targetSheet, x, y);
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
    if (targetSheet != NULL && targetSheet->systemActionManager != NULL && targetSheet->systemActionManager->onDoubleClick != NULL)
    {
        targetSheet->systemActionManager->onDoubleClick(targetSheet, x, y);
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
    if (targetSheet != NULL && targetSheet->systemActionManager != NULL && targetSheet->systemActionManager->onRightClick != NULL)
    {
        targetSheet->systemActionManager->onRightClick(targetSheet, x, y);
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
    if (targetSheet != NULL && targetSheet->systemActionManager != NULL && targetSheet->systemActionManager->onMiddleClick != NULL)
    {
        targetSheet->systemActionManager->onMiddleClick(targetSheet, x, y);
    }

    unsigned int subSheetX = x - targetSheet->x;
    unsigned int subSheetY = y - targetSheet->y;
    handleOnMiddleClick(targetSheet, subSheetX, subSheetY);
}

// int i3 = 0;
void handleKeyPress(char key, unsigned int raw)
{

    struct ActiveSheetItem *currentItem = windowsManager.firstActiveSheet;

    while (currentItem != NULL)
    {
        if (currentItem->sheet->systemActionManager != NULL && currentItem->sheet->systemActionManager->onKeyPress != NULL)
        {
            // i3++;
            // char s4[32];
            // sprintf(s4, "%d", i3);
            // setLabelText(statusLabel, s4, COL8_FFFFFF);
            currentItem->sheet->systemActionManager->onKeyPress(currentItem->sheet, key, raw);
        }
        currentItem = currentItem->nextSheet;
    }
}
void handleKeyUp(char key, unsigned int raw)
{
    struct ActiveSheetItem *currentItem = windowsManager.firstActiveSheet;
    while (currentItem != NULL)
    {
        if (currentItem->sheet->systemActionManager != NULL && currentItem->sheet->systemActionManager->onKeyUp != NULL)
        {
            currentItem->sheet->systemActionManager->onKeyUp(currentItem->sheet, key, raw);
        }
        currentItem = currentItem->nextSheet;
    }
}