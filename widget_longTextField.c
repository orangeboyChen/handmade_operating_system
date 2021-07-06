#include "widget_longTextField.h"

struct LongTextField *createLongTextField(struct Sheet *sheet, short x, short y, short width, short height, short maxHeight, char *content)
{
    return createLongTextFieldWithColor(sheet, x, y, width, height, maxHeight, content, COL8_FFFFFF, COL8_000000);
}

struct LongTextField *createLongTextFieldWithColor(struct Sheet *sheet, short x, short y, short width, short height, short maxHeight, char *content, char backgroundColor, char fontColor)
{
    struct LongTextField *longTextField = allocaMemory(getMemoryManager(), sizeof(struct LongTextField));
    longTextField->backgroundColor = backgroundColor;
    longTextField->fontColor = fontColor;

    struct Sheet *longTextFieldSheet = createSubsheetToTop(sheet, x, y, width, height);
    longTextField->sheet = longTextFieldSheet;
    longTextFieldSheet->attribute[6] = longTextField;
    longTextFieldSheet->attribute[5] = LONGTEXTFIELD;

    //背景
    struct Sheet *backgroundSheet = createSubsheetToTop(longTextFieldSheet, 0, 0, width, height);
    setFixedBottom(backgroundSheet);
    fillInSheet(backgroundSheet, 0, 0, width, height, COL_TRANSPARENT);
    fillInSheet(backgroundSheet, 0, 0, width - 25, height, backgroundColor);
    fillInSheet(backgroundSheet, 0, 0, width - 25, 1, COL8_000000);
    fillInSheet(backgroundSheet, 0, 0, 1, height, COL8_000000);
    fillInSheet(backgroundSheet, 0, height - 1, width - 25, 1, COL8_000000);
    fillInSheet(backgroundSheet, width - 26, 0, 1, height, COL8_000000);
    updateSheet(backgroundSheet);
    longTextField->backgroundSheet = backgroundSheet;

    //按钮
    char s1[2] = {0xff, '\0'};
    char s2[2] = {0xfe, '\0'};
    struct Button *upButton = createButton(longTextFieldSheet, width - 22, 0, 20, 20, s1);
    struct Button *downButton = createButton(longTextFieldSheet, width - 22, height - 20, 20, 20, s2);

    upButton->sheet->attribute[6] = longTextField;
    downButton->sheet->attribute[6] = longTextField;

    longTextField->upButton = upButton->sheet;
    longTextField->downButton = downButton->sheet;

    upButton->sheet->userActionManager->onClick = &onLongTextFieldUp;
    downButton->sheet->userActionManager->onClick = &onLongTextFieldDown;

    //显示层
    unsigned int displaySheetWidth = width - 32;
    unsigned int displaySheetHeight = height - 1;
    longTextField->contentVram = allocaMemory(getMemoryManager(), sizeof(char) * displaySheetWidth * maxHeight);
    longTextField->displayVram = allocaMemory(getMemoryManager(), sizeof(char) * displaySheetWidth * height);

    struct Sheet *displaySheet = createSubsheetToTopWithVram(longTextFieldSheet, 2, 0, displaySheetWidth, displaySheetHeight, longTextField->displayVram);
    longTextField->currentDisplayWidth = 0;
    longTextField->currentDisplayHeight = 0;
    longTextField->displayHeightPointer = 0;
    longTextField->charPerLine = displaySheet->width / 8;
    longTextField->maxHeight = maxHeight;

    longTextField->displaySheet = displaySheet;

    longTextField->content = allocaMemory(getMemoryManager(), sizeof(char) * longTextField->charPerLine * (maxHeight / 16));
    strcpy(longTextField->content, content);

    fillBox(longTextField->contentVram, displaySheet->width, COL_TRANSPARENT, 0, 0, displaySheet->width, maxHeight);
    fillInSheet(displaySheet, 0, 0, displaySheet->width, displaySheet->height, COL_TRANSPARENT);

