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
    rootSheet->index = 999;

    rootSheet->indexMap = allocaMemory(getMemoryManager(), rootSheet->width * rootSheet->height * sizeof(char));
    rootSheet->updateMap = allocaMemory(getMemoryManager(), (rootSheet->width * rootSheet->height - 1) / sizeof(char) + 1);

    //准备背景图层
    struct Sheet *backgroundSheet = createSubsheetToTop(rootSheet, 0, 0, rootSheet->width, rootSheet->height);

    //刷新视图
    fillVram(backgroundSheet, COL8_000084);

    refreshSheetIndexMap(rootSheet);
    refreshSheet(backgroundSheet);

    return rootSheet;
}

struct Sheet *createSubsheetToTop(struct Sheet *fatherSheet, short x, short y, short width, short height)
{
    // char s3[32];
    // sprintf(s3, "%u %u", fatherSheet, sizeof(struct Sheet));
    // putfonts8_asc(getBootInfo()->vram, getBootInfo()->screenX, 0, 0, COL8_FFFFFF, s3);
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
    newSheet->indexMap = allocaMemory(getMemoryManager(), width * height * sizeof(char));

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
            fatherSheet->indexMap[(y + i) * fatherSheet->width + x + j] = newSheet->index;
            setBitInUpdateMap(fatherSheet, (y + i) * fatherSheet->width + x + j, true);
        }
    }

    if (fatherSheet->topSheet == NULL || fatherSheet->bottomSheet == NULL)
    {
        fatherSheet->topSheet = fatherSheet->bottomSheet = newSheet;
    }
    else
    {
        newSheet->nextSheet = fatherSheet->topSheet;
        fatherSheet->previousSheet = newSheet;
        fatherSheet->topSheet = newSheet;
    }

    return newSheet;
}

// void initRootSheetManager(struct SheetManager **sheetManager)
// {
//     struct BootInfo *bootInfo = getBootInfo();

//     // char s3[32];
//     // sprintf(s3, "333333");
//     // putfonts8_asc(getBootInfo()->vram, getBootInfo()->screenX, 0, 0, COL8_FFFFFF, s3);

//     *sheetManager = allocaMemory(getMemoryManager(), sizeof(struct SheetManager));

//     int i;
//     for (i = 0; i < SHEET_MAX; i++)
//     {
//         (*sheetManager)->sheets[i].isUsing = 0;
//         (*sheetManager)->sheets[i].index = i;
//     }

//     struct Sheet *backgroundSheet = createSheetToTop(sheetManager, 0, 0, bootInfo->screenX, bootInfo->screenY);

//     char s[32];
//     sprintf(s, "x=%d y=%d", backgroundSheet->width, backgroundSheet->isUsing);
//     putfonts8_asc(bootInfo->vram, bootInfo->screenX, 0, 0, COL8_FFFFFF, s);

//     fillVram(backgroundSheet, COL8_000084);
//     refreshSheet(backgroundSheet);
// }

// void createSubSheetManager(struct Sheet **fatherSheet)
// {
//     struct SheetManager *sheetManager = allocaMemory(getMemoryManager(), sizeof(struct SheetManager));
//     (*fatherSheet)->subSheetManager = sheetManager;

//     int i;
//     for (i = 0; i < SHEET_MAX; i++)
//     {
//         sheetManager->sheets[i].isUsing = 0;
//         sheetManager->sheets[i].index = i;
//     }
// }

// struct Sheet *createSheetToTop(struct SheetManager **sheetManager, short x, short y, short width, short height)
// {
//     struct Sheet *newSheet = getUnusedSheetFromSheetManager(*sheetManager);
//     newSheet->x = x;
//     newSheet->y = y;
//     newSheet->width = width;
//     newSheet->height = height;
//     newSheet->vram = allocaMemory(getMemoryManager(), sizeof(char) * width * height);

//     char s3[32];
//     sprintf(s3, "%d %u", newSheet->width, &newSheet);
//     putfonts8_asc(getBootInfo()->vram, getBootInfo()->screenX, 0, 16, COL8_FFFFFF, s3);

//     if ((*sheetManager)->topSheet == NULL || (*sheetManager)->bottomSheet == NULL)
//     {
//         (*sheetManager)->topSheet = newSheet;
//         (*sheetManager)->bottomSheet = newSheet;
//     }
//     else
//     {
//         newSheet->nextSheet = (*sheetManager)->topSheet;
//         (*sheetManager)->topSheet->previousSheet = newSheet;
//         (*sheetManager)->topSheet = newSheet;
//     }

//     return newSheet;
// }

// struct Sheet *getUnusedSheetFromSheetManager(struct SheetManager *sheetManager)
// {
//     int i;
//     for (i = 0; i < SHEET_MAX; i++)
//     {
//         if (sheetManager->sheets[i].isUsing == 0)
//         {
//             sheetManager->sheets[i].isUsing == 1;
//             sheetManager->sheets[i].width = 32;
//             return &sheetManager->sheets[i];
//         }
//     }
//     return NULL;
// }

// void releaseSheet(struct SheetManager *sheetManager, struct Sheet *sheet)
// {
//     releaseBlock(getMemoryManager(), sheet);
// }

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

// void refreshSheet(struct Sheet *sheet)
// {
//     struct BootInfo *bootInfo = getBootInfo();
//     int x, y;
//     for (y = sheet->y; y < sheet->y + sheet->height; y++)
//     {
//         for (x = sheet->x; x < sheet->x + sheet->width; x++)
//         {
//             bootInfo->vram[(y + sheet->y) * bootInfo->screenX + sheet->x + x] = sheet->vram[y * sheet->width + sheet->x];
//         }
//     }
//     return;
// }

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

void refreshSheetIndexMap(struct Sheet *sheet)
{
    if (sheet->topSheet == NULL)
    {
        return;
    }

    // char *temp = allocaMemory(getMemoryManager(), sheet->height * sheet->width * sizeof(char));

    struct Sheet *currentSheet = sheet->topSheet;
    while (currentSheet != NULL)
    {
        int x, y;
        for (y = currentSheet->y; y < currentSheet->y + currentSheet->height; y++)
        {
            for (x = currentSheet->x; x < currentSheet->x + currentSheet->width; x++)
            {

                sheet->indexMap[(y + currentSheet->y) * sheet->width + x + currentSheet->x] = currentSheet->index;
            }
        }
        currentSheet = currentSheet->nextSheet;
    }
}

void refreshSheet(struct Sheet *sheet)
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
            if (fatherSheet->indexMap[fatherIndex] == sheet->index && getBitInUpdateMap(sheet, sonIndex) == true)
            {
                isUpdate = true;
                setBitInUpdateMap(fatherSheet, fatherSheet, true);
                fatherSheet->vram[fatherIndex] = sheet->vram[sonIndex];

                setBitInUpdateMap(sheet, sonIndex, false);
            }
        }
    }

    if (fatherSheet->fatherSheet != NULL && isUpdate == true)
    {
        refreshSheet(fatherSheet);
    }
    else
    {

        memset(sheet, 0, (sheet->width * sheet->height - 1) / sizeof(char) + 1);
    }
}
