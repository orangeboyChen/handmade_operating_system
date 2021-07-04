#include "app_test.h"

void testMain()
{
    struct Window *fatherWindow = createWindow(rootSheetManager.sheet, 60, 60, 150, 100, "Father1");

    struct LongTextField *ltfd = createLongTextField(fatherWindow->sheet, 6, 20, 130, 50, 300, "abcdefghijklmnopqabcdefghijklmnopqabcdefghijklmnopqabcdefghijklmnopqabcdefghijklmnopqabcdefghijklmnopq");

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