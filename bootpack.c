#include "bootpack.h"
#include "timer.h"
#include "task.h"
#include "window.h"
#include "action.h"

#include "app_calc.h"
#include "app_taskManager.h"
#include "app_test.h"

struct Sheet *statusLabel;
int i = 0;
struct Window *faWin;

void HariMain(void)
{
	init_gdtidt();
	init_pic();
	io_sti();

	initFifo(&systemFifo);

	struct BootInfo *bootInfo = getBootInfo();
	initPalette();

	struct MemoryManager *memoryManager = getMemoryManager();
	initMemoryManage(memoryManager);

	struct Sheet *rootSheet = initRootSheet();
	initMouseCursorSheet(rootSheet);
	initDesktop(rootSheet);
	statusLabel = createLabel(rootSheet, 0, 32, 320, 16, "", COL8_FFFFFF);

	init_pit();
	initKeyboard();
	enableMouse();
	io_out8(PIC0_IMR, 0xf8);
	io_out8(PIC1_IMR, 0xef);

	initSystemTimerManager();
	struct Task *task_a = initTask(memoryManager);
	systemFifo.task = task_a;
	runTask(task_a, 1, 50);

	// struct Task *task_b[5];
	// for (i = 0; i < 5; i++)
	// {
	// 	task_b[i] = allocaTask();
	// 	task_b[i]->tss.esp = allocaMemory(memoryManager, 64 * 1024) + 64 * 1024 - 8;
	// 	task_b[i]->tss.eip = (int)&calcMain;
	// 	task_b[i]->tss.es = 1 * 8;
	// 	task_b[i]->tss.cs = 2 * 8;
	// 	task_b[i]->tss.ss = 1 * 8;
	// 	task_b[i]->tss.ds = 1 * 8;
	// 	task_b[i]->tss.fs = 1 * 8;
	// 	task_b[i]->tss.gs = 1 * 8;
	// 	runTask(task_b[i], 1, 10);
	// }

	// struct Task *task_b;
	// task_b = allocaTask();
	// task_b->tss.esp = allocaMemory(memoryManager, 64 * 1024) + 64 * 1024 - 8;
	// task_b->tss.eip = (int)&calcMain;
	// task_b->tss.es = 1 * 8;
	// task_b->tss.cs = 2 * 8;
	// task_b->tss.ss = 1 * 8;
	// task_b->tss.ds = 1 * 8;
	// task_b->tss.fs = 1 * 8;
	// task_b->tss.gs = 1 * 8;
	// runTask(task_b, 1, 2);

	struct Task *task_c;
	task_c = allocaTask();
	task_c->tss.esp = allocaMemory(memoryManager, 64 * 1024) + 64 * 1024 - 8;
	task_c->tss.eip = (int)&testMain;
	task_c->tss.es = 1 * 8;
	task_c->tss.cs = 2 * 8;
	task_c->tss.ss = 1 * 8;
	task_c->tss.ds = 1 * 8;
	task_c->tss.fs = 1 * 8;
	task_c->tss.gs = 1 * 8;
	runTask(task_c, 1, 2);

	setSystemTimer(100, &textCursorTick);

	while (1)
	{
		struct FifoItem *item = getInFifo(&systemFifo);
		if (item == NULL)
		{
			io_stihlt();
		}
		else if (item->type != NULL)
		{
			io_sti();
			if (item->type == FIFO_TYPE_KEYBOARD)
			{
				// char s4[32];
				// sprintf(s4, "%02X", item->data);
				// setLabelText(statusLabel, s4, COL8_FFFFFF);
				analyseKeyborardData(item->data);
			}
			else if (item->type == FIFO_TYPE_MOUSE)
			{
				if (putInMouseData(&mouseData, item->data) == 1)
				{
					updateMouseCursorSheet(mouseData.moveX, mouseData.moveY);
					handleOnMouseMoveOfRoot(mouseData.x, mouseData.y);
					// int x = mouseData.x, y = mouseData.y;
					// struct Sheet *cs = rootSheet->sheetStore[rootSheet->actionMap[y * rootSheet->width + (x - 1)]];
					// x -= cs->x;
					// y -= cs->y;

					// char s4[32];
					// sprintf(s4, "%d-%d %d-%d %d %d %d %d (%d)",
					// 		rootSheet->vram[mouseData.y * rootSheet->width + (mouseData.x - 1)],
					// 		rootSheet->actionMap[mouseData.y * rootSheet->width + (mouseData.x - 1)],
					// 		cs->vram[y * cs->width + (x - 1)],
					// 		cs->actionMap[y * cs->width + (x - 1)],
					// 		cs->index,
					// 		cs->fatherSheet->index,
					// 		x,
					// 		y,
					// 		get()->firstTimer->timerId);
					// setLabelText(statusLabel, s4, COL8_FFFFFF);

					// if ((mouseData.btn &W 0x01) == 0x01)
					// char s4[32];
					// sprintf(s4, "%d",
					// 		mouseData.btn);

					// setLabelText(statusLabel, s4, COL8_FFFFFF);

					//mouse left down
					if ((mouseData.preBtn & 0x01) != 0x01 && (mouseData.btn & 0x01) == 0x01)
					{
						handleOnMouseLeftDownOfRoot(mouseData.x, mouseData.y);
					}

					//mouse left up
					if ((mouseData.preBtn & 0x01) == 0x01 && (mouseData.btn & 0x01) != 0x01)
					{
						handleOnMouseLeftUpOfRoot(mouseData.x, mouseData.y);
					}

					//mouse right down
					if ((mouseData.preBtn & 0x02) != 0x02 && (mouseData.btn & 0x02) == 0x02)
					{
						handleOnMouseRightDownOfRoot(mouseData.x, mouseData.y);
					}

					//mouse right up
					if ((mouseData.preBtn & 0x02) == 0x01 && (mouseData.btn & 0x02) != 0x02)
					{
						handleOnMouseRightUpOfRoot(mouseData.x, mouseData.y);
					}

					//mouse middle down
					if ((mouseData.preBtn & 0x04) != 0x04 && (mouseData.btn & 0x01) == 0x04)
					{
						handleOnMouseMiddleDownOfRoot(mouseData.x, mouseData.y);
					}

					//mouse middle up
					if ((mouseData.preBtn & 0x04) == 0x04 && (mouseData.btn & 0x01) != 0x04)
					{
						handleOnMouseMiddleUpOfRoot(mouseData.x, mouseData.y);
					}

					//mouse left click
					if ((mouseData.btn & 0x01) == 0x01)
					{
						handleOnClickOfRoot(mouseData.x, mouseData.y);

						long currentTime = getSystemTime();
						if (mouseData.lastClickTimestamp - currentTime < 50)
						{
							//double click
							handleOnDoubleClickOfRoot(mouseData.x, mouseData.y);
						}
						mouseData.lastClickTimestamp = currentTime;
					}

					//mouse right click
					if ((mouseData.btn & 0x02) == 0x02)
					{
						handleOnRightClickOfRoot(mouseData.x, mouseData.y);
					}

					//mouse middle click
					if ((mouseData.btn & 0x04) == 0x04)
					{
						handleOnMiddleClickOfRoot(mouseData.x, mouseData.y);
					}
				}
			}
			else if (item->type == FIFO_TYPE_TIMER)
			{
				void (*callback)() = item->pointer;
				callback();
			}
		}

		//Timer
		struct FifoItem *item2 = getInFifo(systemTimerManager->fifo);

		// // io_hlt();
		// char c[32];
		// sprintf(c, "%d", item2->data);
		// setLabelText(statusLabel, c, COL8_848400);

		if (item2->type == FIFO_TYPE_TIMER)
		{
			void (*callback)() = item2->pointer;
			callback();
		}
	}
}

