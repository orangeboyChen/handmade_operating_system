#ifndef _SHEET_H
#define _SHEET_H

#include "bootpack.h"
#include <stdio.h>
#include <string.h>
#include "common.h"

#define SHEET_MAX 512

#define FIX_UNSET 0
#define FIX_TOP 1
#define FIX_BOTTOM 2

#define TRANSPARENT_INDEX 127
#define FORCE_TRANSPARENT_INDEX 126
struct Sheet
{
    //自身基本属性
    short width, height;
    short x, y;
    char status; //高3位fixTop 高2位fixBottom
    char *vram;

    //对于兄弟图层
    struct Sheet *fatherSheet;
    struct Sheet *nextSheet;     //nextSheet下一层
    struct Sheet *previousSheet; //previousSheet上一层
    unsigned int index;

    //对于子图层
    struct Sheet *topSheet;
    struct Sheet *bottomSheet;
    int *indexMap;
    char *updateMap;

    //子图层内存管理
    struct Sheet *sheetStore[SHEET_MAX];

    //备注
    unsigned int series;

    //备用属性
    int attribute[128];

    // struct SheetManager *subSheetManager;
};

struct Sheet *initRootSheet();
struct Sheet *createSubsheetToTop(struct Sheet *fatherSheet, short x, short y, short width, short height);
struct Sheet *createSubsheetToTopWithVram(struct Sheet *fatherSheet, short x, short y, short width, short height, char *vram);
void updateSheet(struct Sheet *sheet);
void updateSheetWithTransparent(struct Sheet *sheet, int isTransparent);
void fillVram(struct Sheet *sheet, unsigned char c);
void setBitInUpdateMap(struct Sheet *sheet, unsigned int index, unsigned int value);
void forceUpdateSheet(struct Sheet *sheet);
char getFixedTop(struct Sheet *sheet);
char getFixedBottom(struct Sheet *sheet);
void setFixedTop(struct Sheet *sheet);
void setFixedBottom(struct Sheet *sheet);
void printInSheet(struct Sheet *sheet, short x, short y, char *c, unsigned int colorConstant);
void fillInSheet(struct Sheet *sheet, short x, short y, short width, short height, unsigned int colorConstant);
void updateAllSubsheetWithFather(struct Sheet *sheet, struct Sheet *fatherSheet);
void updateAllSubsheet(struct Sheet *sheet);
void updateSingleSheetIndexAndVramInFatherSheet(struct Sheet *sheet);
void updateIndexMap(struct Sheet *sheet);
void fillVramByIndexMap(struct Sheet *sheet);
void moveSheet(struct Sheet *sheet, short x1, short y1);
void updatePartOfIndexMap(struct Sheet *sheet, short fromX, short fromY, short toX, short toY);
void fillPartOfVramByIndexMap(struct Sheet *sheet, short fromX, short fromY, short toX, short toY);
#endif