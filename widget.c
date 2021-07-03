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
    struct Sheet *textSheet;
    if (labelX <= 5)
    {
        labelX = 5;
        // title = "...";
        textSheet = createLabelWithBackground(buttonSheet, labelX, 2, getStringSize("...") * 8, 16, "...", COL8_000000, COL_TRANSPARENT);
    }
    else
    {
        textSheet = createLabelWithBackground(buttonSheet, labelX, 2, getStringSize(title) * 8, 16, title, COL8_000000, COL_TRANSPARENT);
    }

    buttonSheet->systemActionManager = allocaMemory(getMemoryManager(), sizeof(struct ActionManager));
    buttonSheet->userActionManager = allocaMemory(getMemoryManager(), sizeof(struct ActionManager));
    buttonSheet->systemActionManager->onMouseLeftDown = &onButtonSystemMouseLeftDown;
    buttonSheet->systemActionManager->onMouseLeftUp = &onButtonSystemMouseLeftUp;

    buttonSheet->attribute[0] = backgroundSheet;
    buttonSheet->attribute[1] = textSheet;

    struct Button *button = allocaMemory(getMemoryManager(), sizeof(struct Button));
    button->sheet = buttonSheet;
    button->title = title;
    button->titleSheet = textSheet;
    button->backgroundSheet = backgroundSheet;

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

    updateIndexMapAndActionMap(sheet);
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