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

struct Button *createButton(struct Sheet *sheet, short x, short y, short width, short height, char *title)
{
    struct Sheet *buttonSheet = createSubsheetToTop(sheet, x, y, width, height);

    //背景
    struct Sheet *backgroundSheet = createSubsheetToTop(buttonSheet, 0, 0, width, height);
    setFixedBottom(backgroundSheet);
    fillInSheet(backgroundSheet, 0, 0, width, height, COL8_FFFFFF);
    fillInSheet(backgroundSheet, 0, 0, width, 1, COL8_000000);
    fillInSheet(backgroundSheet, 0, 0, 1, height, COL8_000000);
    fillInSheet(backgroundSheet, 0, height - 1, width, 1, COL8_000000);
    fillInSheet(backgroundSheet, width - 1, 0, 1, height, COL8_000000);
    initButtonFourRadius(backgroundSheet, COL8_000000);
    updateSheet(backgroundSheet);

    //文字
    int labelX = width / 2 - getStringSize(title) * 8 / 2;
    int labelY = height / 2 - 16 / 2;
    struct Sheet *textSheet;
    if (labelX <= 5)
    {
        labelX = 5;
        // title = "...";
        textSheet = createLabelWithBackground(buttonSheet, labelX, labelY, getStringSize("...") * 8, 16, "...", COL8_000000, COL_TRANSPARENT);
    }
    else
    {
        textSheet = createLabelWithBackground(buttonSheet, labelX, labelY, getStringSize(title) * 8, 16, title, COL8_000000, COL_TRANSPARENT);
    }

    buttonSheet->systemActionManager = allocaMemory(getMemoryManager(), sizeof(struct ActionManager));
    buttonSheet->userActionManager = allocaMemory(getMemoryManager(), sizeof(struct ActionManager));
    buttonSheet->systemActionManager->onMouseLeftDown = &onButtonSystemMouseLeftDown;
    buttonSheet->systemActionManager->onMouseLeftUp = &onButtonSystemMouseLeftUp;

    buttonSheet->attribute[0] = backgroundSheet;
    buttonSheet->attribute[1] = textSheet;

    buttonSheet->attribute[5] = BUTTON;

    struct Button *button = allocaMemory(getMemoryManager(), sizeof(struct Button));
    button->sheet = buttonSheet;
    button->title = title;
    button->titleSheet = textSheet;
    button->backgroundSheet = backgroundSheet;

    buttonSheet->attribute[2] = button;

    updateIndexMapAndActionMap(sheet);
    fillVramByIndexMap(sheet);
    updateSheet(sheet);
    return button;
}

void onButtonSystemMouseLeftDown(struct Sheet *this, unsigned int x, unsigned int y)
{
    struct Sheet *backgroundSheet = this->attribute[0];
    struct Sheet *textSheet = this->attribute[1];
    fillInSheet(backgroundSheet, 0, 0, this->width, this->height, COL8_848484);
    fillInSheet(backgroundSheet, 0, 0, this->width, 1, COL8_848484);
    fillInSheet(backgroundSheet, 0, 0, 1, this->height, COL8_848484);
    fillInSheet(backgroundSheet, 0, this->height - 1, this->width, 1, COL8_848484);
    fillInSheet(backgroundSheet, this->width - 1, 0, 1, this->height, COL8_848484);
    initButtonFourRadius(backgroundSheet, COL8_848484);
    fillVramByIndexMap(this);
    // updateSheet(textSheet);
    updateSheet(backgroundSheet);

    if (this->userActionManager != NULL && this->userActionManager->onMouseLeftDown != NULL)
    {
        this->userActionManager->onMouseLeftDown(this, x, y);
    }
    else if (this->userActionManager != NULL && this->userActionManager->onClick != NULL)
    {

        this->userActionManager->onClick(this, x, y);
    }
}

void onButtonSystemMouseLeftUp(struct Sheet *this, unsigned int x, unsigned int y)
{
    struct Sheet *backgroundSheet = this->attribute[0];
    struct Sheet *textSheet = this->attribute[1];
    fillInSheet(backgroundSheet, 0, 0, this->width, this->height, COL8_FFFFFF);
    fillInSheet(backgroundSheet, 0, 0, this->width, 1, COL8_000000);
    fillInSheet(backgroundSheet, 0, 0, 1, this->height, COL8_000000);
    fillInSheet(backgroundSheet, 0, this->height - 1, this->width, 1, COL8_000000);
    fillInSheet(backgroundSheet, this->width - 1, 0, 1, this->height, COL8_000000);
    initButtonFourRadius(backgroundSheet, COL8_000000);
    fillVramByIndexMap(this);
    updateSheet(backgroundSheet);

    if (this->userActionManager != NULL && this->userActionManager->onMouseLeftUp != NULL)
    {
        this->userActionManager->onMouseLeftUp(this, x, y);
    }
}