static unsigned int tick = false;
void textCursorTick()
{

	setSystemTimer(50, &textCursorTick);
	tick = !tick;
	struct Sheet *newTextFieldSheet = NULL;

	struct ActiveSheetItem *activeSheetItem = windowsManager.lastActiveSheet;

	// char s[32];
	// sprintf(s, "%d %d", windowsManager.firstActiveSheet, tick);
	// setLabelText(statusLabel, s, COL8_000000);

	while (activeSheetItem != NULL)
	{
		// if (activeSheetItem->sheet->attribute[5] != 0)
		// {
		// 	char s[32];
		// 	sprintf(s, "%d %d", activeSheetItem->sheet->attribute[5], tick);
		// 	setLabelText(statusLabel, s, COL8_000000);
		// }

		if (activeSheetItem->sheet->attribute[5] == LONGTEXTFIELD)
		{
			newTextFieldSheet = activeSheetItem->sheet;
			break;
		}

		activeSheetItem = activeSheetItem->previousSheet;
	}

	// if (newTextFieldSheet == NULL)
	// {
	// 	return;
	// }

	if (newTextFieldSheet != windowsManager.activeTextField)
	{
		if (windowsManager.activeTextField != NULL && windowsManager.activeTextField->systemActionManager != NULL &&
			windowsManager.activeTextField->systemActionManager->onTextCursorTick != NULL)
		{

			windowsManager.activeTextField->systemActionManager->onTextCursorTick(windowsManager.activeTextField, false);
		}

		windowsManager.activeTextField = newTextFieldSheet;
	}

	if (windowsManager.activeTextField != NULL && windowsManager.activeTextField->systemActionManager != NULL &&
		windowsManager.activeTextField->systemActionManager->onTextCursorTick != NULL)
	{
		// char s[32];
		// sprintf(s, "%d", tick);
		// setLabelText(statusLabel, s, COL8_000000);
		windowsManager.activeTextField->systemActionManager->onTextCursorTick(windowsManager.activeTextField, tick);
	}
}