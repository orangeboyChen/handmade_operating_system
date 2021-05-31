#include "bootpack.h"
#include <stdio.h>
#include <string.h>

struct Sheet *initRootSheet()
{
    struct BootInfo *bootInfo = getBootInfo();
    struct Sheet *rootSheet = (struct Sheet *)allocaMemory(getMemoryManager(), sizeof(struct Sheet));

    rootSheet->x = 0;
    rootSheet->y = 0;
    rootSheet->width = bootInfo->screenX;
    rootSheet->height = bootInfo->screenY;
    rootSheet->vram = bootInfo->vram;
    rootSheet->series = 999;

    rootSheet->indexMap = allocaMemory(getMemoryManager(), rootSheet->width * rootSheet->height * sizeof(int));
    rootSheet->updateMap = allocaMemory(getMemoryManager(), (rootSheet->width * rootSheet->height - 1) / sizeof(char) + 1);

    //准备背景图层
    struct Sheet *backgroundSheet = createSubsheetToTop(rootSheet, 0, 0, rootSheet->width, rootSheet->height);

    //刷新视图
    fillVram(backgroundSheet, COL8_000084);
    setFixedBottom(backgroundSheet);
    forceUpdateSheet(backgroundSheet);

    return rootSheet;
}

struct Sheet *createSubsheetToTop(struct Sheet *fatherSheet, short x, short y, short width, short height)
{
    return createSubsheetToTopWithVram(fatherSheet, x, y, width, height, allocaMemory(getMemoryManager(), width * height));
}

struct Sheet *createSubsheetToTopWithVram(struct Sheet *fatherSheet, short x, short y, short width, short height, char *vram)
{
    struct Sheet *newSheet = allocaMemory(getMemoryManager(), sizeof(struct Sheet));
    newSheet->x = x;
    newSheet->y = y;
    newSheet->width = width;
    newSheet->height = height;
    newSheet->vram = vram;
    newSheet->indexMap = allocaMemory(getMemoryManager(), width * height * sizeof(int));

