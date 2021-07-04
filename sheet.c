#include "sheet.h"
struct RootSheetManager rootSheetManager;

struct Sheet *initRootSheet()
{
    struct BootInfo *bootInfo = getBootInfo();
    struct Sheet *rootSheet = (struct Sheet *)allocaMemory(getMemoryManager(), sizeof(struct Sheet));
    rootSheetManager.sheet = rootSheet;

    rootSheet->x = 0;
    rootSheet->y = 0;
    rootSheet->width = bootInfo->screenX;
    rootSheet->height = bootInfo->screenY;
    rootSheet->vram = bootInfo->vram;
    rootSheet->series = 999;

    rootSheet->indexMap = allocaMemory(getMemoryManager(), rootSheet->width * rootSheet->height * sizeof(int));
    rootSheet->actionMap = allocaMemory(getMemoryManager(), rootSheet->width * rootSheet->height * sizeof(int));
    rootSheet->updateMap = allocaMemory(getMemoryManager(), (rootSheet->width * rootSheet->height - 1) / sizeof(char) + 1);

    //准备背景图层
    struct Sheet *backgroundSheet = createSubsheetToTop(rootSheet, 0, 0, rootSheet->width, rootSheet->height);
    rootSheetManager.backgroundSheet = backgroundSheet;

    //刷新视图
    fillVram(backgroundSheet, COL8_840084);
    setFixedBottom(backgroundSheet);
    forceUpdateSheet(backgroundSheet);

    //添加事件
    backgroundSheet->systemActionManager = allocaMemory(getMemoryManager(), sizeof(struct ActionManager));
    backgroundSheet->systemActionManager->onClick = &onBackgroundClick;

    return rootSheet;
}

void onBackgroundClick()
{
    disactiveSheetWindow(windowsManager.currentActiveWindow->sheet);
    windowsManager.isActiveShowing = false;
    setLabelText(rootSheetManager.titleLabel, "Desktop", COL8_000000);
}

struct Sheet *createSubsheetToTop(struct Sheet *fatherSheet, short x, short y, short width, short height)
{
    return createSubsheetToTopWithVram(fatherSheet, x, y, width, height, allocaMemory(getMemoryManager(), sizeof(char) * width * height));
}

// int i = 0;
struct Sheet *createSubsheetToTopWithVram(struct Sheet *fatherSheet, short x, short y, short width, short height, char *vram)
{
    struct Sheet *newSheet = allocaMemory(getMemoryManager(), sizeof(struct Sheet));
    newSheet->x = x;
    newSheet->y = y;
    newSheet->width = width;
    newSheet->height = height;
    newSheet->vram = vram;
    newSheet->indexMap = allocaMemory(getMemoryManager(), width * height * sizeof(int));
    newSheet->actionMap = allocaMemory(getMemoryManager(), width * height * sizeof(int));

    // if (newSheet == NULL || newSheet->indexMap == NULL || newSheet->actionMap == NULL)
    // {
    //     return NULL;
    // }

    //按位存储节省空间
    newSheet->updateMap = allocaMemory(getMemoryManager(), (width * height - 1) / sizeof(char) + 1 + 1);

    newSheet->fatherSheet = fatherSheet;

    int i;
    for (i = 1; i < SHEET_MAX; i++)
    {
        if (fatherSheet->sheetStore[i] == NULL)
        {
            newSheet->index = i;
            fatherSheet->sheetStore[i] = newSheet;
            break;
        }
    }

    //整理父图层，更新更新数组
    int j;
    int toHeight = y + height >= fatherSheet->height ? fatherSheet->height : y + height;
    int toWidth = x + width >= fatherSheet->width ? fatherSheet->width : x + width;
    for (i = y; i < toHeight; i++)
    {
        for (j = x; j < toWidth; j++)
        {
            if (i >= fatherSheet->height || j >= fatherSheet->width)
            {
                continue;
            }
            unsigned int fatherIndex = i * fatherSheet->width + j;
            // y * fatherSheet->width + x
            //(y + i) * fatherSheet->width + x + j
            fatherSheet->indexMap[fatherIndex] = newSheet->index;
            setBitInUpdateMap(fatherSheet, fatherIndex, true);
        }
    }

