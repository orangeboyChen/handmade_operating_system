#include "app_calc.h"

#define EQUAL 0
#define PLUS 1
#define MINU 2
#define MULT 3
#define DIV 4
#define CLEAR 5

static int total = 0;
static int lastNum = 0;
static unsigned int lastOperator = 0;
static unsigned int isResult = false;

static char currentNumStr[128];
static struct TextField *displaySheet;
static struct Button *numberButton[10];
static struct Button *operateButton[6];

void calcMain()
{

    struct Window *fatherWindow2 = createWindow(rootSheetManager.sheet, 60, 60, 172, 134, "Calculator");
    fatherWindow2->sheet->userActionManager->onKeyPress = &onCalcWindowKeyPress;
    fatherWindow2->sheet->userActionManager->onKeyUp = &onCalcWindowKeyUp;

    displaySheet = createTextField(fatherWindow2->sheet, 3, 22, 166, 20, "0");
    displaySheet->disabled = true;

    struct Button *button1 = createButton(fatherWindow2->sheet, 3, 44, 40, 20, "1");
    button1->sheet->userActionManager->onClick = &onButton1Click;

    struct Button *button2 = createButton(fatherWindow2->sheet, 45, 44, 40, 20, "2");
    button2->sheet->userActionManager->onClick = &onButton2Click;

    struct Button *button3 = createButton(fatherWindow2->sheet, 87, 44, 40, 20, "3");
    button3->sheet->userActionManager->onClick = &onButton3Click;

    struct Button *button4 = createButton(fatherWindow2->sheet, 3, 66, 40, 20, "4");
    button4->sheet->userActionManager->onClick = &onButton4Click;

    struct Button *button5 = createButton(fatherWindow2->sheet, 45, 66, 40, 20, "5");
    button5->sheet->userActionManager->onClick = &onButton5Click;

    struct Button *button6 = createButton(fatherWindow2->sheet, 87, 66, 40, 20, "6");
    button6->sheet->userActionManager->onClick = &onButton6Click;

    struct Button *button7 = createButton(fatherWindow2->sheet, 3, 88, 40, 20, "7");
    button7->sheet->userActionManager->onClick = &onButton7Click;

    struct Button *button8 = createButton(fatherWindow2->sheet, 45, 88, 40, 20, "8");
    button8->sheet->userActionManager->onClick = &onButton8Click;

    struct Button *button9 = createButton(fatherWindow2->sheet, 87, 88, 40, 20, "9");
    button9->sheet->userActionManager->onClick = &onButton9Click;

    struct Button *buttonC = createButton(fatherWindow2->sheet, 3, 110, 40, 20, "C");
    buttonC->sheet->userActionManager->onClick = &onButtonCClick;

    struct Button *button0 = createButton(fatherWindow2->sheet, 45, 110, 40, 20, "0");
    button0->sheet->userActionManager->onClick = &onButton0Click;

    // char s[32];
    // sprintf(s, "%dMb", getUnusedMemoryTotal(getMemoryManager()) / (1024 * 1024));
    // setLabelText(statusLabel, s, COL8_000000);

    struct Button *buttonPlus = createButton(fatherWindow2->sheet, 129, 44, 40, 20, "+");
    buttonPlus->sheet->userActionManager->onClick = &onButtonPlusClick;

    struct Button *buttonMinu = createButton(fatherWindow2->sheet, 129, 66, 40, 20, "-");
    buttonMinu->sheet->userActionManager->onClick = &onButtonMinuClick;

    struct Button *buttonMult = createButton(fatherWindow2->sheet, 129, 88, 40, 20, "*");
    buttonMult->sheet->userActionManager->onClick = &onButtonMultClick;

    struct Button *buttonDiv = createButton(fatherWindow2->sheet, 129, 110, 40, 20, "/");
    buttonDiv->sheet->userActionManager->onClick = &onButtonDivClick;

    struct Button *buttonResult = createButton(fatherWindow2->sheet, 87, 110, 40, 20, "=");
    buttonResult->sheet->userActionManager->onClick = &onButtonResultClick;

    numberButton[0] = button0;
    numberButton[1] = button1;
    numberButton[2] = button2;
    numberButton[3] = button3;
    numberButton[4] = button4;
    numberButton[5] = button5;
    numberButton[6] = button6;
    numberButton[7] = button7;
    numberButton[8] = button8;
    numberButton[9] = button9;

    operateButton[EQUAL] = buttonResult;
    operateButton[PLUS] = buttonPlus;
    operateButton[MINU] = buttonMinu;
    operateButton[MULT] = buttonMult;
    operateButton[DIV] = buttonDiv;
    operateButton[CLEAR] = buttonC;

    while (true)
    {
        //Timer
        struct FifoItem *item2 = getInFifo(systemTimerManager->fifo);

        if (item2->type == FIFO_TYPE_TIMER)
        {
            void (*callback)() = item2->pointer;
            callback();
        }
    }
}

