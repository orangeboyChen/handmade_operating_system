#ifndef _GRAPHIC_H
#define _GRAPHIC_H
#include "common.h"
#include "io.h"
#include "graphic.h"
#include "sheet.h"
#include <stdio.h>
void initMouseCursor(struct Sheet *mouseSheet, short x, short y);
void initFourRadius(struct Sheet *backgroundSheet);
void initButtonCircle(struct Sheet *sheet, short x0, short y0, unsigned int color);
void initButtonFourRadius(struct Sheet *backgroundSheet, unsigned int color);
#endif