struct TextField *createTextField(struct Sheet *sheet, short x, short y, short width, short height, char *content)
{
    struct Sheet *textFieldSheet = createSubsheetToTop(sheet, x, y, width, height);

    //背景
    struct Sheet *backgroundSheet = createSubsheetToTop(textFieldSheet, 0, 0, width, height);
    setFixedBottom(backgroundSheet);
    fillInSheet(backgroundSheet, 0, 0, width, height, COL8_FFFFFF);
    fillInSheet(backgroundSheet, 0, 0, width, 1, COL8_000000);
    fillInSheet(backgroundSheet, 0, 0, 1, height, COL8_000000);
    fillInSheet(backgroundSheet, 0, height - 1, width, 1, COL8_000000);
    fillInSheet(backgroundSheet, width - 1, 0, 1, height, COL8_000000);
    updateSheet(backgroundSheet);

    struct TextField *textField = allocaMemory(getMemoryManager(), sizeof(struct TextField));
    textField->sheet = textFieldSheet;

    //文字
    //512字节
    textField->content = allocaMemory(getMemoryManager(), sizeof(char) * 512);
    textField->maxChar = width / 8;

    strcpy(textField->content, content);

    textField->currentSize = getStringSize(content);

    if (textField->currentSize > textField->maxChar)
    {
        textField->displayContent = textField->content + textField->currentSize * sizeof(char) - textField->maxChar * sizeof(char);
    }
    else
    {
        textField->displayContent = textField->content;
    }

    // int labelX = width / 2 - getStringSize(content) * 8 / 2;
    // char s4[32];
    // sprintf(s4, "%d", contentSize);
    // setLabelText(statusLabel, s4, COL8_FFFFFF);

    int labelY = height / 2 - 16 / 2;
    struct Sheet *textSheet = createLabelWithBackground(textFieldSheet, 3, labelY, width, 16, textField->displayContent, COL8_000000, COL_TRANSPARENT);

    textFieldSheet->systemActionManager = allocaMemory(getMemoryManager(), sizeof(struct ActionManager));

    //激活，接受监听事件
    textFieldSheet->systemActionManager->onClick = &onSystemTextfieldClick;
    textFieldSheet->systemActionManager->onKeyPress = &onSystemTextfieldKeyPress;
    textFieldSheet->systemActionManager->onKeyUp = &onSystemTextfieldKeyUp;

    textFieldSheet->attribute[0] = backgroundSheet;
    textFieldSheet->attribute[1] = textSheet;

    textField->displaySheet = textSheet;
    textField->backgroundSheet = backgroundSheet;

    textFieldSheet->attribute[2] = textField;

    textFieldSheet->attribute[5] = TEXTFIELD;

    // updateIndexMapAndActionMap(sheet);
    updatePartOfIndexMap(sheet, x, y, x + width, y + height);
    fillVramByIndexMap(sheet);
    updateSheet(sheet);
    return textField;
}

void onSystemTextfieldClick()
{
}

int i2 = 0;
void onSystemTextfieldKeyPress(struct Sheet *this, char c, unsigned int raw)
{
    i2++;

    struct TextField *textField = this->attribute[2];

    if (textField->disabled)
    {
        return;
    }
    // char *temp = textField->content + textField->currentSize * sizeof(char);
    // *temp = c;
    // temp++;
    // *temp = '\0';
    // textField->currentSize++;
    *(textField->content + textField->currentSize) = c;
    *(textField->content + textField->currentSize + 1) = '\0';
    textField->currentSize++;
    // char s4[32];
    // sprintf(s4, "->%s %d %d", textField->content, i2, textField->currentSize);
    // setLabelText(statusLabel, s4, COL8_FFFFFF);

    if (textField->currentSize > textField->maxChar)
    {
        textField->displayContent = textField->content + textField->currentSize * sizeof(char) - textField->maxChar * sizeof(char);
    }
    else
    {
        textField->displayContent = textField->content;
    }
    setLabelText(textField->displaySheet, textField->displayContent, COL8_000000);
}

void onSystemTextfieldKeyUp(struct Sheet *this, char c, unsigned int raw)
{
}

