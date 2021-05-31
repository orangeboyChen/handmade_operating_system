#include "bootpack.h"
#include <stdio.h>
void initMouseCursor(struct Sheet *mouseSheet, short x0, short y0)
{
    static char cursor[16][16] = {
        "**************..",
        "*OOOOOOOOOOO*...",
        "*OOOOOOOOOO*....",
        "*OOOOOOOOO*.....",
        "*OOOOOOOO*......",
        "*OOOOOOO*.......",
        "*OOOOOOO*.......",
        "*OOOOOOOO*......",
        "*OOOO**OOO*.....",
        "*OOO*..*OOO*....",
        "*OO*....*OOO*...",
        "*O*......*OOO*..",
        "**........*OOO*.",
        "*..........*OOO*",
        "............*OO*",
        ".............***"};
    int x, y;
    for (y = 0; y < 16; y++)
    {
        for (x = 0; x < 16; x++)
        {
            if (cursor[y][x] == '*')
            {
                mouseSheet->vram[(y + y0) * mouseSheet->width + x + x0] = COL8_000000;
            }
            if (cursor[y][x] == 'O')
            {
                mouseSheet->vram[(y + y0) * mouseSheet->width + x + x0] = COL8_FFFFFF;
            }
            if (cursor[y][x] == '.')
            {
                mouseSheet->vram[(y + y0) * mouseSheet->width + x + x0] = COL_TRANSPARENT;
            }
            setBitInUpdateMap(mouseSheet, (y + y0) * mouseSheet->width + x + x0, true);
        }
    }
}