    //处理文字信息
    unsigned int strSize = getStringSize(content);

    char *temp = longTextField->content;
    int i;
    for (i = 0; i < getStringSize(longTextField->content); i++)
    {
        putSingleChar(longTextField->contentVram, displaySheet->width, longTextField->currentDisplayWidth * 8, longTextField->currentDisplayHeight * 16, *temp, fontColor);

        longTextField->currentDisplayWidth++;
        if (longTextField->currentDisplayWidth >= longTextField->charPerLine)
        {
            longTextField->currentDisplayWidth = 0;
            longTextField->currentDisplayHeight++;
        }

        temp++;
    }

    //从内容填充到显示
    if (longTextField->displayHeightPointer > maxHeight - height)
    {
        longTextField->displayHeightPointer = maxHeight - height;
    }
    else if (longTextField->displayHeightPointer < 0)
    {
        longTextField->displayHeightPointer = 0;
    }
    int j = 0;
    for (i = longTextField->displayHeightPointer * displaySheet->width; i < (longTextField->displayHeightPointer + displaySheet->height) * displaySheet->width; i++)
    {

        longTextField->displayVram[j] = longTextField->contentVram[i];
        j++;
    }

    //处理键盘事件
    longTextFieldSheet->systemActionManager = allocaMemory(getMemoryManager(), sizeof(struct ActionManager));
    longTextFieldSheet->userActionManager = allocaMemory(getMemoryManager(), sizeof(struct ActionManager));

    longTextFieldSheet->systemActionManager->onKeyPress = &onLongTextFieldKeyDown;
    longTextFieldSheet->systemActionManager->onKeyUp = &onLongTextFieldKeyUp;
    longTextFieldSheet->systemActionManager->onClick = &onLongTextFieldClick;
    longTextFieldSheet->systemActionManager->onTextCursorTick = &onLongTextFieldCursorTick;

    longTextField->currentCursorHeight = longTextField->currentDisplayHeight;
    longTextField->currentCursorWidth = longTextField->currentDisplayWidth;

    updateIndexMapAndActionMap(longTextFieldSheet);
    fillVramByIndexMap(longTextFieldSheet);
    updateSheet(longTextFieldSheet);

    return longTextField;
}

void onLongTextFieldKeyUp(struct Sheet *this, char c, unsigned int raw)
{
    // return;
    struct LongTextField *longTextField = this->attribute[6];

    if (raw == 0x48)
    {
        longTextField->upButton->systemActionManager->onMouseLeftUp(longTextField->upButton, 0, 0);
    }
    else if (raw == 0x50)
    {
        longTextField->downButton->systemActionManager->onMouseLeftUp(longTextField->downButton, 0, 0);
    }
}

void onLongTextFieldKeyDown(struct Sheet *this, char c, unsigned int raw)
{
    struct LongTextField *longTextField = this->attribute[6];

    if (raw == 0x48)
    {
        longTextField->upButton->systemActionManager->onMouseLeftDown(longTextField->upButton, 0, 0);
    }
    else if (raw == 0x50)
    {
        longTextField->downButton->systemActionManager->onMouseLeftDown(longTextField->downButton, 0, 0);
    }
    else if (raw == 0x0e)
    {
        if ((longTextField->currentCursorHeight == longTextField->currentDisplayHeight) && (longTextField->currentCursorWidth == longTextField->currentDisplayWidth))
        {
            deleteSingleCharInLongTextField(longTextField);
        }
        else
        {
            deleteSingleCharInLongTextFieldByCursor(longTextField);
        }
    }
    else if (raw == 0x4b)
    {
        leftMoveLongTextFieldCursor(longTextField);
    }
    else if (raw == 0x4d)
    {
        rightMoveLongTextFieldCursor(longTextField);
    }
    else if (0x20 <= c && c <= 0x7e || c == '\n')
    {
        // char s[32];
        // sprintf(s, "%d %d %d %d", longTextField->currentCursorWidth, longTextField->currentCursorHeight, longTextField->currentDisplayWidth, longTextField->currentDisplayHeight);
        // setLabelText(statusLabel, s, COL8_000000);
        if ((longTextField->currentCursorHeight == longTextField->currentDisplayHeight) && (longTextField->currentCursorWidth == longTextField->currentDisplayWidth))
        {
            putSingleCharInLongTextField(longTextField, c);
        }
        else
        {
            putSingleCharInLongTextFieldByCurrentCursor(longTextField, c);
        }

        unsigned int currentCursorIndexInContentVram = longTextField->currentCursorHeight * 16 * longTextField->displaySheet->width + longTextField->currentCursorWidth * 8;

        if (currentCursorIndexInContentVram <= longTextField->displayHeightPointer * longTextField->displaySheet->width || currentCursorIndexInContentVram >= longTextField->displayHeightPointer * longTextField->displaySheet->width + (longTextField->displaySheet->height - 16) * longTextField->displaySheet->width)
        {
            moveLongTextFieldToBottom(longTextField);
        }
    }
}