void onCalcWindowKeyUp(struct Sheet *sheet, char c, unsigned int raw)
{
    if ('0' <= c && c <= '9')
    {
        // handleCalcInput(c - '0');
        struct Sheet *buttonSheet = numberButton[c - '0']->sheet;

        buttonSheet->systemActionManager->onMouseLeftUp(buttonSheet, 0, 0);
    }
    else if (raw == 0x2e)
    {
        struct Sheet *buttonSheet = operateButton[CLEAR]->sheet;
        buttonSheet->systemActionManager->onMouseLeftUp(buttonSheet, 0, 0);
    }
    else if (c == '+')
    {
        struct Sheet *buttonSheet = operateButton[PLUS]->sheet;
        buttonSheet->systemActionManager->onMouseLeftUp(buttonSheet, 0, 0);
    }
    else if (c == '-')
    {
        struct Sheet *buttonSheet = operateButton[MINU]->sheet;
        buttonSheet->systemActionManager->onMouseLeftUp(buttonSheet, 0, 0);
    }
    else if (c == '*')
    {
        struct Sheet *buttonSheet = operateButton[MULT]->sheet;
        buttonSheet->systemActionManager->onMouseLeftUp(buttonSheet, 0, 0);
    }
    else if (c == '/')
    {
        struct Sheet *buttonSheet = operateButton[DIV]->sheet;
        buttonSheet->systemActionManager->onMouseLeftUp(buttonSheet, 0, 0);
    }
    else if (raw == 0x1c)
    {
        struct Sheet *buttonSheet = operateButton[EQUAL]->sheet;
        buttonSheet->systemActionManager->onMouseLeftUp(buttonSheet, 0, 0);
    }
}

void onCalcWindowKeyPress(struct Sheet *sheet, char c, unsigned int raw)
{
    if ('0' <= c && c <= '9')
    {
        // handleCalcInput(c - '0');
        struct Sheet *buttonSheet = numberButton[c - '0']->sheet;
        buttonSheet->systemActionManager->onMouseLeftDown(buttonSheet, 0, 0);
    }
    else if (raw == 0x2e)
    {
        struct Sheet *buttonSheet = operateButton[CLEAR]->sheet;
        buttonSheet->systemActionManager->onMouseLeftDown(buttonSheet, 0, 0);
    }
    else if (c == '+')
    {
        struct Sheet *buttonSheet = operateButton[PLUS]->sheet;
        buttonSheet->systemActionManager->onMouseLeftDown(buttonSheet, 0, 0);
    }
    else if (c == '-')
    {
        struct Sheet *buttonSheet = operateButton[MINU]->sheet;
        buttonSheet->systemActionManager->onMouseLeftDown(buttonSheet, 0, 0);
    }
    else if (c == '*')
    {
        struct Sheet *buttonSheet = operateButton[MULT]->sheet;
        buttonSheet->systemActionManager->onMouseLeftDown(buttonSheet, 0, 0);
    }
    else if (c == '/')
    {
        struct Sheet *buttonSheet = operateButton[DIV]->sheet;
        buttonSheet->systemActionManager->onMouseLeftDown(buttonSheet, 0, 0);
    }
    else if (raw == 0x1c)
    {
        struct Sheet *buttonSheet = operateButton[EQUAL]->sheet;
        buttonSheet->systemActionManager->onMouseLeftDown(buttonSheet, 0, 0);
    }
}

void handleCalcInput(int num)
{
    if (isResult == true)
    {
        onButtonCClick();
        handleCalcInput(num);
        return;
    }
    if (getStringSize(currentNumStr) >= 9)
    {
        return;
    }

    int currentNum = String2Int(currentNumStr);
    currentNum = currentNum * 10 + num;

    sprintf(currentNumStr, "%d", currentNum);
    setTextFieldText(displaySheet, currentNumStr);
}

void operate()
{
    switch (lastOperator)
    {
    case PLUS:
        total += lastNum;
        return;
    case MINU:
        total -= lastNum;
        return;
    case MULT:
        total *= lastNum;
        return;
    case DIV:
        total /= lastNum;
        return;
    default:
        total = lastNum;
    }
}

void handleOperator(unsigned int type)
{
    if (isResult)
    {
        lastNum = total;
        total = 0;
        lastOperator = EQUAL;
    }
    else
    {
        lastNum = String2Int(currentNumStr);
    }

    operate();
    switch (type)
    {
    case PLUS:
        setTextFieldText(displaySheet, "+");
        break;
    case MINU:
        setTextFieldText(displaySheet, "-");
        break;
    case MULT:
        setTextFieldText(displaySheet, "*");
        break;
    case DIV:
        setTextFieldText(displaySheet, "/");
        break;
    }
    lastOperator = type;
    // sprintf(currentNumStr, "%d", total);
    strcpy(currentNumStr, "");
    isResult = false;

    char s[32];
    sprintf(s, "%d", total);
    // setLabelText(statusLabel, s, COL8_000000);
}

void onButton1Click()
{
    handleCalcInput(1);
}

void onButton2Click()
{
    handleCalcInput(2);
}

void onButton3Click()
{
    handleCalcInput(3);
}

void onButton4Click()
{
    handleCalcInput(4);
}

void onButton5Click()
{
    handleCalcInput(5);
}

void onButton6Click()
{
    handleCalcInput(6);
}

void onButton7Click()
{
    handleCalcInput(7);
}

void onButton8Click()
{
    handleCalcInput(8);
}

void onButton9Click()
{
    handleCalcInput(9);
}

void onButton0Click()
{
    handleCalcInput(0);
}

void onButtonPlusClick()
{
    handleOperator(PLUS);
}

void onButtonMinuClick()
{
    handleOperator(MINU);
}

void onButtonMultClick()
{
    handleOperator(MULT);
}

void onButtonDivClick()
{
    handleOperator(DIV);
}

void onButtonResultClick()
{
    lastNum = String2Int(currentNumStr);
    operate();

    char s[32];
    sprintf(s, "%d", total);
    setTextFieldText(displaySheet, s);
    isResult = true;
    // setLabelText(statusLabel, s, COL8_000000);
    // strcpy(currentNumStr, "");

    // lastNum = total;
    // total = 0;
    // lastOperator = 0;
}

void onButtonCClick()
{
    lastNum = 0;
    strcpy(currentNumStr, "");
    total = 0;
    lastOperator = 0;
    setTextFieldText(displaySheet, "0");
    isResult = false;
}
