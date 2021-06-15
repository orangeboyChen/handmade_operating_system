#include "widget.h"

struct Sheet *createLabel(struct Sheet *sheet, short x, short y, short width, short height, char *c, char color)
{
    struct Sheet *labelSheet = createSubsheetToTop(sheet, x, y, width, height);
    fillVram(labelSheet, COL_TRANSPARENT);
    labelSheet->attribute[0] = COL_TRANSPARENT;
    putfonts8_asc(labelSheet->vram, width, 0, 0, color, c);
    updateSheet(labelSheet);
    return labelSheet;
}

struct Sheet *createLabelWithBackground(struct Sheet *sheet, short x, short y, short width, short height, char *c, char color, char background)
{
    struct Sheet *labelSheet = createSubsheetToTop(sheet, x, y, width, height);
    fillVram(labelSheet, background);
    labelSheet->attribute[0] = background;

    putfonts8_asc(labelSheet->vram, width, 0, 0, color, c);
    updateSheet(labelSheet);
    return labelSheet;
}

void setLabelText(struct Sheet *sheet, char *c, char color)
{

    fillVram(sheet, sheet->attribute[0]);

    putfonts8_asc(sheet->vram, sheet->width, 0, 0, color, c);
    updateSheet(sheet);
}