    if (fatherSheet->topSheet == NULL || fatherSheet->bottomSheet == NULL)
    {
        fatherSheet->topSheet = fatherSheet->bottomSheet = newSheet;
    }
    else
    {
        struct Sheet *currentSheet = fatherSheet->topSheet;
        while (true)
        {
            if (getFixedTop(currentSheet) == false)
            {
                break;
            }
            currentSheet = currentSheet->nextSheet;
        }

        if (currentSheet == NULL)
        {
            currentSheet = fatherSheet->bottomSheet;
        }

        newSheet->nextSheet = currentSheet;
        newSheet->previousSheet = currentSheet->previousSheet;

        if (newSheet->nextSheet != NULL)
        {
            newSheet->nextSheet->previousSheet = newSheet;
        }

        if (newSheet->previousSheet != NULL)
        {
            newSheet->previousSheet->nextSheet = newSheet;
        }

        if (fatherSheet->topSheet == newSheet->nextSheet)
        {
            fatherSheet->topSheet = newSheet;
        }
    }

    return newSheet;
}

void setBitInUpdateMap(struct Sheet *sheet, unsigned int index, unsigned int value)
{
    char *butch = &sheet->updateMap[index / sizeof(char)];
    unsigned int indexInChar = index - (index / sizeof(char)) * sizeof(char);
    if (value == true)
    {
        *butch = *butch | (1 << indexInChar);
    }
    else
    {
        *butch = *butch & (!(1 << indexInChar));
    }
}

unsigned int getBitInUpdateMap(struct Sheet *sheet, unsigned int index)
{
    char *butch = &sheet->updateMap[index / sizeof(char)];
    unsigned int indexInChar = index - (index / sizeof(char)) * sizeof(char);
    return (*butch & (1 << indexInChar)) >> indexInChar;
}

void fillVram(struct Sheet *sheet, unsigned char c)
{
    int i, j;
    for (i = 0; i < sheet->height; i++)
    {
        for (j = 0; j < sheet->width; j++)
        {
            sheet->vram[i * sheet->width + j] = c;
            setBitInUpdateMap(sheet, i * sheet->width + j, true);
        }
    }
}

void moveSheet(struct Sheet *sheet, short x1, short y1)
{
    short oldX = sheet->x;
    short oldY = sheet->y;
    sheet->x = x1;
    sheet->y = y1;

    // if (oldX == x1 && oldY == y1)
    // {
    //     return;
    // }

    int toX = x1 + sheet->width - 1;
    int toY = y1 + sheet->height - 1;
    int oldToX = oldX + sheet->width - 1;
    int oldToY = oldY + sheet->height - 1;

    if (x1 <= 0)
    {
        x1 = 0;
    }

    if (y1 <= 0)
    {
        y1 = 0;
    }

    if (sheet->x <= 0)
    {
        sheet->x = 0;
    }

    if (sheet->y <= 0)
    {
        sheet->y = 0;
    }

    if (toX >= sheet->fatherSheet->width)
    {
        toX = sheet->fatherSheet->width - 1;
    }

    if (toY >= sheet->fatherSheet->height)
    {
        toY = sheet->fatherSheet->height - 1;
    }

    if (oldToX >= sheet->fatherSheet->width)
    {
        oldToX = sheet->fatherSheet->width - 1;
    }

    if (oldToY >= sheet->fatherSheet->height)
    {
        oldToY = sheet->fatherSheet->height - 1;
    }

    updatePartOfIndexMap(sheet->fatherSheet, x1, y1, toX, toY);
    fillPartOfVramByIndexMap(sheet->fatherSheet, x1, y1, toX, toY);

    updatePartOfIndexMap(sheet->fatherSheet, oldX, oldY, oldToX, oldToY);
    fillPartOfVramByIndexMap(sheet->fatherSheet, oldX, oldY, oldToX, oldToY);

    // updateAllSubsheet(sheet->fatherSheet->fatherSheet);
    // updateSheet(sheet);
    updateSheet(sheet->fatherSheet);
    // forceUpdateSheet(sheet);
}

