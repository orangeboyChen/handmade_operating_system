#include "bootpack.h"
#include <stdio.h>
#include <string.h>

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
	struct Sheet *win = createWindow(rootSheet, 30, 30, 200, 100, "Father2");
	// forceUpdateSheet(win);

	createWindow(win, 5, 5 + 18, 180, 40, "Son1");

	// updateIndexMap(win);
	// updateIndexMap(rootSheet);
	// forceUpdateSheet(win);
	// createWindow(win, 20, 20, 80, 40, "Son2");
	// createWindow(rootSheet, 150, 120, 150, 60, "HelloForm3");

	// updateAllSubsheet(rootSheet);

	// char s4[32];
	// sprintf(s4, "%dMB %dByte", getUnusedMemoryTotal(memoryManager) / (1024 * 1024), getUnusedMemoryTotal(memoryManager));
	// // printInSheet(sheet, 0, 0, s4, COL8_FFFFFF);
	// putfonts8_asc(getBootInfo()->vram, getBootInfo()->screenX, 0, 16, COL8_FFFFFF, s4);

	io_out8(PIC0_IMR, 0xf9);
	io_out8(PIC1_IMR, 0xef);

	initKeyboard();
	enableMouse();

	while (1)
	{
		io_cli();
		struct FifoItem *item = getInFifo(&systemFifo);
		if (item == NULL)
		{
			io_stihlt();
		}
		else
		{
			io_sti();
			if (item->type == FIFO_TYPE_KEYBOARD)
			{
				char s4[32];
				sprintf(s4, "%02X", item->data);
				// printInSheet(sheet, 0, 0, s4, COL8_FFFFFF);
				fillInSheet(rootSheet->bottomSheet, 0, 16, 16, 16, COL8_000000);
				printInSheet(rootSheet->bottomSheet, 0, 16, s4, COL8_FFFFFF);
			}
			else if (item->type == FIFO_TYPE_MOUSE)
			{
				if (putInMouseData(&mouseData, item->data) == 1)
				{
					updateMouseCursorSheet(mouseData.moveX, mouseData.moveY);

					// struct Sheet *cs = rootSheet->sheetStore[rootSheet->indexMap[mouseData.y * rootSheet->width + (mouseData.x - 1)]];
					int x = mouseData.x, y = mouseData.y;
					struct Sheet *cs = rootSheet->sheetStore[rootSheet->indexMap[y * rootSheet->width + (x - 1)]];
					// while (true)
					// {
					// 	x -= cs->x;
					// 	y -= cs->y;
					// 	if (cs->indexMap[y * cs->width + x] != 0)
					// 	{
					// 		cs = cs->indexMap[y * cs->width + x];
					// 	}
					// 	else
					// 	{
					// 		cs = cs->fatherSheet;
					// 		break;
					// 	}
					// }
					// cs = cs->topSheet;
					x -= cs->x;
					y -= cs->y;

					char s4[32];
					sprintf(s4, "%d-%d %d-%d %d %d %d %d",
							rootSheet->vram[mouseData.y * rootSheet->width + (mouseData.x - 1)],
							rootSheet->indexMap[mouseData.y * rootSheet->width + (mouseData.x - 1)],
							cs->vram[y * cs->width + (x - 1)],
							cs->indexMap[y * cs->width + (x - 1)],
							cs->index,
							cs->fatherSheet->index,
							x,
							y
							// rootSheet->topSheet->index,
							// rootSheet->topSheet->nextSheet->index,
							// rootSheet->topSheet->nextSheet->nextSheet->index
							// rootSheet->topSheet->nextSheet->nextSheet->nextSheet->index,
							// rootSheet->topSheet->nextSheet->nextSheet->nextSheet->nextSheet->index,
							// rootSheet->topSheet->nextSheet->nextSheet->nextSheet->nextSheet->nextSheet->index
					);
					// printInSheet(sheet, 0, 0, s4, COL8_FFFFFF);
					// fillInSheet(rootSheet->bottomSheet, 0, 32, 32 * 10, 16, COL8_000000);
					// printInSheet(rootSheet->bottomSheet, 0, 32, s4, COL8_FFFFFF);

					// char s5[32];
					// struct Sheet *cs = rootSheet->topSheet->nextSheet->nextSheet;
					// sprintf(s5, "%d %d", rootSheet->vram[mouseData.y * rootSheet->width + mouseData.x],
					// 		cs->vram[(mouseData.y - cs->height) * cs->width + (mouseData.x - cs->width)]);
					// // printInSheet(sheet, 0, 0, s4, COL8_FFFFFF);
					// fillInSheet(rootSheet->bottomSheet, 0, 64, 8 * 10, 16, COL8_000000);
					// printInSheet(rootSheet->bottomSheet, 0, 64, s5, COL8_FFFFFF);

					// if (mouseData.x > 120)
					// {
					// 	moveSheet(rootSheet->topSheet->nextSheet->nextSheet->nextSheet, 20, 20);
					// }
					// else
					// {
					// 	moveSheet(rootSheet->topSheet->nextSheet->nextSheet->nextSheet, 70, 20);
					// }

					// if ((mouseData.btn & 0x01) == 0x01)
					if (mouseData.btn == 0x01)
					{
						moveSheet(rootSheet->topSheet->nextSheet->nextSheet, mouseData.x, mouseData.y);
					}
					// if ((mouseData.btn & 0x02) == 0x02)
					if (mouseData.btn == 0x02)
					{
						struct Sheet *cur = rootSheet->topSheet->nextSheet->nextSheet->topSheet->nextSheet;
						moveSheet(cur, mouseData.x - cur->fatherSheet->x, mouseData.y - cur->fatherSheet->y);
					}

					// if ((mouseData.btn & 0x04) == 0x04)
					if (mouseData.btn == 0x04)
					{
						struct Sheet *cur = rootSheet->topSheet->nextSheet->nextSheet->nextSheet;
						moveSheet(cur, mouseData.x - cur->fatherSheet->x, mouseData.y - cur->fatherSheet->y);

						// struct Sheet *cur = rootSheet->topSheet->nextSheet->nextSheet->topSheet->nextSheet->nextSheet;
						// moveSheet(cur, mouseData.x - cur->fatherSheet->x, mouseData.y - cur->fatherSheet->y);
						// moveSheet(rootSheet->topSheet->nextSheet->nextSheet->nextSheet->nextSheet, mouseData.x, mouseData.y);
					}

					if (mouseData.btn == 3)
					{
						// struct Sheet *cur = rootSheet->topSheet->nextSheet->nextSheet->nextSheet;
						// moveSheet(cur, mouseData.x - cur->fatherSheet->x, mouseData.y - cur->fatherSheet->y);
						// moveSheet(rootSheet->topSheet->nextSheet->nextSheet->nextSheet->nextSheet, mouseData.x, mouseData.y);
					}
				}
			}
		}
	}
}
