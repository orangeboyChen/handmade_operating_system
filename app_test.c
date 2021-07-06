#include "app_test.h"

void testMain()
{
    struct Window *fatherWindow = createWindow(rootSheetManager.sheet, 60, 60, 250, 110, "TextField");

    // createWindow(fatherWindow->sheet, 0, 30, 300, 300, "1234");

    struct LongTextField *ltfd = createLongTextFieldWithColor(fatherWindow->sheet, 6, 20, 242, 80, 300, "55", COL8_000000, COL8_FFFFFF);

    while (true)
    {
        // setLabelText(statusLabel, "qqqqqqqq", COL8_000000);
        //Timer
        struct FifoItem *item2 = getInFifo(systemTimerManager->fifo);

        if (item2->type == FIFO_TYPE_TIMER)
        {
            void (*callback)() = item2->pointer;
            callback();
        }
    }
}