void updatePartOfIndexMap(struct Sheet *sheet, short fromX, short fromY, short toX, short toY)
{
    int x, y;
    for (y = fromY; y <= toY; y++)
    {
        for (x = fromX; x <= toX; x++)
        {
            sheet->indexMap[y * sheet->width + x] = 0;
            sheet->actionMap[y * sheet->width + x] = 0;
        }
    }

    struct Sheet *currentSheet = sheet->topSheet;
    while (currentSheet != NULL)
    {
        int x, y;

        for (y = currentSheet->y; y < currentSheet->y + currentSheet->height; y++)
        {
            for (x = currentSheet->x; x < currentSheet->x + currentSheet->width; x++)
            {
                if (y < fromY || y > toY || x < fromX || x > toX)
                {
                    continue;
                }

                if (x >= sheet->width || y >= sheet->height)
                {
                    continue;
                }

                // if (y > sheet->height || x > sheet->width)
                // {
                //     continue;
                // }
                unsigned int currentSheetIndex = (y - currentSheet->y) * currentSheet->width + (x - currentSheet->x);
                if (sheet->indexMap[y * sheet->width + x] == 0 || sheet->indexMap[y * sheet->width + x] == TRANSPARENT_INDEX)
                {
                    if (currentSheet->vram[currentSheetIndex] == COL_TRANSPARENT || currentSheet->indexMap[currentSheetIndex] == TRANSPARENT_INDEX)
                    {
                        sheet->indexMap[y * sheet->width + x] = TRANSPARENT_INDEX;
                    }
                    else
                    {
                        sheet->indexMap[y * sheet->width + x] = currentSheet->index;
                    }
                    sheet->actionMap[y * sheet->width + x] = currentSheet->index;
                }

                if (currentSheet->vram[currentSheetIndex] == COL_FORCE_TRANSPARENT || currentSheet->indexMap[currentSheetIndex] == FORCE_TRANSPARENT_INDEX)
                {
                    sheet->indexMap[y * sheet->width + x] = FORCE_TRANSPARENT_INDEX;
                    sheet->actionMap[y * sheet->width + x] = FORCE_TRANSPARENT_INDEX;
                }
            }
        }
        currentSheet = currentSheet->nextSheet;
    }
    int i, j;
    for (i = 0; i < sheet->height; i++)
    {
        for (j = 0; j < sheet->width; j++)
        {
            if (sheet->indexMap[i * sheet->width + j] == FORCE_TRANSPARENT_INDEX)
            {
                sheet->indexMap[i * sheet->width + j] = TRANSPARENT_INDEX;
                sheet->actionMap[i * sheet->width + j] = TRANSPARENT_INDEX;
            }
        }
    }
}

