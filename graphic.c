#include "graphic.h"
#define RADIUS_LENGTH 16
#define CIRCLE_RADIUS 8

void initButtonFourRadius(struct Sheet *backgroundSheet, unsigned int inputColor)
{
    static char radius[6][6] =
        {
            "....**",
            "..**OO",
            ".**OOO",
            "**OOOO",
            "*OOOOO",
            "*OOOOO"};

    int x, y;
    for (y = 0; y < 6; y++)
    {
        for (x = 0; x < 6; x++)
        {
            unsigned int color = 0;
            if (radius[y][x] == '*')
            {
                color = inputColor;
            }
            if (radius[y][x] == 'O')
            {
                continue;
            }
            if (radius[y][x] == '/')
            {
                color = COL8_848484;
            }
            if (radius[y][x] == '.')
            {
                color = COL_FORCE_TRANSPARENT;
                backgroundSheet->vram[y * backgroundSheet->width + x] = COL_FORCE_TRANSPARENT;
            }
            backgroundSheet->vram[y * backgroundSheet->width + x] = color;
            backgroundSheet->vram[y * backgroundSheet->width + (backgroundSheet->width - 1 - x)] = color;
            backgroundSheet->vram[(backgroundSheet->height - 1 - y) * backgroundSheet->width + x] = color;
            backgroundSheet->vram[(backgroundSheet->height - 1 - y) * backgroundSheet->width + (backgroundSheet->width - 1 - x)] = color;
            setBitInUpdateMap(backgroundSheet, y * backgroundSheet->width + x, true);
        }
    }
}

void initFourRadius(struct Sheet *backgroundSheet)
{

    static char radius[RADIUS_LENGTH][RADIUS_LENGTH] =
        {
            ".........*******",
            "......***OOOOOOO",
            ".....**OOOOOOOOO",
            "....**OOOOOOOOOO",
            "...**OOOOOOOOOOO",
            "..**OOOOOOOOOOOO",
            ".**OOOOOOOOOOOOO",
            ".*OOOOOOOOOOOOOO",
            ".*OOOOOOOOOOOOOO",
            "*OOOOOOOOOOOOOOO",
            "*OOOOOOOOOOOOOOO",
            "*OOOOOOOOOOOOOOO",
            "*OOOOOOOOOOOOOOO",
            "*OOOOOOOOOOOOOOO",
            "*OOOOOOOOOOOOOOO",
            "*OOOOOOOOOOOOOOO",
        };

    int x, y;
    for (y = 0; y < RADIUS_LENGTH; y++)
    {
        for (x = 0; x < RADIUS_LENGTH; x++)
        {
            unsigned int color = 0;
            if (radius[y][x] == '*')
            {
                color = COL8_848484;
            }
            if (radius[y][x] == 'O')
            {
                continue;
            }
            if (radius[y][x] == '/')
            {
                color = COL8_848484;
            }
            if (radius[y][x] == '.')
            {
                color = COL_FORCE_TRANSPARENT;
                backgroundSheet->vram[y * backgroundSheet->width + x] = COL_FORCE_TRANSPARENT;
            }
            backgroundSheet->vram[y * backgroundSheet->width + x] = color;
            backgroundSheet->vram[y * backgroundSheet->width + (backgroundSheet->width - 1 - x)] = color;
            backgroundSheet->vram[(backgroundSheet->height - 1 - y) * backgroundSheet->width + x] = color;
            backgroundSheet->vram[(backgroundSheet->height - 1 - y) * backgroundSheet->width + (backgroundSheet->width - 1 - x)] = color;
            setBitInUpdateMap(backgroundSheet, y * backgroundSheet->width + x, true);
        }
    }

    // static char radius2[RADIUS_LENGTH][RADIUS_LENGTH] =
    // {""}
}

void initButtonCircle(struct Sheet *sheet, short x0, short y0, unsigned int color)
{

    static char circle[CIRCLE_RADIUS][CIRCLE_RADIUS] = {
        "..****..",
        ".******.",
        "********",
        "********",
        "********",
        "********",
        ".******.",
        "..****.."};

    int x, y;
    for (y = 0; y < CIRCLE_RADIUS; y++)
    {
        for (x = 0; x < CIRCLE_RADIUS; x++)
        {

            if (circle[y][x] == '*')
            {
                sheet->vram[(y + y0) * sheet->width + x + x0] = color;
            }
            if (circle[y][x] == '.')
            {
                sheet->vram[(y + y0) * sheet->width + x + x0] = COL_TRANSPARENT;
            }
            setBitInUpdateMap(sheet, (y + y0) * sheet->width + x + x0, true);
        }
    }
}