void setTextFieldText(struct TextField *textField, char *c)
{
    strcpy(textField->content, c);

    textField->currentSize = getStringSize(c);
    if (textField->currentSize > textField->maxChar)
    {
        textField->displayContent = textField->content + textField->currentSize * sizeof(char) - textField->maxChar * sizeof(char);
    }
    else
    {
        textField->displayContent = textField->content;
    }

    setLabelText(textField->displaySheet, textField->displayContent, COL8_000000);
}

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

    // fillInSheet(upButton->titleSheet, 0, 0, upButton->titleSheet->width, upButton->titleSheet->height, COL_TRANSPARENT);
    // initTriangle1(upButton->titleSheet, COL8_000000);
    // updateIndexMapAndActionMap(upButton->titleSheet);
    // fillVramByIndexMap(upButton->titleSheet);
    // updateSheet(upButton->titleSheet);

    // initTriangle2(downButton->titleSheet, COL8_000000);
    // updateSheet(downButton->titleSheet);

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
        // if (i == 100)
        // {
        //     break;
        // }
        longTextField->displayVram[j] = longTextField->contentVram[i];
        j++;
    }

    // char a = '2';
    // putSingleChar(longTextField->displayVram, width - 2, 0, 0, a, COL8_000000);

    // printInSheet(displaySheet, 0, 32, temp, COL8_000000);
    // putSingleChar(longTextField->displaySheet, width - 2, 0, 0, 'X', COL8_000000);

    //处理键盘事件
    longTextFieldSheet->systemActionManager = allocaMemory(getMemoryManager(), sizeof(struct ActionManager));
    longTextFieldSheet->userActionManager = allocaMemory(getMemoryManager(), sizeof(struct ActionManager));

    longTextFieldSheet->systemActionManager->onKeyPress = &onLongTextFieldKeyDown;
    longTextFieldSheet->systemActionManager->onKeyUp = &onLongTextFieldKeyUp;
    longTextFieldSheet->systemActionManager->onClick = &onLongTextFieldClick;
    longTextFieldSheet->systemActionManager->onTextCursorTick = &onLongTextFieldCursorTick;

    longTextField->currentCursorHeight = longTextField->currentDisplayHeight;
    longTextField->currentCursorWidth = longTextField->currentDisplayWidth;

    // showLongTextFieldCursor(longTextField);
    // updateMoveLongTextField(longTextField);

    // updateSheet(displaySheet);

    // updatePartOfIndexMap(sheet, x, y, x + width, y + height);
    updateIndexMapAndActionMap(longTextFieldSheet);
    fillVramByIndexMap(longTextFieldSheet);
    updateSheet(longTextFieldSheet);

    // moveLongTextFieldToBottom(longTextField);

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
    // char s[32];
    // sprintf(s, "%d %X", '\n', c);
    // setLabelText(statusLabel, s, COL8_FFFFFF);

    // char s4[32];
    // sprintf(s4, "---");
    // setLabelText(statusLabel, s4, COL8_FFFFFF);

    // return;
    struct LongTextField *longTextField = this->attribute[6];
    // char s4[32];
    // sprintf(s4, "%d", longTextField->upButton->systemActionManager);
    // setLabelText(statusLabel, s4, COL8_FFFFFF);
    // return;
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
        deleteSingleCharInLongTextField(longTextField);
    }
    else if (0x20 <= c && c <= 0x7e || c == '\n')
    {

        putSingleCharInLongTextField(longTextField, c);

        unsigned int currentCursorIndexInContentVram = longTextField->currentCursorHeight * 16 * longTextField->displaySheet->width + longTextField->currentCursorWidth * 8;

        // char s[32];
        // sprintf(s, "--%d %d", currentCursorIndexInContentVram, longTextField->displayHeightPointer + (longTextField->displaySheet->height - 16) * longTextField->displaySheet->width);
        // setLabelText(statusLabel, s, COL8_FFFFFF);

        if (currentCursorIndexInContentVram <= longTextField->displayHeightPointer * longTextField->displaySheet->width || currentCursorIndexInContentVram >= longTextField->displayHeightPointer * longTextField->displaySheet->width + (longTextField->displaySheet->height - 16) * longTextField->displaySheet->width)
        {
            moveLongTextFieldToBottom(longTextField);
        }
    }
}

void deleteSingleCharInLongTextField(struct LongTextField *longTextField)
{
    unsigned int contentStrSize = getStringSize(longTextField->content);
    if (contentStrSize == 0)
    {
        return;
    }

    char *pointer = longTextField->content + getStringSize(longTextField->content) - 1;
    *pointer = '\0';
    updateLongTextFieldContent(longTextField);

    unsigned int currentCursorIndexInContentVram = longTextField->currentCursorHeight * 16 * longTextField->displaySheet->width + longTextField->currentCursorWidth * 8;
    if (currentCursorIndexInContentVram <= longTextField->displayHeightPointer * longTextField->displaySheet->width || currentCursorIndexInContentVram >= longTextField->displayHeightPointer * longTextField->displaySheet->width + (longTextField->displaySheet->height - 16) * longTextField->displaySheet->width)
    {
        moveLongTextFieldToBottom(longTextField);
    }
    else
    {
        updateLongTextFieldDisplay(longTextField);
    }
    // moveLongTextFieldToBottom(longTextField);
}