void updateIndexMapAndActionMap(struct Sheet *sheet)
{
    if (sheet->topSheet == NULL)
    {
        return;
    }

    // char *temp = allocaMemory(getMemoryManager(), sheet->height * sheet->width * sizeof(char));

    memset(sheet->indexMap, 0, sizeof(int) * sheet->height * sheet->width);
    memset(sheet->actionMap, 0, sizeof(int) * sheet->height * sheet->width);
    struct Sheet *currentSheet = sheet->topSheet;
    while (currentSheet != NULL)
    {
        int x, y;
        for (y = currentSheet->y; y < currentSheet->y + currentSheet->height; y++)
        {
            for (x = currentSheet->x; x < currentSheet->x + currentSheet->width; x++)
            {
                if (x >= sheet->width || y >= sheet->height || x < 0 || y < 0)
                {
                    continue;
                }

                unsigned int currentSheetIndex = (y - currentSheet->y) * currentSheet->width + (x - currentSheet->x);
                if (sheet->indexMap[y * sheet->width + x] == 0 || sheet->indexMap[y * sheet->width + x] == TRANSPARENT_INDEX)
                {
                    if (currentSheet->vram[currentSheetIndex] == COL_TRANSPARENT || currentSheet->indexMap[currentSheetIndex] == TRANSPARENT_INDEX)
                    {
                        sheet->indexMap[y * sheet->width + x] = TRANSPARENT_INDEX;
                    }
                    else
                    {
                        sheet->indexMap[y * sheet->width + x] = currentSheet->index;
                    }
                    sheet->actionMap[y * sheet->width + x] = currentSheet->index;
                }

                if (currentSheet->vram[currentSheetIndex] == COL_FORCE_TRANSPARENT || currentSheet->indexMap[currentSheetIndex] == FORCE_TRANSPARENT_INDEX)
                {
                    sheet->indexMap[y * sheet->width + x] = FORCE_TRANSPARENT_INDEX;
                    sheet->actionMap[y * sheet->width + x] = FORCE_TRANSPARENT_INDEX;
                }
            }
        }
        currentSheet = currentSheet->nextSheet;
    }

    int i, j;
    for (i = 0; i < sheet->height; i++)
    {
        for (j = 0; j < sheet->width; j++)
        {
            if (sheet->indexMap[i * sheet->width + j] == FORCE_TRANSPARENT_INDEX)
            {
                sheet->indexMap[i * sheet->width + j] = TRANSPARENT_INDEX;
            }
        }
    }
}

void forceUpdateSheet(struct Sheet *sheet)
{
    updateIndexMapAndActionMap(sheet->fatherSheet);
    int i, j;
    for (i = 0; i < sheet->height; i++)
    {
        for (j = 0; j < sheet->width; j++)
        {
            setBitInUpdateMap(sheet, i * sheet->width + j, true);
        }
    }
    updateSheet(sheet);
}

void updateAllSubsheet(struct Sheet *sheet)
{
    updateAllSubsheetWithFather(sheet, sheet);
}

void updateAllSubsheetWithFather(struct Sheet *sheet, struct Sheet *fatherSheet)
{
    if (sheet->topSheet != NULL)
    {
        updateAllSubsheetWithFather(sheet->topSheet, fatherSheet);
    }

    if (sheet->nextSheet != NULL)
    {
        updateAllSubsheetWithFather(sheet->nextSheet, fatherSheet);
    }

    if (fatherSheet != sheet)
    {
        updateSingleSheetIndexAndVramInFatherSheet(sheet);
    }
    else
    {
        updateSheet(fatherSheet);
    }
}

void updateSingleSheetIndexAndVramInFatherSheet(struct Sheet *sheet)
{
    struct Sheet *fatherSheet = sheet->fatherSheet;
    int x, y;
    char isUpdate = false;
    for (y = sheet->y; y < sheet->height + sheet->y; y++)
    {
        for (x = sheet->x; x < sheet->width + sheet->x; x++)
        {
            unsigned int fatherIndex = y * fatherSheet->width + x;
            unsigned int sonIndex = (y - sheet->y) * sheet->width + (x - sheet->x);
            fatherSheet->indexMap[fatherIndex] = sheet->index;

            if (sheet->vram[sonIndex] != COL_TRANSPARENT)
            {
                fatherSheet->vram[fatherIndex] = sheet->vram[sonIndex];
            }

            if ((sheet->vram[sonIndex] == COL_TRANSPARENT && sheet->nextSheet == NULL) || (sheet->vram[sonIndex] == COL_FORCE_TRANSPARENT))
            {
                fatherSheet->vram[fatherIndex] = COL_TRANSPARENT;
            }
            // fatherSheet->vram[fatherIndex] = sheet->vram[sonIndex];

            setBitInUpdateMap(fatherSheet, fatherIndex, true);
        }
    }
}

