#ifndef _ACTION_H
#define _ACTION_H
#include "task.h"
#include "sheet.h"
#include "window.h"

struct ActionManager
{
    void (*onClick)(struct Sheet *this, unsigned int x, unsigned y);
    void (*onMouseIn)(struct Sheet *this, unsigned int x, unsigned y);

    void (*onMouseLeftDown)(struct Sheet *this, unsigned int x, unsigned int y);
    void (*onMouseLeftUp)(struct Sheet *this, unsigned int x, unsigned int y);
    void (*onMouseRightDown)(struct Sheet *this, unsigned int x, unsigned int y);
    void (*onMouseRightUp)(struct Sheet *this, unsigned int x, unsigned int y);
    void (*onMouseMiddleDown)(struct Sheet *this, unsigned int x, unsigned int y);
    void (*onMouseMiddleUp)(struct Sheet *this, unsigned int x, unsigned int y);

    void (*onDoubleClick)(struct Sheet *this, unsigned int x, unsigned int y);
    void (*onRightClick)(struct Sheet *this, unsigned int x, unsigned int y);
    void (*onMiddleClick)(struct Sheet *this, unsigned int x, unsigned int y);

    void (*onKeyPress)(struct Sheet *this, char c, unsigned int raw);
    void (*onKeyUp)(struct Sheet *this, char c, unsigned int raw);

    void (*onTextCursorTick)(struct Sheet *this, unsigned int tick);
};

void handleOnClickOfRoot(unsigned int x, unsigned int y);
void handleOnMouseMoveOfRoot(unsigned int x, unsigned int y);
void handleOnMouseLeftDownOfRoot(unsigned int x, unsigned int y);
void handleOnMouseLeftUpOfRoot(unsigned int x, unsigned int y);
void handleOnMouseRightDownOfRoot(unsigned int x, unsigned int y);
void handleOnMouseRightUpOfRoot(unsigned int x, unsigned int y);
void handleOnMouseMiddleDownOfRoot(unsigned int x, unsigned int y);
void handleOnMouseMiddleUpOfRoot(unsigned int x, unsigned int y);
void handleOnDoubleClickOfRoot(unsigned int x, unsigned int y);
void handleOnRightClickOfRoot(unsigned int x, unsigned int y);
void handleOnMiddleClickOfRoot(unsigned int x, unsigned int y);

void handleOnClick(struct Sheet *sheet, unsigned int x, unsigned int y);
void handleOnMouseMove(struct Sheet *sheet, unsigned int x, unsigned int y);
void handleOnMouseLeftDown(struct Sheet *sheet, unsigned int x, unsigned int y);
void handleOnMouseLeftUp(struct Sheet *sheet, unsigned int x, unsigned int y);
void handleOnMouseRightDown(struct Sheet *sheet, unsigned int x, unsigned int y);
void handleOnMouseRightUp(struct Sheet *sheet, unsigned int x, unsigned int y);
void handleOnMouseMiddleDown(struct Sheet *sheet, unsigned int x, unsigned int y);
void handleOnMouseMiddleUp(struct Sheet *sheet, unsigned int x, unsigned int y);
void handleOnDoubleClick(struct Sheet *sheet, unsigned int x, unsigned int y);
void handleOnRightClick(struct Sheet *sheet, unsigned int x, unsigned int y);
void handleOnMiddleClick(struct Sheet *sheet, unsigned int x, unsigned int y);

void handleKeyPress(char key, unsigned int raw);
void handleKeyUp(char key, unsigned int raw);

#endif