void onLongTextFieldClick()
{
}

void onLongTextFieldUp(struct Sheet *this)
{

    struct LongTextField *longTextField = this->attribute[6];
    // char s[32];
    // sprintf(s, "%d", longTextField->displayHeightPointer);
    // setLabelText(statusLabel, s, COL8_000000);

    longTextField->displayHeightPointer -= 4;
    if (longTextField->displayHeightPointer < 0)
    {
        longTextField->displayHeightPointer = 0;
    }
    // leftMoveLongTextFieldCursor(longTextField);

    updateLongTextFieldDisplay(longTextField);

    // putSingleCharInLongTextField(longTextField, 'A');
    // moveLongTextFieldToBottom(longTextField);
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
    // char s[32];
    // sprintf(s, "%d %d %d %d %d", contentVramX, contentVramY, toX, toY, longTextField->displayHeightPointer);
    // setLabelText(statusLabel, s, COL8_FFFFFF);

    // for (i = (longTextField->displayHeightPointer + y) * displaySheet->width; i < (longTextField->displayHeightPointer + displaySheet->height) * displaySheet->width; i++)
    // {
    //     // if (i == 100)
    //     // {
    //     //     break;
    //     // }
    //     longTextField->displayVram[j] = longTextField->contentVram[i];
    //     j++;
    // }

    // int i, j = 0;
    // for (i = longTextField->displayHeightPointer * displaySheet->width; i < (longTextField->displayHeightPointer + displaySheet->height) * displaySheet->width; i++)
    // {
    //     // if (i == 100)
    //     // {
    //     //     break;
    //     // }
    //     longTextField->displayVram[j] = longTextField->contentVram[i];
    //     j++;
    // }

    updatePartOfIndexMap(longTextField->sheet, displaySheet->x + contentVramX, displaySheet->y + contentVramY - longTextField->displayHeightPointer, displaySheet->x + toX, displaySheet->y + toY - longTextField->displayHeightPointer);
    fillPartOfVramByIndexMap(longTextField->sheet, displaySheet->x + contentVramX, displaySheet->y + contentVramY - longTextField->displayHeightPointer, displaySheet->x + toX, displaySheet->y + toY - longTextField->displayHeightPointer);
    updateSheet(longTextField->sheet);

    // updateIndexMapAndActionMap(longTextField->sheet);
    // fillVramByIndexMap(longTextField->sheet);
    // updateSheet(longTextField->sheet);
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

    updateLongTextFieldCursorChar(longTextField);

    longTextField->currentCursorHeight = longTextField->currentDisplayHeight;
    longTextField->currentCursorWidth = longTextField->currentDisplayWidth;

    showLongTextFieldCursor(longTextField);
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
        // if (i == 100)
        // {
        //     break;
        // }
        longTextField->displayVram[j] = longTextField->contentVram[i];
        j++;
    }

    // fillInSheet(displaySheet, 0, 0, 100, 100, COL8_000000);
    // setLabelText(statusLabel, "f", COL8_000000);

    // char a = '2';
    // putSingleChar(longTextField->displayVram, width - 2, 0, 0, a, COL8_000000);

    // printInSheet(displaySheet, 0, 32, temp, COL8_000000);
    // putSingleChar(longTextField->displaySheet, width - 2, 0, 0, 'X', COL8_000000);

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
    putfonts8_asc(longTextField->contentVram, displaySheet->width, x, y, longTextField->backgroundColor, cursor);
    putSingleChar(longTextField->contentVram, displaySheet->width, x, y, *(longTextField->content + longTextField->currentCursorHeight * longTextField->charPerLine + longTextField->currentCursorWidth), longTextField->fontColor);
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
    if (longTextField->currentCursorHeight >= 0 && longTextField->currentCursorWidth > 0)
    {
        longTextField->currentCursorWidth--;
        if (longTextField->currentCursorWidth < 0)
        {
            longTextField->currentCursorWidth = longTextField->charPerLine;
            longTextField->currentCursorHeight--;
        }

        if (longTextField->currentCursorHeight < 0)
        {
            longTextField->currentCursorHeight = 0;
        }
    }

    showLongTextFieldCursor(longTextField);
}