    //按位存储节省空间
    newSheet->updateMap = allocaMemory(getMemoryManager(), (width * height - 1) / sizeof(char) + 1);

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
    for (i = y; i < y + height; i++)
    {
        for (j = x; j < x + width; j++)
        {
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
        while (currentSheet->nextSheet != NULL && getFixedTop(currentSheet) == true)
        {
            currentSheet = currentSheet->nextSheet;
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

void updateSheetIndexMap(struct Sheet *sheet)
{
    if (sheet->topSheet == NULL)
    {
        return;
    }

    // char *temp = allocaMemory(getMemoryManager(), sheet->height * sheet->width * sizeof(char));

    memset(sheet->indexMap, 0, sizeof(int) * sheet->height * sheet->width);
    struct Sheet *currentSheet = sheet->topSheet;
    while (currentSheet != NULL)
    {
        int x, y;
        for (y = currentSheet->y; y < currentSheet->y + currentSheet->height; y++)
        {
            for (x = currentSheet->x; x < currentSheet->x + currentSheet->width; x++)
            {
                if (sheet->indexMap[y * sheet->width + x] == 0 || sheet->indexMap[y * sheet->width + x] == -1)
                {
                    unsigned int currentSheetIndex = (y - currentSheet->y) * currentSheet->width + (x - currentSheet->x);
                    if (currentSheet->vram[currentSheetIndex] == COL_TRANSPARENT || currentSheet->indexMap[currentSheetIndex] == TRANSPARENT_INDEX)
                    {
                        sheet->indexMap[y * sheet->width + x] = TRANSPARENT_INDEX;
                    }
                    else
                    {
                        sheet->indexMap[y * sheet->width + x] = currentSheet->index;
                    }
                }
                // else if (sheet->indexMap[y * sheet->width + x] == -1)
                // {
                //     struct Sheet *assistSheet = sheet->sheetStore[sheet->indexMap[y * sheet->width + x]];
                //     setBitInUpdateMap(assistSheet, (y - assistSheet->y) * assistSheet->width + (x - assistSheet->x), true);
                // }

                // sheet->indexMap[y * sheet->width + x] = currentSheet->index;
            }
        }
        currentSheet = currentSheet->nextSheet;
    }

    // struct Sheet *currentSheet = sheet->bottomSheet;
    // while (currentSheet != NULL)
    // {
    //     int x, y;
    //     for (y = currentSheet->y; y < currentSheet->y + currentSheet->height; y++)
    //     {
    //         for (x = currentSheet->x; x < currentSheet->x + currentSheet->width; x++)
    //         {
    //             if (currentSheet->vram[(y - currentSheet->y) * currentSheet->width + (x - currentSheet->x)] != COL_TRANSPARENT)
    //             {
    //                 sheet->indexMap[y * sheet->width + x] = currentSheet->index;
    //             }
    //             else
    //             {
    //                 struct Sheet *assistSheet = sheet->sheetStore[sheet->indexMap[y * sheet->width + x]];
    //                 setBitInUpdateMap(assistSheet, (y - assistSheet->y) * assistSheet->width + (x - assistSheet->x), true);
    //             }
    //             // sheet->indexMap[y * sheet->width + x] = currentSheet->index;
    //         }
    //     }
    //     currentSheet = currentSheet->previousSheet;
    // }
}

void forceUpdateSheet(struct Sheet *sheet)
{
    updateSheetIndexMap(sheet->fatherSheet);
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

            if (sheet->vram[sonIndex] == COL_TRANSPARENT && sheet->nextSheet == NULL)
            {
                fatherSheet->vram[fatherIndex] = COL_TRANSPARENT;
            }
            // fatherSheet->vram[fatherIndex] = sheet->vram[sonIndex];

            setBitInUpdateMap(fatherSheet, fatherIndex, true);
        }
    }
}

void updateSheet(struct Sheet *sheet)
{

    struct Sheet *fatherSheet = sheet->fatherSheet;
    if (fatherSheet == NULL)
    {
        return;
    }

    int x, y;
    char isUpdate = false;
    for (y = sheet->y; y < sheet->height + sheet->y; y++)
    {
        for (x = sheet->x; x < sheet->width + sheet->x; x++)
        {
            unsigned int fatherIndex = y * fatherSheet->width + x;
            unsigned int sonIndex = (y - sheet->y) * sheet->width + (x - sheet->x);
            if ((fatherSheet->indexMap[fatherIndex] == sheet->index || sheet->vram[sonIndex] == COL_TRANSPARENT) && getBitInUpdateMap(sheet, sonIndex) == true)
            {
                isUpdate = true;
                setBitInUpdateMap(fatherSheet, fatherIndex, true);

                // char color = sheet->vram[sonIndex];
                // if (color != COL_TRANSPARENT)
                // {
                //     fatherSheet->vram[fatherIndex] = color;
                // }
                // else
                // {
                //     struct Sheet *currentSheet = fatherSheet->sheetStore[fatherSheet->indexMap[fatherIndex]];
                //     fatherSheet->vram[fatherIndex] = currentSheet->vram[(y - currentSheet->y) * currentSheet->width + (x - currentSheet->x)];
                // }

                // if (fatherSheet->indexMap[fatherIndex] == 0)
                // {
                //     if (sheet->vram[sonIndex] == COL_TRANSPARENT)
                //     {
                //         fatherSheet->vram[fatherIndex] = COL_TRANSPARENT;
                //     }
                //     else
                //     {
                //         continue;
                //     }
                // }
                // else if (fatherSheet->indexMap[fatherIndex] == TRANSPARENT_INDEX)
                // {
                //     fatherSheet->vram[fatherIndex] = COL_TRANSPARENT;
                // }
                // if ((sheet->vram[sonIndex] == COL_TRANSPARENT || sheet->indexMap[sonIndex] == TRANSPARENT_INDEX) && sheet->nextSheet == NULL)
                // {
                //     fatherSheet->vram[fatherIndex] = COL_TRANSPARENT;
                // }
                // else
                // {
                //     struct Sheet *currentSheet = fatherSheet->sheetStore[fatherSheet->indexMap[fatherIndex]];
                //     fatherSheet->vram[fatherIndex] = currentSheet->vram[(y - currentSheet->y) * currentSheet->width + (x - currentSheet->x)];
                // }
                struct Sheet *currentSheet = fatherSheet->sheetStore[fatherSheet->indexMap[fatherIndex]];
                fatherSheet->vram[fatherIndex] = currentSheet->vram[(y - currentSheet->y) * currentSheet->width + (x - currentSheet->x)];

                setBitInUpdateMap(sheet, sonIndex, false);
            }
        }
    }

    if (fatherSheet->fatherSheet != NULL && isUpdate == true)
    {
        updateSheet(fatherSheet);
    }
    else
    {
        memset(sheet->updateMap, 0, (sheet->width * sheet->height - 1) / sizeof(char) + 1);
    }
}

char getFixedTop(struct Sheet *sheet)
{
    return sheet->status & (!(1 << 7)) != 0;
}
char getFixedBottom(struct Sheet *sheet)
{
    return sheet->status & (!(1 << 6)) != 0;
}

void setFixedTop(struct Sheet *sheet)
{
    struct Sheet *fatherSheet = sheet->fatherSheet;

    struct Sheet *currentSheet = fatherSheet->topSheet;
    while (currentSheet->nextSheet != NULL && getFixedTop(currentSheet) == true)
    {
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

    sheet->previousSheet->nextSheet = sheet;
    sheet->nextSheet->previousSheet = sheet;
    sheet->status = sheet->status | (1 << 7);

    if (sheet->nextSheet == sheet->previousSheet)
    {
        sheet->nextSheet = NULL;
        sheet->previousSheet = NULL;
    }

    updateSheetIndexMap(fatherSheet);
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

    char s4[32];
    sprintf(s4, "123");
    putfonts8_asc(getBootInfo()->vram, getBootInfo()->screenX, 0, 0, COL8_FFFFFF, s4);

    updateSheetIndexMap(fatherSheet);
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
    for (x = 0; x < sheet->height; x++)
    {
        for (y = 0; y < sheet->width; y++)
        {
            if (sheet->indexMap[y * sheet->width + x] != 0)
            {
                struct Sheet *assistSheet = sheet->sheetStore[sheet->indexMap[y * sheet->width + x]];
                sheet->vram[y * sheet->width + x] = assistSheet->vram[(y - assistSheet->y) * assistSheet->width + (x - assistSheet->x)];
                setBitInUpdateMap(sheet, x * sheet->width + y, true);
            }
        }
    }
}
