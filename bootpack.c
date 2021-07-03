#include "bootpack.h"
#include "timer.h"
#include "task.h"
#include "window.h"
#include "action.h"

#include "app_calc.h"

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
	struct Window *fatherWindow = createWindow(rootSheet, 60, 60, 150, 100, "Father1");
	// struct Button *btn = createButton(fatherWindow->sheet, 6, 20, 80, 20, "button");
	// struct TextField *tfd = createTextField(fatherWindow->sheet, 6, 50, 80, 20, "Ha");

	// struct Window *win = createWindow(rootSheet, 30, 30, 100, 60, "Father");
	// struct Window *win2 = createWindow(rootSheet, 60, 60, 200, 60, "Fatheraaa");

	// releaseWindow(fatherWindow);
	statusLabel = createLabel(rootSheet, 0, 32, 320, 16, "", COL8_FFFFFF);
	setFixedBottom(statusLabel);

	init_pit();
	initKeyboard();
	enableMouse();
	io_out8(PIC0_IMR, 0xf8);
	io_out8(PIC1_IMR, 0xef);

	initSystemTimerManager();
	struct Task *task_a = initTask(memoryManager);
	systemFifo.task = task_a;
	runTask(task_a, 1, 100);

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

	struct Task *task_b;
	task_b = allocaTask();
	task_b->tss.esp = allocaMemory(memoryManager, 64 * 1024) + 64 * 1024 - 8;
	task_b->tss.eip = (int)&calcMain;
	task_b->tss.es = 1 * 8;
	task_b->tss.cs = 2 * 8;
	task_b->tss.ss = 1 * 8;
	task_b->tss.ds = 1 * 8;
	task_b->tss.fs = 1 * 8;
	task_b->tss.gs = 1 * 8;
	runTask(task_b, 1, 2);

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

void task_b_main()
{
	while (true)
	{
		// setLabelText(statusLabel, "taskb==", COL8_000000);
		// io_hlt();

		struct FifoItem *item = getInFifo(&systemFifo);

		// // io_hlt();

		if (item->type == FIFO_TYPE_TIMER)
		{
			void (*callback)() = item->pointer;
			callback();
		}
	}
}