void onLongTextFieldClick()
{
}

void onLongTextFieldUp(struct Sheet *this)
{

    struct LongTextField *longTextField = this->attribute[6];

    longTextField->displayHeightPointer -= 4;
    if (longTextField->displayHeightPointer < 0)
    {
        longTextField->displayHeightPointer = 0;
    }

    updateLongTextFieldDisplay(longTextField);
}

void onLongTextFieldDown(struct Sheet *this)
{
    struct LongTextField *longTextField = this->attribute[6];
    longTextField->displayHeightPointer += 4;
    updateLongTextFieldDisplay(longTextField);
}

void putSingleCharInLongTextField(struct LongTextField *longTextField, char c)
{
    updateLongTextFieldCursorChar(longTextField);
    char *pointer = longTextField->content + getStringSize(longTextField->content);
    *pointer = c;
    pointer++;
    *pointer = '\0';

    // updateLongTextFieldContent(longTextField);
    struct Sheet *displaySheet = longTextField->displaySheet;

    if (c == '\n')
    {
        longTextField->currentDisplayHeight++;
        longTextField->currentDisplayWidth = 0;
    }
    else
    {
        putSingleChar(longTextField->contentVram, displaySheet->width, longTextField->currentDisplayWidth * 8, longTextField->currentDisplayHeight * 16, c, longTextField->fontColor);
        updatePartOfLongTextFieldDisplay(longTextField, longTextField->currentDisplayWidth * 8, longTextField->currentDisplayHeight * 16);

        longTextField->currentDisplayWidth++;
        if (longTextField->currentDisplayWidth >= longTextField->charPerLine)
        {
            longTextField->currentDisplayWidth = 0;
            longTextField->currentDisplayHeight++;
        }
    }

    longTextField->currentCursorHeight = longTextField->currentDisplayHeight;
    longTextField->currentCursorWidth = longTextField->currentDisplayWidth;

    showLongTextFieldCursor(longTextField);

    // updateLongTextFieldDisplay(longTextField);
}