void initMouseCursor(struct Sheet *mouseSheet, short x0, short y0)
{
    // static char cursor[MOUSE_HEIGHT][MOUSE_WIDTH] = {
    //     "**..............",
    //     "*O***...........",
    //     ".*OOO***........",
    //     ".*OOOOOO***.....",
    //     ".*OOOOOOO*......",
    //     "..*OOOOO*.......",
    //     "..*OOOOO*.......",
    //     "..*OO*OOO*......",
    //     "...*O**OOO*.....",
    //     "...**..*OOO*....",
    //     "...*....*OOO*...",
    //     ".........*OOO*..",
    //     "..........*O*...",
    //     "...........*....",
    //     "................",
    //     "................"};
    static char cursor[MOUSE_HEIGHT][MOUSE_WIDTH] = {
        "**......",
        "*O*.....",
        "*OO*....",
        "*OOO*...",
        "*OOOO*..",
        "*OOOOO*.",
        "*OOOOOO*",
        "*OOOOO*.",
        "*OOOO*..",
        "*OOOO*..",
        "*OOOOO*.",
        "*O**OO*.",
        "**.*OOO*",
        "*...*OO*",
        "....*O*.",
        ".....*.."};

    int drawHeight = MOUSE_HEIGHT;
    int drawWidth = MOUSE_WIDTH;

    // if (x0 + MOUSE_WIDTH >= mouseSheet->width)
    // {
    //     return;
    //     drawWidth = mouseSheet->width - x0 - 1;
    // }

    // if (y0 + MOUSE_HEIGHT >= mouseSheet->height)
    // {
    //     return;
    //     drawHeight = mouseSheet->height - y0 - 1;
    // }

    int x, y;
    for (y = 0; y < MOUSE_HEIGHT; y++)
    {
        for (x = 0; x < MOUSE_WIDTH; x++)
        {
            // if (x + x0 >= mouseSheet->width || y + y0 >= mouseSheet->height)
            // {
            //     continue;
            // }

            if (cursor[y][x] == '*')
            {
                mouseSheet->vram[(y + y0) * mouseSheet->width + x + x0] = COL8_000000;
            }
            if (cursor[y][x] == 'O')
            {
                mouseSheet->vram[(y + y0) * mouseSheet->width + x + x0] = COL8_FFFFFF;
            }
            if (cursor[y][x] == '/')
            {
                mouseSheet->vram[(y + y0) * mouseSheet->width + x + x0] = COL8_848484;
            }
            if (cursor[y][x] == '.')
            {
                mouseSheet->vram[(y + y0) * mouseSheet->width + x + x0] = COL_TRANSPARENT;
            }
            setBitInUpdateMap(mouseSheet, (y + y0) * mouseSheet->width + x + x0, true);
        }
    }
}

void initTriangle1(struct Sheet *sheet, unsigned int inputColor)
{
    static char triangle[16][8] =
        {
            "........",
            "........",
            "........",
            "........",
            "...**...",
            "...**...",
            "..****..",
            "..****..",
            ".******.",
            ".******.",
            "********",
            "********",
            "........",
            "........",
            "........",
            "........"};

    int x, y;
    for (y = 0; y < 16; y++)
    {
        for (x = 0; x < 16; x++)
        {
            unsigned int color = 0;
            if (triangle[y][x] == '*')
            {
                color = inputColor;
            }
            if (triangle[y][x] == 'O')
            {
                continue;
            }
            if (triangle[y][x] == '/')
            {
                color = COL8_848484;
            }
            if (triangle[y][x] == '.')
            {
                color = COL_TRANSPARENT;
                sheet->vram[y * sheet->width + x] = COL_TRANSPARENT;
            }
            sheet->vram[y * sheet->width + x] = color;

            setBitInUpdateMap(sheet, y * sheet->width + x, true);
        }
    }
}

void initTriangle2(struct Sheet *sheet, unsigned int inputColor)
{
    static char triangle[16][8] =
        {
            "........",
            "........",
            "........",
            "........",
            "********",
            "********",
            ".******.",
            ".******.",
            "..****..",
            "..****..",
            "...**...",
            "...**...",
            "........",
            "........",
            "........",
            "........"};

    int x, y;
    for (y = 0; y < 16; y++)
    {
        for (x = 0; x < 16; x++)
        {
            unsigned int color = 0;
            if (triangle[y][x] == '*')
            {
                color = inputColor;
            }
            if (triangle[y][x] == 'O')
            {
                continue;
            }
            if (triangle[y][x] == '/')
            {
                color = COL8_848484;
            }
            if (triangle[y][x] == '.')
            {
                color = COL_TRANSPARENT;
                sheet->vram[y * sheet->width + x] = COL_TRANSPARENT;
            }
            sheet->vram[y * sheet->width + x] = color;

            setBitInUpdateMap(sheet, y * sheet->width + x, true);
        }
    }
}