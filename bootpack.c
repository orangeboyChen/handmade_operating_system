#include "bootpack.h"
#include <stdio.h>
#include <string.h>

void initPalette(void)
{
	static unsigned char table_rgb[16 * 3] = {
		0x00, 0x00, 0x00, /*  0:�� */
		0xff, 0x00, 0x00, /*  1:���邢�� */
		0x00, 0xff, 0x00, /*  2:���邢�� */
		0xff, 0xff, 0x00, /*  3:���邢���F */
		0x00, 0x00, 0xff, /*  4:���邢�� */
		0xff, 0x00, 0xff, /*  5:���邢�� */
		0x00, 0xff, 0xff, /*  6:���邢���F */
		0xff, 0xff, 0xff, /*  7:�� */
		0xc6, 0xc6, 0xc6, /*  8:���邢�D�F */
		0x84, 0x00, 0x00, /*  9:�Â��� */
		0x00, 0x84, 0x00, /* 10:�Â��� */
		0x84, 0x84, 0x00, /* 11:�Â����F */
		0x00, 0x00, 0x84, /* 12:�Â��� */
		0x84, 0x00, 0x84, /* 13:�Â��� */
		0x00, 0x84, 0x84, /* 14:�Â����F */
		0x84, 0x84, 0x84  /* 15:�Â��D�F */
	};
	setPalette(0, 15, table_rgb);
	return;
}

void setPalette(int start, int end, unsigned char *rgb)
{
	int i, eflags;
	eflags = io_load_eflags(); /* ���荞�݋��t���O�̒l���L�^���� */
	io_cli();				   /* ���t���O��0�ɂ��Ċ��荞�݋֎~�ɂ��� */
	io_out8(0x03c8, start);
	for (i = start; i <= end; i++)
	{
		io_out8(0x03c9, rgb[0] / 4);
		io_out8(0x03c9, rgb[1] / 4);
		io_out8(0x03c9, rgb[2] / 4);
		rgb += 3;
	}
	io_store_eflags(eflags); /* ���荞�݋��t���O�����ɖ߂� */
	return;
}

void fillBox(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1)
{
	int x, y;
	for (y = y0; y <= y1; y++)
	{
		for (x = x0; x <= x1; x++)
			vram[y * xsize + x] = c;
	}
	return;
}

void initBackground(char *vram, int x, int y)
{
	// fillBox(vram, x, COL8_008484, 0, 0, x - 1, y - 29);
	// fillBox(vram, x, COL8_C6C6C6, 0, y - 28, x - 1, y - 28);
	// fillBox(vram, x, COL8_FFFFFF, 0, y - 27, x - 1, y - 27);
	// fillBox(vram, x, COL8_C6C6C6, 0, y - 26, x - 1, y - 1);

	// fillBox(vram, x, COL8_FFFFFF, 3, y - 24, 59, y - 24);
	// fillBox(vram, x, COL8_FFFFFF, 2, y - 24, 2, y - 4);
	// fillBox(vram, x, COL8_848484, 3, y - 4, 59, y - 4);
	// fillBox(vram, x, COL8_848484, 59, y - 23, 59, y - 5);
	// fillBox(vram, x, COL8_000000, 2, y - 3, 59, y - 3);
	// fillBox(vram, x, COL8_000000, 60, y - 24, 60, y - 3);

	// fillBox(vram, x, COL8_848484, x - 47, y - 24, x - 4, y - 24);
	// fillBox(vram, x, COL8_848484, x - 47, y - 23, x - 47, y - 4);
	// fillBox(vram, x, COL8_FFFFFF, x - 47, y - 3, x - 4, y - 3);
	// fillBox(vram, x, COL8_FFFFFF, x - 3, y - 24, x - 3, y - 3);
	fillBox(vram, x, COL8_0000FF, 0, 0, x - 1, y - 1);
	return;
}

void putfont8(char *vram, int xsize, int x, int y, char c, char *font)
{
	int i;
	char *p, d /* data */;
	for (i = 0; i < 16; i++)
	{
		p = vram + (y + i) * xsize + x;
		d = font[i];
		if ((d & 0x80) != 0)
		{
			p[0] = c;
		}
		if ((d & 0x40) != 0)
		{
			p[1] = c;
		}
		if ((d & 0x20) != 0)
		{
			p[2] = c;
		}
		if ((d & 0x10) != 0)
		{
			p[3] = c;
		}
		if ((d & 0x08) != 0)
		{
			p[4] = c;
		}
		if ((d & 0x04) != 0)
		{
			p[5] = c;
		}
		if ((d & 0x02) != 0)
		{
			p[6] = c;
		}
		if ((d & 0x01) != 0)
		{
			p[7] = c;
		}
	}
	return;
}

void putfonts8_asc(char *vram, int xsize, int x, int y, char c, unsigned char *s)
{
	extern char hankaku[4096];
	for (; *s != 0x00; s++)
	{
		putfont8(vram, xsize, x, y, c, hankaku + *s * 16);
		x += 8;
	}
	return;
}