void updateSheetWithTransparent(struct Sheet *sheet, int isTransparent)
{
    struct Sheet *fatherSheet = sheet->fatherSheet;
    if (fatherSheet == NULL)
    {
        return;
    }

    if (isTransparent == true)
    {
        updateIndexMapAndActionMap(fatherSheet);
    }

    int x, y;
    char isUpdate = false;
    int currentIsTransparent = false;
    for (y = sheet->y; y < sheet->height + sheet->y; y++)
    {
        for (x = sheet->x; x < sheet->width + sheet->x; x++)
        {
            if (x > fatherSheet->width || y > fatherSheet->height)
            {
                continue;
            }
            unsigned int fatherIndex = y * fatherSheet->width + x;
            unsigned int sonIndex = (y - sheet->y) * sheet->width + (x - sheet->x);

            // if (fatherSheet->indexMap[fatherIndex] == FORCE_TRANSPARENT_INDEX)
            // {
            //     fatherSheet->indexMap[fatherIndex] = TRANSPARENT_INDEX;
            //     fatherSheet->vram[fatherIndex] = COL_TRANSPARENT;
            // }

            if ((fatherSheet->indexMap[fatherIndex] == sheet->index || sheet->vram[sonIndex] == COL_TRANSPARENT || sheet->vram[sonIndex] == COL_FORCE_TRANSPARENT) && getBitInUpdateMap(sheet, sonIndex) == true)
            {
                isUpdate = true;
                setBitInUpdateMap(fatherSheet, fatherIndex, true);
                struct Sheet *currentSheet = fatherSheet->sheetStore[fatherSheet->indexMap[fatherIndex]];

                fatherSheet->vram[fatherIndex] = currentSheet->vram[(y - currentSheet->y) * currentSheet->width + (x - currentSheet->x)];

                // if (fatherSheet->vram[fatherIndex] == COL_FORCE_TRANSPARENT)
                // {
                //     fatherSheet->indexMap[fatherIndex] = TRANSPARENT_INDEX;
                //     fatherSheet->vram[fatherIndex] = COL_TRANSPARENT;
                // }

                if (sheet->vram[sonIndex] == COL_TRANSPARENT || sheet->vram[sonIndex] == COL_FORCE_TRANSPARENT)
                {
                    currentIsTransparent = true;
                }
                setBitInUpdateMap(sheet, sonIndex, false);
            }
        }
    }

    // if (!isUpdate)
    // {
    //     return;
    // }

    if (fatherSheet->fatherSheet != NULL)
    {
        updateSheetWithTransparent(fatherSheet, currentIsTransparent);
        // updateSheetWithTransparent(fatherSheet, false);
    }
    else
    {
        // releaseBlock(getMemoryManager(), sheet->updateMap);
        // sheet->updateMap = allocaMemory(getMemoryManager(), (sheet->width * sheet->height - 1) / sizeof(char) + 1);

        // int i, j;
        // for (i = 0; i < sheet->height; i++)
        // {
        //     for (j = 0; j < sheet->width; j++)
        //     {
        //         setBitInUpdateMap(sheet, i * sheet->width + j, false);
        //     }
        // }

        memset(sheet->updateMap, 0, (sheet->width * sheet->height - 1) / sizeof(char) + 1 + 1);
    }
}

void updateSheet(struct Sheet *sheet)
{
    updateSheetWithTransparent(sheet, true);
}

char getFixedTop(struct Sheet *sheet)
{
    return (sheet->status & (1 << 5)) == 1 << 5;
}

char getFixedBottom(struct Sheet *sheet)
{
    return (sheet->status & (1 << 6)) == 1 << 6;
}

