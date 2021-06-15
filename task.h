#ifndef _TASK_H
#define _TASK_H

#include "memory.h"
#include "timer.h"
#include "dsctbl.h"

#define TASK_MAX 1024
#define TASK_GDT0 3
#define TASK_LEVEL_ITEM_MAX 128
#define TASK_LEVEL_MAX 100

struct Tss
{
    int backlink, esp0, ss0, esp1, ss1, esp2, ss2, cr3;
    int eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
    int es, cs, ss, ds, fs, gs;
    int ldtr, iomap;
};

struct Task
{
    int sel, flags;
    int level, priority;
    struct Tss tss;
    struct Task *nextTaskOfLevel;
};

struct TaskLevel
{
    unsigned int runningTaskCount;
    struct Task *headTask;
    struct Task *tailTask;
};

struct TaskManager
{
    int currentLevel;
    char isNeedChangeLevel;
    struct TaskLevel level[TASK_LEVEL_MAX];
    struct Task taskStore[TASK_MAX];
};

void switchTask();
struct Task *getCurrentTask();
void sleepTask(struct Task *task);
void switchTaskLevel();
struct Task *allocaTask();
void removeTask(struct Task *task);
void addTask(struct Task *task);
void runTask(struct Task *task, int level, int priority);
struct Task *initTask(struct MemoryManager *memoryManager);

#endif