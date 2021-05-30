#include "bootpack.h"
#include <stdio.h>

void initRootSheetManager(struct SheetManager *sheetManager)
{
    struct BootInfo *bootInfo = getBootInfo();

    // char *s3;
    // sprintf(s3, "%u", allocaMemory(getMemoryManager(), sizeof(struct Sheet)));
    // putfonts8_asc(getBootInfo()->vram, getBootInfo()->screenX, 0, 0, COL8_FFFFFF, s3);
    // return;

    struct Sheet *backgroundSheet = allocaMemory(getMemoryManager(), sizeof(struct Sheet));

    backgroundSheet->isUsing = 1;
    backgroundSheet->x = 0;
    backgroundSheet->y = 0;
    backgroundSheet->width = bootInfo->screenX;
    backgroundSheet->height = bootInfo->screenY;
    backgroundSheet->index = 0;
    backgroundSheet->vram = allocaMemory(getMemoryManager(), bootInfo->screenX * bootInfo->screenY);

    int i;
    for (i = 1; i < SHEET_MAX; i++)
    {
        sheetManager->sheetStore[i] = NULL;
    }

    sheetManager->topSheet = backgroundSheet;
    sheetManager->bottomSheet = backgroundSheet;
    sheetManager->fatherSheet = NULL;

    fillVram(backgroundSheet, COL8_FFFFFF);

    refreshSheet(backgroundSheet);

    // sheetManager->sheetStore[0] = backgroundSheet;
}

void initSheetManager(struct Sheet *fatherSheet, struct SheetManager *sheetManager, short x, short y, unsigned char c)
{
    sheetManager->fatherSheet = fatherSheet;

    struct Sheet *backgroundSheet = allocaMemory(getMemoryManager(), sizeof(struct Sheet));
    backgroundSheet->isUsing = 1;
    backgroundSheet->x = fatherSheet->x;
    backgroundSheet->y = fatherSheet->y;
    backgroundSheet->width = x;
    backgroundSheet->height = y;
    backgroundSheet->index = 0;

    sheetManager->sheetStore[0] = backgroundSheet;

    int i;
    for (i = 1; i < 512; i++)
    {
        sheetManager->sheetStore[i] = NULL;
    }

    sheetManager->topSheet = backgroundSheet;
    sheetManager->bottomSheet = backgroundSheet;

    fillVram(&backgroundSheet, c);
}

struct Sheet *insertSheetToTop(struct SheetManager *sheetManager, short x1, short y1, short width, short height)
{
    struct Sheet *newSheet = createSheet(sheetManager);
    newSheet->x = x1;
    newSheet->y = y1;
    newSheet->width = width;
    newSheet->height = height;
    newSheet->isUsing = 1;

    newSheet->nextSheet = sheetManager->topSheet;
    sheetManager->topSheet->previousSheet = newSheet;
    sheetManager->topSheet = newSheet;

    struct SheetManager *subSheetManager;
    initSheetManager(newSheet, subSheetManager, width, height, COL8_008400);

    return newSheet;
}

struct Sheet *createSheet(struct SheetManager *sheetManager)
{
    struct Sheet *newSheet = createSheet(sheetManager);
    int i;
    for (i = 0; i < 512; i++)
    {
        if (sheetManager->sheetStore[i] != NULL)
        {
            sheetManager->sheetStore[i] = newSheet;
            newSheet->index = i;
            return newSheet;
        }
    }
    return NULL;
}

void releaseSheet(struct SheetManager *sheetManager, struct Sheet *sheet)
{
    sheetManager->sheetStore[sheet->index] = NULL;
    releaseBlock(getMemoryManager(), sheet);
}

void refreshSheet(struct Sheet *sheet)
{
    char *s3;
    sprintf(s3, "asde");
    putfonts8_asc(getBootInfo()->vram, getBootInfo()->screenX, 0, 0, COL8_FFFFFF, s3);
    return;

    struct BootInfo *bootInfo = getBootInfo();
    int x, y;
    for (y = sheet->y; y <= sheet->y + sheet->height; y++)
    {
        for (x = sheet->x; x <= sheet->x + sheet->width; x++)
        {
            // bootInfo->vram[(y + sheet->y) * bootInfo->screenY + sheet->x + x] = sheet->vram[y * sheet->height + sheet->x];

            // char *s;
            // sprintf(s, "sheet->vram[y * sheet->height + sheet->x]");
            // putfonts8_asc(bootInfo->vram, bootInfo->screenX, 0, 0, COL8_FFFFFF, s);
        }
    }
    // for (y = 0; y <= 200; y++)
    // {
    //     for (x = 0; x <= 200; x++)
    //     {
    //         bootInfo->vram[y * bootInfo->screenY + x] = COL8_0000FF;
    //     }
    // }
    // char *s;
    // sprintf(s, "sheet->vram[y * sheet->height + sheet->x]");
    // putfonts8_asc(bootInfo->vram, bootInfo->screenX, 0, 0, COL8_FFFFFF, s);

    return;
}

void fillVram(struct Sheet *sheet, unsigned char c)
{
    struct BootInfo *bootInfo = getBootInfo();
    int i, j;
    for (i = 0; i < sheet->height; i++)
    {
        for (j = 0; j < sheet->width; j++)
        {
            sheet->vram[i * sheet->height + j] = c;
        }
    }
}
