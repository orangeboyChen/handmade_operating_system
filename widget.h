#ifndef _WIDGET_H
#define _WIDGET_H

#include "common.h"
#include "sheet.h"
#include "utils.h"

struct Sheet *createLabel(struct Sheet *sheet, short x, short y, short width, short height, char *c, char color);
void setLabelText(struct Sheet *sheet, char *c, char color);
struct Sheet *createLabelWithBackground(struct Sheet *sheet, short x, short y, short width, short height, char *c, char color, char background);
#endif