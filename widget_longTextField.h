#ifndef _WIDGET_LONGTEXTFIELD_H
#define _WIDGET_LONGTEXTFIELD_H

#include "widget.h"
struct LongTextField
{
    char *contentVram;
    char *displayVram;
    unsigned int height;
    unsigned int maxHeight;
    unsigned int width;
    unsigned int currentDisplayHeight;
    unsigned int currentDisplayWidth;
    unsigned int charPerLine;

    int displayHeightPointer; //displayVram对应的像素行号

    char *content;

    struct Sheet *displaySheet;
    struct Sheet *backgroundSheet;
    struct Sheet *sheet;

    struct Sheet *upButton;
    struct Sheet *downButton;

    char backgroundColor;
    char fontColor;

    int currentCursorHeight;
    int currentCursorWidth;
};

struct LongTextField *createLongTextField(struct Sheet *fatherSheet, short x, short y, short width, short height, short maxHeight, char *content);
void onLongTextFieldUp(struct Sheet *this);
void updateLongTextFieldDisplay(struct LongTextField *longTextField);
void onLongTextFieldDown(struct Sheet *this);
void onLongTextFieldKeyDown(struct Sheet *this, char c, unsigned int raw);
void onLongTextFieldKeyUp(struct Sheet *this, char c, unsigned int raw);
void onLongTextFieldClick();
void putSingleCharInLongTextField(struct LongTextField *longTextField, char c);
void moveLongTextFieldToBottom(struct LongTextField *longTextField);
void deleteSingleCharInLongTextField(struct LongTextField *longTextField);
void onLongTextFieldCursorTick(struct Sheet *longTextFieldSheet, unsigned int tick);
void updatePartOfLongTextFieldDisplay(struct LongTextField *longTextField, unsigned int x, unsigned int y);
void updateLongTextFieldCursorChar(struct LongTextField *longTextField);
void onLongTextFieldCursorTick(struct Sheet *longTextFieldSheet, unsigned int tick);
void updateLongTextFieldContent(struct LongTextField *longTextField);
void showLongTextFieldCursor(struct LongTextField *longTextField);
struct LongTextField *createLongTextFieldWithColor(struct Sheet *sheet, short x, short y, short width, short height, short maxHeight, char *content, char backgroundColor, char fontColor);
void rightMoveLongTextFieldCursor(struct LongTextField *longTextField);
void leftMoveLongTextFieldCursor(struct LongTextField *longTextField);
int getCharIndexInLongTextFieldContent(struct LongTextField *longTextField, unsigned int x, unsigned int y);
void getCursorPositionByCharIndexInLongTextField(struct LongTextField *longTextField, unsigned int charIndex, int *resultArray);

void putSingleCharInLongTextFieldByCurrentCursor(struct LongTextField *longTextField, char c);
#endif