void updatePartOfLongTextFieldDisplay(struct LongTextField *longTextField, unsigned int contentVramX, unsigned int contentVramY)
{

    struct Sheet *displaySheet = longTextField->displaySheet;
    if (longTextField->displayHeightPointer > longTextField->maxHeight - longTextField->height)
    {
        longTextField->displayHeightPointer = longTextField->maxHeight - longTextField->height;
    }

    if (longTextField->displayHeightPointer < 0)
    {
        longTextField->displayHeightPointer = 0;
    }

    unsigned int toX = contentVramX + 8;
    unsigned int toY = contentVramY + 16;

    if (contentVramY + 16 < longTextField->displayHeightPointer || contentVramY > longTextField->displayHeightPointer + longTextField->displaySheet->height)
    {
        return;
    }

    if (contentVramX >= longTextField->displaySheet->width)
    {
        contentVramX = longTextField->displaySheet->width;
    }

    if (contentVramY < longTextField->displayHeightPointer)
    {
        contentVramY = longTextField->displayHeightPointer;
    }

    if (toX >= longTextField->displaySheet->width)
    {
        toX = longTextField->displaySheet->width;
    }

    if (toY >= longTextField->displayHeightPointer + longTextField->displaySheet->height)
    {
        toY = longTextField->displayHeightPointer + longTextField->displaySheet->height;
    }

    int i, j;
    for (i = contentVramY; i < toY; i++)
    {
        for (j = contentVramX; j < toX; j++)
        {
            longTextField->displayVram[(i - longTextField->displayHeightPointer) * displaySheet->width + j] = longTextField->contentVram[i * displaySheet->width + j];
        }
    }

    updatePartOfIndexMap(longTextField->sheet, displaySheet->x + contentVramX, displaySheet->y + contentVramY - longTextField->displayHeightPointer, displaySheet->x + toX, displaySheet->y + toY - longTextField->displayHeightPointer);
    fillPartOfVramByIndexMap(longTextField->sheet, displaySheet->x + contentVramX, displaySheet->y + contentVramY - longTextField->displayHeightPointer, displaySheet->x + toX, displaySheet->y + toY - longTextField->displayHeightPointer);
    updateSheet(longTextField->sheet);
}

void updateLongTextFieldContent(struct LongTextField *longTextField)
{
    struct Sheet *displaySheet = longTextField->displaySheet;
    fillBox(longTextField->contentVram, displaySheet->width, COL_TRANSPARENT, 0, 0, displaySheet->width, longTextField->maxHeight);

    longTextField->currentDisplayHeight = 0;
    longTextField->currentDisplayWidth = 0;
    // longTextField->displayHeightPointer = 0;

    char *temp = longTextField->content;
    int i;
    for (i = 0; i < getStringSize(longTextField->content); i++)
    {

        if (*temp == '\n')
        {
            longTextField->currentDisplayHeight++;
            longTextField->currentDisplayWidth = 0;
        }
        else
        {
            putSingleChar(longTextField->contentVram, displaySheet->width, longTextField->currentDisplayWidth * 8, longTextField->currentDisplayHeight * 16, *temp, longTextField->fontColor);

            longTextField->currentDisplayWidth++;
            if (longTextField->currentDisplayWidth >= longTextField->charPerLine)
            {
                longTextField->currentDisplayWidth = 0;
                longTextField->currentDisplayHeight++;
            }
        }

        temp++;
    }

    // updateLongTextFieldCursorChar(longTextField);

    // longTextField->currentCursorHeight = longTextField->currentDisplayHeight;
    // longTextField->currentCursorWidth = longTextField->currentDisplayWidth;

    // showLongTextFieldCursor(longTextField);
}

void moveLongTextFieldToBottom(struct LongTextField *longTextField)
{
    longTextField->displayHeightPointer = longTextField->currentDisplayHeight * 16 - longTextField->displaySheet->height + 16;
    if (longTextField->displayHeightPointer < 0)
    {
        longTextField->displayHeightPointer = 0;
    }
    // char s[32];
    // sprintf(s, "%d", longTextField->currentHeightCursor * 16 - longTextField->displaySheet->height);
    // setLabelText(statusLabel, s, COL8_000000);

    updateLongTextFieldDisplay(longTextField);
}

