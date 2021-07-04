#include "app_taskManager.h"

#include "memory.h"

void taskManagerMain()
{
    struct Window *window = createWindow(rootSheetManager.sheet, 30, 30, 250, 40, "TaskManager");

    struct Sheet *labelSheet = createLabel(window->sheet, 0, 19, 250, 16, "", COL8_000000);

    while (true)
    {
        unsigned int left = getUnusedMemoryTotal(getMemoryManager());
        char s[32];
        sprintf(s, "%dByte left.(%dmb)", left, left / (1024 * 1024));
        setLabelText(labelSheet, s, COL8_000000);

        //Timer
        struct FifoItem *item2 = getInFifo(systemTimerManager->fifo);

        if (item2->type == FIFO_TYPE_TIMER)
        {
            void (*callback)() = item2->pointer;
            callback();
        }
    }
}