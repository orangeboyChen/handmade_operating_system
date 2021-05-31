#include "bootpack.h"
#include <stdio.h>
#include <string.h>

void HariMain(void)
{
	init_gdtidt();
	init_pic();
	io_sti();

	struct BootInfo *bootInfo = getBootInfo();
	initPalette();

	struct MemoryManager *memoryManager = getMemoryManager();
	initMemoryManage(memoryManager);

	struct Sheet *rootSheet = initRootSheet();
	initMouseCursorSheet(rootSheet);
	initDesktop(rootSheet);
	createWindow(rootSheet, 50, 50, 200, 100, "HelloForm");

	// char s4[32];
	// sprintf(s4, "%dMB %dByte", getUnusedMemoryTotal(memoryManager) / (1024 * 1024), getUnusedMemoryTotal(memoryManager));
	// // printInSheet(sheet, 0, 0, s4, COL8_FFFFFF);
	// putfonts8_asc(getBootInfo()->vram, getBootInfo()->screenX, 0, 16, COL8_FFFFFF, s4);

	io_out8(PIC0_IMR, 0xf9);
	io_out8(PIC1_IMR, 0xef);

	while (1)
	{
		io_hlt();
	}
}