void updateLongTextFieldDisplay(struct LongTextField *longTextField)
{
    struct Sheet *displaySheet = longTextField->displaySheet;
    // fillBox(longTextField->contentVram, displaySheet->width, COL_TRANSPARENT, 0, 0, displaySheet->width, longTextField->maxHeight);
    // fillInSheet(displaySheet, 0, 0, displaySheet->width, displaySheet->height, COL_TRANSPARENT);

    // 处理文字信息
    // unsigned int strSize = getStringSize(longTextField->content);

    // char *temp = longTextField->content;
    // int i;
    // for (i = 0; i < getStringSize(longTextField->content); i++)
    // {
    //     putSingleChar(longTextField->contentVram, displaySheet->width, longTextField->currentWidthCursor * 8, longTextField->currentHeightCursor * 16, *temp, COL8_000000);

    //     longTextField->currentWidthCursor++;
    //     if (longTextField->currentWidthCursor >= longTextField->charPerLine)
    //     {
    //         longTextField->currentWidthCursor = 0;
    //         longTextField->currentHeightCursor++;
    //     }

    //     temp++;
    // }

    //从内容填充到显示
    if (longTextField->displayHeightPointer > longTextField->maxHeight - longTextField->height)
    {
        longTextField->displayHeightPointer = longTextField->maxHeight - longTextField->height;
    }

    if (longTextField->displayHeightPointer < 0)
    {
        longTextField->displayHeightPointer = 0;
    }

    int i, j = 0;
    for (i = longTextField->displayHeightPointer * displaySheet->width; i < (longTextField->displayHeightPointer + displaySheet->height) * displaySheet->width; i++)
    {
        longTextField->displayVram[j] = longTextField->contentVram[i];
        j++;
    }

    updateSheet(displaySheet);

    updateIndexMapAndActionMap(longTextField->sheet);
    fillVramByIndexMap(longTextField->sheet);
    updateSheet(longTextField->sheet);
}

void onLongTextFieldCursorTick(struct Sheet *longTextFieldSheet, unsigned int tick)
{
    if (tick)
    {
        showLongTextFieldCursor(longTextFieldSheet->attribute[6]);
    }
    else
    {
        updateLongTextFieldCursorChar(longTextFieldSheet->attribute[6]);
    }
}

void updateLongTextFieldCursorChar(struct LongTextField *longTextField)
{
    struct Sheet *displaySheet = longTextField->displaySheet;
    char cursor[2] = {0xdb, '\0'};
    unsigned int x = longTextField->currentCursorWidth * 8;
    unsigned int y = longTextField->currentCursorHeight * 16;
    int displayCharIndex = getCharIndexInLongTextFieldContent(longTextField, longTextField->currentCursorWidth, longTextField->currentCursorHeight);

    char displayChar = ' ';
    if (displayCharIndex != -1)
    {
        displayChar = *(longTextField->content + displayCharIndex);
    }

    putfonts8_asc(longTextField->contentVram, displaySheet->width, x, y, longTextField->backgroundColor, cursor);
    if (displayChar != '\n')
    {
        putSingleChar(longTextField->contentVram, displaySheet->width, x, y, displayChar, longTextField->fontColor);
    }
    // updateLongTextFieldDisplay(longTextField);
    updatePartOfLongTextFieldDisplay(longTextField, x, y);
}

void showLongTextFieldCursor(struct LongTextField *longTextField)
{
    // setLabelText(statusLabel, "123", COL8_000000);
    struct Sheet *displaySheet = longTextField->displaySheet;
    char cursor[2] = {0xfd, '\0'};
    unsigned int x = longTextField->currentCursorWidth * 8;
    unsigned int y = longTextField->currentCursorHeight * 16;

    putfonts8_asc(longTextField->contentVram, displaySheet->width, x, y, longTextField->fontColor, cursor);
    // updateLongTextFieldDisplay(longTextField);
    updatePartOfLongTextFieldDisplay(longTextField, x, y);
}

