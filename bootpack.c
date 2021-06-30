#include "bootpack.h"
#include "timer.h"
#include "task.h"

struct Sheet *statusLabel;
int i = 0;
void showSomething()
{
	i++;

	// setTimer(10, &showSomething);

	// char c[32];
	// sprintf(c, "%ds", i);
	// setLabelText(statusLabel, c, COL8_848400);

	return;
}

void switchA();
void switchT();

void switchT()
{
	setSystemTimer(1, &switchA);
	farjmp(0, 4 * 8);
}

void switchA()
{
	setSystemTimer(1, &switchT);
	farjmp(0, 3 * 8);
}

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
	rootSheet->index = 999;
	initMouseCursorSheet(rootSheet);
	initDesktop(rootSheet);
	createWindow(rootSheet, 60, 60, 80, 80, "Father1");
	// struct Sheet *win = createWindow(rootSheet, 30, 30, 200, 100, "Father2");
	// createWindow(win, 5, 5 + 18, 180, 40, "Son1");

	statusLabel = createLabel(rootSheet, 0, 32, 320, 16, "", COL8_FFFFFF);

	setFixedBottom(statusLabel);

	initSystemTimerManager();

	init_pit();
	io_out8(PIC0_IMR, 0xf8);
	io_out8(PIC1_IMR, 0xef);

	initKeyboard();
	enableMouse();

	struct Task *task_a = initTask(memoryManager);
	systemFifo.task = task_a;
	runTask(task_a, 1, 100);

	struct Task *task_b[3];
	for (i = 0; i < 3; i++)
	{
		task_b[i] = allocaTask();
		task_b[i]->tss.esp = allocaMemory(memoryManager, 64 * 1024) + 64 * 1024 - 8;
		task_b[i]->tss.eip = (int)&task_b_main;
		task_b[i]->tss.es = 1 * 8;
		task_b[i]->tss.cs = 2 * 8;
		task_b[i]->tss.ss = 1 * 8;
		task_b[i]->tss.ds = 1 * 8;
		task_b[i]->tss.fs = 1 * 8;
		task_b[i]->tss.gs = 1 * 8;
		runTask(task_b[i], 1, 10);
	}

	// int task_b_esp = allocaMemory(memoryManager, 64 * 1024) + 64 * 1024;
	// struct Tss tss_a, tss_b;
	// tss_a.ldtr = 0;
	// tss_a.iomap = 0x40000000;
	// tss_b.ldtr = 0;
	// tss_b.iomap = 0x40000000;

	// struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR *)ADR_GDT;
	// set_segmdesc(gdt + 3, 103, (int)&tss_a, AR_TSS32);
	// set_segmdesc(gdt + 4, 103, (int)&tss_b, AR_TSS32);
	// load_tr(3 * 8);

	// tss_b.eip = (int)&task_b_main;
	// tss_b.eflags = 0x00000202; /* IF = 1; */
	// tss_b.eax = 0;
	// tss_b.ecx = 0;
	// tss_b.edx = 0;
	// tss_b.ebx = 0;
	// tss_b.esp = task_b_esp;
	// tss_b.ebp = 0;
	// tss_b.esi = 0;
	// tss_b.edi = 0;
	// tss_b.es = 1 * 8;
	// tss_b.cs = 2 * 8;
	// tss_b.ss = 1 * 8;
	// tss_b.ds = 1 * 8;
	// tss_b.fs = 1 * 8;
	// tss_b.gs = 1 * 8;

	// setSystemTimer(100, &switchT);
	// setSystemTimer(300, &switchT);
	// setSystemTimer(500, &switchT);
	// setSystemTimer(700, &switchT);
	// setSystemTimer(900, &switchT);

	// int timerDev = 0;
	while (1)
	{
		// io_cli();
		// setLabelText(statusLabel, "main", COL8_000000);
		struct FifoItem *item = getInFifo(&systemFifo);

		// char c[32];
		// sprintf(c, "%ds", systemTimerManager->currentTime);
		// setLabelText(statusLabel, c, COL8_848400);
		if (item == NULL)
		{
			// timerDev++;
			// if (timerDev % 100 == 0)
			// {
			// 	char s4[32];
			// 	sprintf(s4, "(%d)",
			// 			timerDev);
			// 	setLabelText(statusLabel, s4, COL8_FFFFFF);
			// }

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
			}
			else if (item->type == FIFO_TYPE_MOUSE)
			{
				if (putInMouseData(&mouseData, item->data) == 1)
				{
					updateMouseCursorSheet(mouseData.moveX, mouseData.moveY);
					// int x = mouseData.x, y = mouseData.y;
					// struct Sheet *cs = rootSheet->sheetStore[rootSheet->indexMap[y * rootSheet->width + (x - 1)]];
					// x -= cs->x;
					// y -= cs->y;

					// char s4[32];
					// sprintf(s4, "%d-%d %d-%d %d %d %d %d (%d)",
					// 		rootSheet->vram[mouseData.y * rootSheet->width + (mouseData.x - 1)],
					// 		rootSheet->indexMap[mouseData.y * rootSheet->width + (mouseData.x - 1)],
					// 		cs->vram[y * cs->width + (x - 1)],
					// 		cs->indexMap[y * cs->width + (x - 1)],
					// 		cs->index,
					// 		cs->fatherSheet->index,
					// 		x,
					// 		y,
					// 		get()->firstTimer->timerId);
					// setLabelText(statusLabel, s4, COL8_FFFFFF);

					// if ((mouseData.btn & 0x01) == 0x01)
					if (mouseData.btn == 0x01)
					{
						// moveSheet(rootSheet->topSheet->nextSheet->nextSheet, mouseData.x, mouseData.y);
						// setTimer(100, &showSomething);
					}
					// if ((mouseData.btn & 0x02) == 0x02)
					if (mouseData.btn == 0x02)
					{
						struct Sheet *cur = rootSheet->topSheet->nextSheet->nextSheet->topSheet->nextSheet;
						// moveSheet(cur, mouseData.x - cur->fatherSheet->x, mouseData.y - cur->fatherSheet->y);
					}

					// if ((mouseData.btn & 0x04) == 0x04)
					if (mouseData.btn == 0x04)
					{
						struct Sheet *cur = rootSheet->topSheet->nextSheet->nextSheet->nextSheet;
						// moveSheet(cur, mouseData.x - cur->fatherSheet->x, mouseData.y - cur->fatherSheet->y);
					}
				}
			}
			else if (item->type == FIFO_TYPE_TIMER)
			{
				void (*callback)() = item->pointer;
				callback();
			}
		}
	}
}

void task_b_main()
{
	while (true)
	{
		setLabelText(statusLabel, "taskb==", COL8_000000);
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
