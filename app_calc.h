#ifndef _APP_CALC_H
#define _APP_CALC_H

#include "common.h"
#include "fifo.h"
#include "widget.h"

void calcMain();
void onCalcWindowKeyPress(struct Sheet *sheet, char c, unsigned int raw);
void onCalcWindowKeyUp(struct Sheet *sheet, char c, unsigned int raw);
void onButton1Click();
void onButton2Click();
void onButton3Click();
void onButton4Click();
void onButton5Click();
void onButton6Click();
void onButton7Click();
void onButton8Click();
void onButton9Click();
void onButton0Click();
void onButtonPlusClick();
void onButtonMinuClick();
void onButtonMultClick();
void onButtonDivClick();
void onButtonResultClick();
void onButtonCClick();

void handleCalcInput(int num);

#endif