void moveSheetToTop(struct Sheet *sheet)
{
    struct Sheet *fatherSheet = sheet->fatherSheet;

    struct Sheet *currentSheet = fatherSheet->topSheet;
    while (currentSheet->nextSheet != NULL)
    {
        if (currentSheet == sheet)
        {
            currentSheet = currentSheet->nextSheet;
            continue;
        }
        if (currentSheet->nextSheet == NULL || getFixedTop(currentSheet) == false)
        {
            break;
        }
        currentSheet = currentSheet->nextSheet;
    }

    if (sheet->previousSheet != NULL)
    {
        sheet->previousSheet->nextSheet = sheet->nextSheet;
    }

    if (sheet->nextSheet != NULL)
    {
        sheet->nextSheet->previousSheet = sheet->previousSheet;
    }

    sheet->previousSheet = currentSheet->previousSheet;
    sheet->nextSheet = currentSheet;

    if (sheet->previousSheet != NULL)
    {
        sheet->previousSheet->nextSheet = sheet;
    }

    if (sheet->nextSheet != NULL)
    {
        sheet->nextSheet->previousSheet = sheet;
    }

    if (sheet->nextSheet == sheet)
    {
        sheet->nextSheet = NULL;
    }

    if (sheet->previousSheet == sheet)
    {
        sheet->previousSheet = NULL;
    }

    unsigned int toX = sheet->x + sheet->width - 1 > fatherSheet->width ? fatherSheet->width : sheet->x + sheet->width - 1;
    unsigned int toY = sheet->y + sheet->height - 1 > fatherSheet->height ? fatherSheet->height : sheet->y + sheet->height - 1;

    updatePartOfIndexMap(fatherSheet, sheet->x, sheet->y, toX, toY);
    // updatePartOfIndexMap(fatherSheet, sheet->nextSheet->x, sheet->nextSheet->y,
    //                      sheet->nextSheet->x + sheet->nextSheet->width, sheet->nextSheet->y + sheet->nextSheet->height);
    // updateIndexMapAndActionMap(fatherSheet);
    fillVramByIndexMap(fatherSheet);
    updateSheet(fatherSheet);
}

void setFixedTop(struct Sheet *sheet)
{
    struct Sheet *fatherSheet = sheet->fatherSheet;

    struct Sheet *currentSheet = fatherSheet->topSheet;
    while (currentSheet->nextSheet != NULL)
    {
        if (currentSheet == sheet)
        {
            currentSheet = currentSheet->nextSheet;
            continue;
        }
        if (currentSheet->nextSheet == NULL || getFixedTop(currentSheet) == false)
        {
            break;
        }
        currentSheet = currentSheet->nextSheet;
    }

    if (sheet->previousSheet != NULL)
    {
        sheet->previousSheet->nextSheet = sheet->nextSheet;
    }

    if (sheet->nextSheet != NULL)
    {
        sheet->nextSheet->previousSheet = sheet->previousSheet;
    }

    sheet->previousSheet = currentSheet->previousSheet;
    sheet->nextSheet = currentSheet;

    if (sheet->previousSheet != NULL)
    {
        sheet->previousSheet->nextSheet = sheet;
    }

    if (sheet->nextSheet != NULL)
    {
        sheet->nextSheet->previousSheet = sheet;
    }

    sheet->status = sheet->status | (1 << 5);

    if (sheet->nextSheet == sheet)
    {
        sheet->nextSheet = NULL;
    }

    if (sheet->previousSheet == sheet)
    {
        sheet->previousSheet = NULL;
    }

    updateIndexMapAndActionMap(fatherSheet);
}

