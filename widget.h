#ifndef _WIDGET_H
#define _WIDGET_H

#include "common.h"
#include "sheet.h"
#include "utils.h"
#include "action.h"
#include "graphic.h"

#define BUTTON 1
#define TEXTFIELD 2

struct Button
{
    struct Sheet *titleSheet;
    struct ActionManager *userActionManager;
    char *title;
    struct Sheet *backgroundSheet;
    struct Sheet *sheet;
};

struct TextField
{
    struct Sheet *displaySheet;
    char *content;
    char *displayContent;
    unsigned int maxChar;
    unsigned int currentSize;
    struct Sheet *backgroundSheet;
    struct Sheet *sheet;

    unsigned int disabled;
};

struct LongTextField
{
    char *contentVram;
    char *displayVram;
    unsigned int height;
    unsigned int maxHeight;
    unsigned int width;
    unsigned int currentHeightCursor;
    unsigned int currentWidthCursor;
    unsigned int charPerLine;

    int displayHeightPointer; //displayVram对应的像素行号

    char *content;

    struct Sheet *displaySheet;
    struct Sheet *backgroundSheet;
    struct Sheet *sheet;

    struct Sheet *upButton;
    struct Sheet *downButton;
};

struct Sheet *createLabel(struct Sheet *sheet, short x, short y, short width, short height, char *c, char color);
void setLabelText(struct Sheet *sheet, char *c, char color);
struct Sheet *createLabelWithBackground(struct Sheet *sheet, short x, short y, short width, short height, char *c, char color, char background);

struct Button *createButton(struct Sheet *sheet, short x, short y, short width, short height, char *title);
void onButtonSystemMouseLeftDown(struct Sheet *this, unsigned int x, unsigned int y);
void onButtonSystemMouseLeftUp(struct Sheet *this, unsigned int x, unsigned int y);

struct TextField *createTextField(struct Sheet *sheet, short x, short y, short width, short height, char *content);
void setTextFieldText(struct TextField *this, char *c);
void onSystemTextfieldClick();
void onSystemTextfieldKeyPress(struct Sheet *this, char c, unsigned int raw);
void onSystemTextfieldKeyUp(struct Sheet *this, char c, unsigned int raw);

struct LongTextField *createLongTextField(struct Sheet *fatherSheet, short x, short y, short width, short height, short maxHeight, char *content);
void onLongTextFieldUp(struct Sheet *this);
void updateLongTextField(struct LongTextField *longTextField);
void onLongTextFieldDown(struct Sheet *this);
void onLongTextFieldKeyDown(struct Sheet *this, char c, unsigned int raw);
void onLongTextFieldKeyUp(struct Sheet *this, char c, unsigned int raw);
void onLongTextFieldClick();
#endif