#include "task.h"

struct TaskManager *taskManager;
struct Timer *taskTimer;

struct Task *initTask(struct MemoryManager *memoryManager)
{
    struct Task *task;
    struct SEGMENT_DESCRIPTOR *gdt = ADR_GDT;
    taskManager = allocaMemory(getMemoryManager(), sizeof(struct TaskManager));

    int i;
    for (i = 0; i < TASK_MAX; i++)
    {
        taskManager->taskStore[i].flags = 0;
        taskManager->taskStore[i].sel = (TASK_GDT0 + i) << 3;
        set_segmdesc(gdt + TASK_GDT0 + i, 103, (int)&taskManager->taskStore[i].tss, AR_TSS32);
    }

    for (i = 0; i < TASK_LEVEL_MAX; i++)
    {
        taskManager->level[i].runningTaskCount = 0;
        taskManager->level[i].headTask = NULL;
        taskManager->level[i].tailTask = NULL;
    }

    task = allocaTask();
    task->flags = 2;
    task->priority = 2;
    task->level = 0;

    addTask(task);
    switchTaskLevel();
    load_tr(task->sel);
    taskManager->currentTask = task;
    // switchTask();
    unsigned int id = setSystemTimer(task->priority, &switchTask);
    // char s4[32];
    // sprintf(s4, "-%d-",
    //         id);
    // setLabelText(statusLabel, s4, COL8_FFFFFF);
    return task;
}

void runTask(struct Task *task, int level, int priority)
{
    if (level < 0)
    {
        level = task->level;
    }

    if (priority > 0)
    {
        task->priority = priority;
    }

    if (task->flags == 2 && task->level != level)
    {
        removeTask(task);
    }

    if (task->flags != 2)
    {
        task->level = level;
        addTask(task);
    }

    taskManager->isNeedChangeLevel = true;
    // setLabelText(statusLabel, "123", COL8_000000);

    return;
}

void addTask(struct Task *task)
{
    struct TaskLevel *taskLevel = &taskManager->level[task->level];

    if (taskLevel->headTask == NULL)
    {
        taskLevel->headTask = taskLevel->tailTask = task;
    }
    else
    {
        taskLevel->tailTask->nextTaskOfLevel = task;
        taskLevel->tailTask = task;
    }
    taskLevel->runningTaskCount++;
    task->flags = 2;
    return;
}

void removeTask(struct Task *task)
{
    struct TaskLevel *taskLevel = &taskManager->level[task->level];

    struct Task *currentTask = taskLevel->headTask;
    if (currentTask == task)
    {
        if (currentTask->nextTaskOfLevel == NULL || currentTask->nextTaskOfLevel == currentTask)
        {
            taskLevel->headTask = NULL;
            taskLevel->tailTask = NULL;
        }
        else
        {
            taskLevel->headTask = task->nextTaskOfLevel;
        }
    }
    else
    {
        while (currentTask->nextTaskOfLevel != NULL)
        {
            if (currentTask->nextTaskOfLevel == task)
            {
                currentTask->nextTaskOfLevel = task->nextTaskOfLevel;
                if (task->nextTaskOfLevel == NULL)
                {
                    taskLevel->tailTask = currentTask;
                }
                break;
            }
            currentTask = currentTask->nextTaskOfLevel;
        }
    }
    task->flags = 1;
    taskLevel->runningTaskCount--;
}

struct Task *allocaTask()
{
    struct Task *task;
    int i;
    for (i = 0; i < TASK_MAX; i++)
    {
        if (taskManager->taskStore[i].flags == 0)
        {
            task = &taskManager->taskStore[i];
            task->flags = 1;
            task->tss.eflags = 0x00000202;
            task->tss.eax = 0;
            task->tss.ecx = 0;
            task->tss.edx = 0;
            task->tss.ebx = 0;
            task->tss.ebp = 0;
            task->tss.esi = 0;
            task->tss.edi = 0;
            task->tss.es = 0;
            task->tss.ds = 0;
            task->tss.fs = 0;
            task->tss.gs = 0;
            task->tss.ldtr = 0;
            task->tss.iomap = 0x40000000;
            return task;
        }
    }
}

void switchTaskLevel()
{
    int i;
    for (i = TASK_LEVEL_MAX - 1; i >= 0; i--)
    {
        if (taskManager->level[i].runningTaskCount > 0)
        {
            break;
        }
    }

    taskManager->currentLevel = i;
    taskManager->isNeedChangeLevel = false;
    return;
}

void sleepTask(struct Task *task)
{
    struct Task *currentTask;
    if (task->flags == 2)
    {
        currentTask = getCurrentTask();
        removeTask(task);
        if (task == currentTask)
        {
            switchTaskLevel();
            currentTask = getCurrentTask();
            farjmp(0, currentTask->sel);
        }
    }
}

struct Task *getCurrentTask()
{
    struct TaskLevel *taskLevel = &taskManager->level[taskManager->currentLevel];
    return taskLevel->headTask;
}

void switchTask()
{

    switchTaskLevel();
    struct TaskLevel *taskLevel = &taskManager->level[taskManager->currentLevel];
    struct Task *currentTask = taskLevel->headTask;
    setSystemTimer(currentTask->priority, &switchTask);

    if (taskLevel == NULL || currentTask == NULL)
    {
        return;
    }

    // char c[32];
    // sprintf(c, "%d %d %d", currentTask->nextTaskOfLevel->sel / 8, currentTask->sel / 8, currentTask->level);
    // setLabelText(statusLabel, c, COL8_848400);

    // setSystemTimer(1000, &switchTask);

    // if (currentTask == taskLevel->tailTask)
    // {
    //     removeTask(currentTask);
    //     if (currentTask->level > 1)
    //     {
    //         currentTask->level--;
    //     }
    //     addTask(currentTask);
    // }
    removeTask(currentTask);
    if (currentTask->level > 1)
    {
        currentTask->level--;
    }
    addTask(currentTask);

    if (taskManager->currentTask != currentTask || taskManager->currentTask->sel != currentTask->sel)
    {
        taskManager->currentTask = currentTask;
        farjmp(0, currentTask->sel);
    }

    return;

    // struct Task *newTask, *currentTask = taskLevel->headTask;

    // if (taskManager->isNeedChangeLevel)
    // {
    //     switchTaskLevel();
    //     taskLevel = &taskManager->level[taskManager->currentLevel];
    // }

    // newTask = taskLevel->headTask;

    // if (newTask != currentTask)
    // {
    //     // if (newTask->level == 32)
    //     // {
    //     //     farjmp(0, newTask->sel);
    //     // }
    //     // farjmp(0, newTask->sel);
    // }
}