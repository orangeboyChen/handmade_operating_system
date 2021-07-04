#ifndef _TIMER_H
#define _TIMER_H

#include "common.h"
#include "memory.h"
#include "fifo.h"
#define TIMER_MAX 512

struct Timer
{
    unsigned long countdown;
    unsigned long targetTime;
    void (*onTimerStopCallback)();
    struct Timer *nextTimer;
    unsigned int timerId;
    unsigned int isUsing;
};

struct TimerManager
{
    unsigned long currentTime;
    struct Timer *firstTimer;
    struct Timer *lastTimer;
    struct Timer timerStore[TIMER_MAX];
    void (*onTick)();
    struct Fifo *fifo;
};
extern struct TimerManager *systemTimerManager;
struct TimerManager *get();
struct TimerManager *initSystemTimerManager();
void onSystemTimerTick();
void stopTimerOfSystem(unsigned int timerId);
unsigned int setSystemTimer(unsigned long countdown, void (*onTimerStopCallback)());
unsigned int setTimer(struct TimerManager *systemTimerManager, unsigned long countdown, void (*onTimerStopCallback)());
struct TimerManager *allocaTimerManager(void (*onTick)());
long getSystemTime();
#endif