void init_mouse_cursor8(char *mouse, char bc)
/* �}�E�X�J�[�\���������i16x16�j */
{
	static char cursor[16][16] = {
		"**************..",
		"*OOOOOOOOOOO*...",
		"*OOOOOOOOOO*....",
		"*OOOOOOOOO*.....",
		"*OOOOOOOO*......",
		"*OOOOOOO*.......",
		"*OOOOOOO*.......",
		"*OOOOOOOO*......",
		"*OOOO**OOO*.....",
		"*OOO*..*OOO*....",
		"*OO*....*OOO*...",
		"*O*......*OOO*..",
		"**........*OOO*.",
		"*..........*OOO*",
		"............*OO*",
		".............***"};
	int x, y;

	for (y = 0; y < 16; y++)
	{
		for (x = 0; x < 16; x++)
		{
			if (cursor[y][x] == '*')
			{
				mouse[y * 16 + x] = COL8_000000;
			}
			if (cursor[y][x] == 'O')
			{
				mouse[y * 16 + x] = COL8_FFFFFF;
			}
			if (cursor[y][x] == '.')
			{
				mouse[y * 16 + x] = bc;
			}
		}
	}
	return;
}

void putblock8_8(char *vram, int vxsize, int pxsize,
				 int pysize, int px0, int py0, char *buf, int bxsize)
{
	int x, y;
	for (y = 0; y < pysize; y++)
	{
		for (x = 0; x < pxsize; x++)
		{
			vram[(py0 + y) * vxsize + (px0 + x)] = buf[y * bxsize + x];
		}
	}
	return;
}

void HariMain(void)
{
	struct BootInfo *bootInfo = getBootInfo();
	initPalette();
	// initBackground(bootInfo->vram, bootInfo->screenX, bootInfo->screenY);

	// char *s;
	// sprintf(s, "%dB", memtest_sub(0x00400000, 0xbfffffff));
	// putfonts8_asc(getBootInfo()->vram, getBootInfo()->screenX, 0, 0, COL8_FFFFFF, s);

	struct MemoryManager *memoryManager = getMemoryManager();
	initMemoryManage(memoryManager);

	// struct SheetManager *sheetManager;
	struct Sheet *rootSheet = initRootSheet();
	struct Sheet *sheet = createSubsheetToTop(rootSheet, 10, 10, 300, 120);
	fillVram(sheet, COL8_008400);
	// forceUpdateSheet(sheet);
	// updateSheet(sheet);

	struct Sheet *sheet2 = createSubsheetToTop(sheet, 20, 40, 50, 50);
	fillVram(sheet2, COL8_FFFFFF);
	// updateSheet(sheet2);
	// forceUpdateSheet(sheet2);

	// updateAllSubsheet(rootSheet);
	// forceUpdateSheet(sheet2);
	// forceUpdateSheet(sheet);

	fillInSheet(sheet, 0, 32, sheet->width, 2, COL8_848400);
	fillInSheet(sheet, 0, 48, sheet->width, 2, COL_TRANSPARENT);
	fillInSheet(sheet2, 0, 15, 10, 10, COL_TRANSPARENT);
	fillInSheet(sheet2, 20, 15, 10, 10, COL_TRANSPARENT);

	updateSheetIndexMap(rootSheet);
	forceUpdateSheet(sheet2);
	forceUpdateSheet(sheet);

	// fillVramByIndexMap(rootSheet);
	// updateSheet(rootSheet);
	// updateSheet(sheet);
	// updateSheet(sheet2);

	// forceUpdateSheet(sheet2);

	// updateAllSubsheet(rootSheet);

	// char s3[32];
	// sprintf(s3, "Mem: %uMB", getUnusedMemoryTotal(memoryManager) / (1024 * 1024));
	// // putfonts8_asc(getBootInfo()->vram, getBootInfo()->screenX, 0, 0, COL8_FFFFFF, s3);
	// printInSheet(sheet, 0, 0, s3, COL8_FFFFFF);

	char s4[32];
	sprintf(s4, "%u %u", rootSheet->indexMap[(15 + sheet2->y) * rootSheet->width + sheet2->x], sheet2->vram[15 * sheet2->width + 20]);
	printInSheet(sheet, 0, 0, s4, COL8_FFFFFF);
	// putfonts8_asc(getBootInfo()->vram, getBootInfo()->screenX, 0, 0, COL8_FFFFFF, s4);

	while (1)
	{
		io_hlt();
	}
}

void logger(char *c, unsigned int y)
{

	char *s;
	sprintf(s, c);
	putfonts8_asc(getBootInfo()->vram, getBootInfo()->screenX, y, 0, COL8_FFFFFF, s);
}

struct BootInfo *getBootInfo()
{
	return (struct BootInfo *)0x0ff0;
}

struct MemoryManager *getMemoryManager()
{
	return (struct MemoryManager *)MEMMAN_ADDR;
}