void leftMoveLongTextFieldCursor(struct LongTextField *longTextField)
{
    updateLongTextFieldCursorChar(longTextField);
    if (!(longTextField->currentCursorHeight <= 0 && longTextField->currentCursorWidth <= 0))
    {
        int currentCharIndex = getCharIndexInLongTextFieldContent(longTextField, longTextField->currentCursorWidth, longTextField->currentCursorHeight);
        // char s[32];
        // sprintf(s, "a %d", currentCharIndex);
        // setLabelText(statusLabel, s, COL8_FFFFFF);

        if (currentCharIndex > 0)
        {
            int result[3];
            getCursorPositionByCharIndexInLongTextField(longTextField, currentCharIndex - 1, result);

            // char s[32];
            // sprintf(s, "a %d %d %d %d %d %d", result[0], result[1], result[2], currentCharIndex, longTextField->currentCursorWidth, longTextField->currentCursorHeight);
            // setLabelText(statusLabel, s, COL8_FFFFFF);

            if (result[0] == 0)
            {
                longTextField->currentCursorWidth = result[1];
                longTextField->currentCursorHeight = result[2];

                if (longTextField->currentCursorHeight * 16 < longTextField->displayHeightPointer)
                {
                    moveLongTextFieldToCursorHeight(longTextField);
                }
                else if (longTextField->currentCursorHeight * 16 > longTextField->displayHeightPointer + longTextField->displaySheet->height - 16)
                {
                    moveLongTextFieldToCursorHeightBottom(longTextField);
                }
            }
            else
            {
            }
        }
    }

    showLongTextFieldCursor(longTextField);
}

void rightMoveLongTextFieldCursor(struct LongTextField *longTextField)
{
    updateLongTextFieldCursorChar(longTextField);
    if (!(longTextField->currentCursorHeight > longTextField->currentDisplayHeight || ((longTextField->currentCursorHeight == longTextField->currentDisplayHeight) && (longTextField->currentCursorWidth > longTextField->currentDisplayWidth))))
    // {
    // }

    // if (longTextField->currentCursorHeight <= longTextField->currentDisplayHeight && longTextField->currentCursorWidth < longTextField->currentDisplayWidth)
    {
        int currentCharIndex = getCharIndexInLongTextFieldContent(longTextField, longTextField->currentCursorWidth, longTextField->currentCursorHeight);

        // char s[32];
        // sprintf(s, "a %d %d %d", currentCharIndex, longTextField->currentCursorWidth, longTextField->currentCursorHeight);
        // setLabelText(statusLabel, s, COL8_FFFFFF);
        if (currentCharIndex >= 0)
        {
            int result[3];
            getCursorPositionByCharIndexInLongTextField(longTextField, currentCharIndex + 1, result);

            // char s[32];
            // sprintf(s, "a %d %d %d %d %d %d", result[0], result[1], result[2], currentCharIndex, longTextField->currentCursorWidth, longTextField->currentCursorHeight);
            // setLabelText(statusLabel, s, COL8_FFFFFF);

            // char s[32];
            // sprintf(s, "a %d %d %d %d", result[0], result[1], result[2], currentCharIndex);
            // setLabelText(statusLabel, s, COL8_FFFFFF);

            if (result[0] == 0)
            {
                longTextField->currentCursorWidth = result[1];
                longTextField->currentCursorHeight = result[2];
                if (longTextField->currentCursorHeight * 16 < longTextField->displayHeightPointer)
                {
                    moveLongTextFieldToCursorHeight(longTextField);
                }
                else if (longTextField->currentCursorHeight * 16 > longTextField->displayHeightPointer + longTextField->displaySheet->height - 16)
                {
                    moveLongTextFieldToCursorHeightBottom(longTextField);
                }
            }
            else
            {
            }
        }
    }

    showLongTextFieldCursor(longTextField);
}

