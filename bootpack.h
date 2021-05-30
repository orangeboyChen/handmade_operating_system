//color
#define COL8_000000 0
#define COL8_FF0000 1
#define COL8_00FF00 2
#define COL8_FFFF00 3
#define COL8_0000FF 4
#define COL8_FF00FF 5
#define COL8_00FFFF 6
#define COL8_FFFFFF 7
#define COL8_C6C6C6 8
#define COL8_840000 9
#define COL8_008400 10
#define COL8_848400 11
#define COL8_000084 12
#define COL8_840084 13
#define COL8_008484 14
#define COL8_848484 15

//boot
struct BootInfo
{
    char cyls, leds, vmode, reserve;
    short screenX, screenY;
    char *vram;
};

/* naskfunc.nas */
void io_hlt(void);
void io_cli(void);
void io_sti(void);
void io_stihlt(void);
int io_in8(int port);
void io_out8(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);
void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);
int load_cr0(void);
void store_cr0(int cr0);
void asm_inthandler21(void);
void asm_inthandler27(void);
void asm_inthandler2c(void);
unsigned int memtest_sub(unsigned int start, unsigned int end);

void initPalette(void);
void setPalette(int start, int end, unsigned char *rgb);
void initBackground(char *vram, int x, int y);
void fillBox(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1);

void putfont8(char *vram, int xsize, int x, int y, char c, char *font);
void putfonts8_asc(char *vram, int xsize, int x, int y, char c, unsigned char *s);
void init_mouse_cursor8(char *mouse, char bc);
void putblock8_8(char *vram, int vxsize, int pxsize,
                 int pysize, int px0, int py0, char *buf, int bxsize);

struct BootInfo *getBootInfo();
struct MemoryManager *getMemoryManager();
void logger(char *c, unsigned int y);

//sheet.c
#define MEMMAN_ADDR 0x003c0000
#define SHEET_MAX 512
typedef struct SheetManager
{
    struct Sheet *topSheet;
    struct Sheet *bottomSheet;
    struct Sheet *sheetStore[SHEET_MAX];
    struct Sheet *fatherSheet;
};

typedef struct Sheet
{
    short width, height;
    short x, y;
    int isUsing;
    struct SheetManager *sheetManager;
    struct Sheet *nextSheet, *previousSheet; //nextSheet下一层，previousSheet上一层
    char *vram;
    int index;
};

void initRootSheetManager(struct SheetManager *sheetManager);
void initSheetManager(struct Sheet *fatherSheet, struct SheetManager *sheetManager, short x, short y, unsigned char c);
struct Sheet *insertSheetToTop(struct SheetManager *sheetManager, short x1, short y1, short width, short height);
struct Sheet *createSheet(struct SheetManager *sheetManager);
void releaseSheet(struct SheetManager *sheetManager, struct Sheet *sheet);
void refreshSheet(struct Sheet *sheet);
void fillVram(struct Sheet *sheet, unsigned char c);

//memory.c
struct MemoryBlock
{
    unsigned int addrFrom;
    unsigned int addrTo;
    struct MemoryBlock *nextBlock;
    unsigned int blockType;

    unsigned int requestSize;
    unsigned int taskId;
    unsigned int isUsing;
};

struct MemoryManager
{
    struct MemoryBlock *firstBlock;
    struct MemoryBlock blocks[2048];
};

void initMemoryManage(struct MemoryManager *memoryManager);
void initMemoryManager(struct MemoryManager *memoryManager, unsigned int start, unsigned int end);
unsigned int allocaMemory(struct MemoryManager *memoryManager, unsigned int size);
struct MemoryBlock *getUnusedBlock(struct MemoryManager *memoryManager);
void combineMemoryBlock(struct MemoryManager *memoryManager);
unsigned int getPreferBlockType(struct MemoryManager *memoryManager, unsigned int size);
unsigned int getUnusedMemoryTotal(struct MemoryManager *memoryManager);
int releaseBlock(struct MemoryManager *memoryManager, unsigned int addr);
