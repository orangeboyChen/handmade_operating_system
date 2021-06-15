#include "timer.h"

struct TimerManager *systemTimerManager;

unsigned int setTimer(struct TimerManager *timerManager, unsigned long countdown, void (*onTimerStopCallback)())
{
    struct Timer *timer = getTimerInstance(timerManager);
    timer->countdown = countdown;
    timer->targetTime = timerManager->currentTime + countdown;
    timer->onTimerStopCallback = onTimerStopCallback;

    if (timer->targetTime < timerManager->firstTimer->targetTime)
    {
        timer->nextTimer = timerManager->firstTimer;
        timerManager->firstTimer = timer;
    }
    else
    {
        struct Timer *currentTimer = timerManager->firstTimer;
        while (currentTimer->nextTimer != NULL)
        {
            if (currentTimer->nextTimer->targetTime > timer->targetTime)
            {
                break;
            }
            currentTimer = currentTimer->nextTimer;
        }
        timer->nextTimer = currentTimer->nextTimer;
        currentTimer->nextTimer = timer;
    }

    return timer->timerId;
}

struct TimerManager *createTimerManager(struct Fifo *fifo)
{
    systemTimerManager = allocaMemory(getMemoryManager(), sizeof(struct TimerManager));

    int i;
    for (i = 0; i < TIMER_MAX; i++)
    {
        systemTimerManager->timerStore[i].isUsing = false;
        systemTimerManager->timerStore[i].timerId = i;
    }

    struct Timer *idleTimer = getTimerInstance(systemTimerManager);
    idleTimer->targetTime = 0xFFFFFFFFFFFFFFFF;
    systemTimerManager->firstTimer = idleTimer;
    systemTimerManager->lastTimer = idleTimer;

    systemTimerManager->fifo = fifo;
}

void initSystemTimerManager()
{
    systemTimerManager = createTimerManager(&systemFifo);
}

unsigned int setSystemTimer(unsigned long countdown, void (*onTimerStopCallback)())
{
    return setTimer(systemTimerManager, countdown, onTimerStopCallback);
}

struct Timer *getTimerInstance(struct TimerManager *timerManager)
{
    int i;
    for (i = 0; i < TIMER_MAX; i++)
    {
        if (systemTimerManager->timerStore[i].isUsing == false)
        {
            systemTimerManager->timerStore[i].isUsing = true;
            return &systemTimerManager->timerStore[i];
        }
    }

    return NULL;
}

void onTick(struct TimerManager *timerManager)
{
    timerManager->currentTime++;

    if (timerManager->firstTimer->targetTime > timerManager->currentTime)
    {
        return;
    }

    struct Timer *currentTimer = timerManager->firstTimer;
    while (currentTimer->nextTimer != NULL)
    {
        if (currentTimer->targetTime <= timerManager->currentTime)
        {
            struct Timer *temp = currentTimer;
            currentTimer = currentTimer->nextTimer;
            timerManager->firstTimer = currentTimer;

            int isUsing = temp->isUsing;
            void (*callback)() = temp->onTimerStopCallback;

            temp->isUsing = false;
            temp->onTimerStopCallback = NULL;

            if (isUsing)
            {
                putInFifoWithPointer(timerManager->fifo, FIFO_TYPE_TIMER, NULL, callback);
            }
        }
        else
        {
            break;
        }
    }
}

void onSystemTimerTick()
{
    onTick(systemTimerManager);
}

void stopTimer(struct TimerManager *timerManager, unsigned int timerId)
{
    timerManager->timerStore[timerId].isUsing = false;
}

void stopTimerOfSystem(unsigned int timerId)
{
    stopTimer(systemTimerManager, timerId);
}

struct TimerManager *get()
{
    return systemTimerManager;
}