int getCharIndexInLongTextFieldContent(struct LongTextField *longTextField, unsigned int x, unsigned int y)
{
    char *content = longTextField->content;
    unsigned int contentSize = getStringSize(content);

    int tempX = 0, tempY = 0;
    int i;

    char *contentTemp = content;
    for (i = 0; i < contentSize; i++)
    {
        if (tempX == x && tempY == y)
        {
            // char s[32];
            // sprintf(s, ">%d %d", tempX, tempY);
            // setLabelText(statusLabel, s, COL8_FFFFFF);
            return i;
        }

        if (*contentTemp == '\n')
        {
            tempX = 0;
            tempY++;
        }
        else
        {
            tempX++;
            if (tempX >= longTextField->charPerLine)
            {
                tempX = 0;
                tempY++;
            }
        }

        contentTemp++;
    }
    // char s[32];
    // sprintf(s, ">>%d %d %d %d", tempX, tempY, x, y);
    // setLabelText(statusLabel, s, COL8_FFFFFF);

    if (tempY == y && tempX == x)
    {
        return contentSize;
    }
    return -1;
}

void getCursorPositionByCharIndexInLongTextField(struct LongTextField *longTextField, unsigned int charIndex, int resultArray[])
{
    char *content = longTextField->content;
    unsigned int contentSize = getStringSize(content);

    int tempX = 0, tempY = 0;
    int i;

    char *contentTemp = content;
    for (i = 0; i <= contentSize; i++)
    {
        if (charIndex == i)
        {
            resultArray[0] = 0;
            resultArray[1] = tempX;
            resultArray[2] = tempY;
            // char s[32];
            // sprintf(s, ">%d %d", tempX, resultArray[1]);
            // setLabelText(statusLabel, s, COL8_FFFFFF);

            return;
        }

        if (*contentTemp == '\n')
        {
            tempX = 0;
            tempY++;
        }
        else
        {
            tempX++;
            if (tempX >= longTextField->charPerLine)
            {
                tempX = 0;
                tempY++;
            }
        }

        contentTemp++;
    }
    resultArray[0] = 1;
}

void putSingleCharInLongTextFieldByCurrentCursor(struct LongTextField *longTextField, char c)
{

    int currentContentIndex = getCharIndexInLongTextFieldContent(longTextField, longTextField->currentCursorWidth, longTextField->currentCursorHeight);

    char temp = c;
    char *content = longTextField->content + currentContentIndex;
    while (temp != '\0')
    {
        char lastChar = temp;
        temp = *content;
        *content = lastChar;
        *content++;
    }
    *content = '\0';
    updateLongTextFieldContent(longTextField);
    updateLongTextFieldDisplay(longTextField);

    updateLongTextFieldCursorChar(longTextField);
    int result[3];
    getCursorPositionByCharIndexInLongTextField(longTextField, currentContentIndex + 1, result);

    // char s[32];
    // sprintf(s, "%d %d %d", currentContentIndex + 1, result[1], result[2]);
    // setLabelText(statusLabel, s, COL8_FFFFFF);

    if (result[0] == 0)
    {
        longTextField->currentCursorWidth = result[1];
        longTextField->currentCursorHeight = result[2];
    }

    // char *pointer = longTextField->content + getStringSize(longTextField->content);
    // *pointer = c;
    // pointer++;
    // *pointer = '\0';

    // // updateLongTextFieldContent(longTextField);
    // struct Sheet *displaySheet = longTextField->displaySheet;

    // if (c == '\n')
    // {
    //     longTextField->currentDisplayHeight++;
    //     longTextField->currentDisplayWidth = 0;
    // }
    // else
    // {
    //     putSingleChar(longTextField->contentVram, displaySheet->width, longTextField->currentDisplayWidth * 8, longTextField->currentDisplayHeight * 16, c, longTextField->fontColor);
    //     updatePartOfLongTextFieldDisplay(longTextField, longTextField->currentDisplayWidth * 8, longTextField->currentDisplayHeight * 16);

    //     longTextField->currentDisplayWidth++;
    //     if (longTextField->currentDisplayWidth >= longTextField->charPerLine)
    //     {
    //         longTextField->currentDisplayWidth = 0;
    //         longTextField->currentDisplayHeight++;
    //     }
    // }

    // longTextField->currentCursorHeight = longTextField->currentDisplayHeight;
    // longTextField->currentCursorWidth = longTextField->currentDisplayWidth;

    showLongTextFieldCursor(longTextField);
}