void setFixedBottom(struct Sheet *sheet)
{
    struct Sheet *fatherSheet = sheet->fatherSheet;

    struct Sheet *currentSheet = fatherSheet->bottomSheet;
    while (currentSheet->previousSheet != NULL && getFixedBottom(currentSheet) == true)
    {
        currentSheet = currentSheet->previousSheet;
    }

    if (sheet->previousSheet != NULL)
    {
        sheet->previousSheet->nextSheet = sheet->nextSheet;
    }

    if (sheet->nextSheet != NULL)
    {
        sheet->nextSheet->previousSheet = sheet->previousSheet;
    }

    sheet->nextSheet = currentSheet->nextSheet;
    sheet->previousSheet = currentSheet;

    sheet->previousSheet->nextSheet = sheet;
    sheet->nextSheet->previousSheet = sheet;
    sheet->status = sheet->status | (1 << 6);

    if (sheet->nextSheet == sheet->previousSheet)
    {
        sheet->nextSheet = NULL;
        sheet->previousSheet = NULL;
    }

    // char s4[32];
    // sprintf(s4, "System is booting");
    // putfonts8_asc(getBootInfo()->vram, getBootInfo()->screenX, 0, 0, COL8_FFFFFF, s4);

    updateIndexMapAndActionMap(fatherSheet);
}

void printInSheet(struct Sheet *sheet, short x, short y, char *c, unsigned int colorConstant)
{
    putfonts8_asc(sheet->vram, sheet->width, x, y, colorConstant, c);

    forceUpdateSheet(sheet);
}

void fillInSheet(struct Sheet *sheet, short x, short y, short width, short height, unsigned int colorConstant)
{
    int i, j;
    for (i = y; i < height + y; i++)
    {
        for (j = x; j < width + x; j++)
        {
            sheet->vram[i * sheet->width + j] = colorConstant;
            setBitInUpdateMap(sheet, i * sheet->width + j, true);
        }
    }
}

void fillVramByIndexMap(struct Sheet *sheet)
{
    int x, y;
    for (y = 0; y < sheet->height; y++)
    {
        for (x = 0; x < sheet->width; x++)
        {
            if (sheet->indexMap[y * sheet->width + x] != 0)
            {
                struct Sheet *assistSheet = sheet->sheetStore[sheet->indexMap[y * sheet->width + x]];
                if (sheet->indexMap[y * sheet->width + x] == TRANSPARENT_INDEX)
                {
                    sheet->vram[y * sheet->width + x] = COL_TRANSPARENT;
                }
                else if (sheet->indexMap[y * sheet->width + x] == FORCE_TRANSPARENT_INDEX)
                {
                    sheet->vram[y * sheet->width + x] = COL_TRANSPARENT;
                }
                else
                {
                    sheet->vram[y * sheet->width + x] = assistSheet->vram[(y - assistSheet->y) * assistSheet->width + (x - assistSheet->x)];
                }
                setBitInUpdateMap(sheet, y * sheet->width + x, true);
            }
        }
    }
}

void fillPartOfVramByIndexMap(struct Sheet *sheet, short fromX, short fromY, short toX, short toY)
{
    int x, y;
    for (y = fromY; y <= toY; y++)
    {
        for (x = fromX; x <= toX; x++)
        {
            if (sheet->indexMap[y * sheet->width + x] != 0)
            {
                struct Sheet *assistSheet = sheet->sheetStore[sheet->indexMap[y * sheet->width + x]];
                if (sheet->indexMap[y * sheet->width + x] == TRANSPARENT_INDEX)
                {
                    sheet->vram[y * sheet->width + x] = COL_TRANSPARENT;
                }
                else if (sheet->indexMap[y * sheet->width + x] == FORCE_TRANSPARENT_INDEX)
                {
                    sheet->vram[y * sheet->width + x] = COL_TRANSPARENT;
                }
                else
                {
                    sheet->vram[y * sheet->width + x] = assistSheet->vram[(y - assistSheet->y) * assistSheet->width + (x - assistSheet->x)];
                }
                setBitInUpdateMap(sheet, y * sheet->width + x, true);
            }
        }
    }
}