void deleteSingleCharInLongTextField(struct LongTextField *longTextField)
{
    unsigned int contentStrSize = getStringSize(longTextField->content);
    if (contentStrSize == 0)
    {
        return;
    }

    char *pointer = longTextField->content + contentStrSize - 1;
    *pointer = '\0';

    int result[3];
    getCursorPositionByCharIndexInLongTextField(longTextField, contentStrSize - 1, result);
    if (result[0] == 0)
    {
        longTextField->currentCursorWidth = result[1];
        longTextField->currentCursorHeight = result[2];
    }

    updateLongTextFieldContent(longTextField);
    // updateLongTextFieldDisplay(longTextField);

    unsigned int currentCursorIndexInContentVram = longTextField->currentCursorHeight * 16 * longTextField->displaySheet->width + longTextField->currentCursorWidth * 8;
    if (currentCursorIndexInContentVram <= longTextField->displayHeightPointer * longTextField->displaySheet->width)
    {
        moveLongTextFieldToCursorHeight(longTextField);
    }
    else if (currentCursorIndexInContentVram >= longTextField->displayHeightPointer * longTextField->displaySheet->width + (longTextField->displaySheet->height - 16) * longTextField->displaySheet->width)
    {
        moveLongTextFieldToCursorHeightBottom(longTextField);
    }
    else
    {
        updateLongTextFieldDisplay(longTextField);
    }
    showLongTextFieldCursor(onLongTextFieldClick);
    // moveLongTextFieldToBottom(longTextField);
}

void deleteSingleCharInLongTextFieldByCursor(struct LongTextField *longTextField)
{

    unsigned int contentStrSize = getStringSize(longTextField->content);
    if (contentStrSize == 0)
    {
        return;
    }
    int currentContentIndex = getCharIndexInLongTextFieldContent(longTextField, longTextField->currentCursorWidth, longTextField->currentCursorHeight);

    char *content = longTextField->content + currentContentIndex - 1;
    while (*content != '\0')
    {
        *content = *(content + 1);
        *content++;
    }

    // char s[32];
    // sprintf(s, "--%s", longTextField->content);
    // setLabelText(statusLabel, s, COL8_FFFFFF);

    int result[3];
    getCursorPositionByCharIndexInLongTextField(longTextField, currentContentIndex - 1, result);
    if (result[0] == 0)
    {
        longTextField->currentCursorWidth = result[1];
        longTextField->currentCursorHeight = result[2];
    }

    updateLongTextFieldContent(longTextField);
    // updateLongTextFieldDisplay(longTextField);

    unsigned int currentCursorIndexInContentVram = longTextField->currentCursorHeight * 16 * longTextField->displaySheet->width + longTextField->currentCursorWidth * 8;
    if (currentCursorIndexInContentVram <= longTextField->displayHeightPointer * longTextField->displaySheet->width)
    {
        moveLongTextFieldToCursorHeight(longTextField);
    }
    else if (currentCursorIndexInContentVram >= longTextField->displayHeightPointer * longTextField->displaySheet->width + (longTextField->displaySheet->height - 16) * longTextField->displaySheet->width)
    {
        moveLongTextFieldToCursorHeightBottom(longTextField);
    }
    else
    {
        updateLongTextFieldDisplay(longTextField);
    }
    showLongTextFieldCursor(onLongTextFieldClick);
}

void moveLongTextFieldToCursorHeight(struct LongTextField *longTextField)
{
    longTextField->displayHeightPointer = longTextField->currentCursorHeight * 16;
    updateLongTextFieldDisplay(longTextField);
}

void moveLongTextFieldToCursorHeightBottom(struct LongTextField *longTextField)
{
    longTextField->displayHeightPointer = longTextField->currentCursorHeight * 16 - longTextField->displaySheet->height + 16;
    updateLongTextFieldDisplay(longTextField);
}